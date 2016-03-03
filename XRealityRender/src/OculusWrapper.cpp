#include "OculusWrapper.h"

#include "Win32_GLAppUtil.h"
#include "Kernel/OVR_System.h"
#include "XREntity.h"

namespace OculusWrapper
{
	static TextureBuffer * eyeRenderTexture[2] = { nullptr, nullptr };
	static DepthBuffer   * eyeDepthBuffer[2] = { nullptr, nullptr };
	static ovrGLTexture  * mirrorTexture = nullptr;
	static GLuint          mirrorFBO = 0;

	static ovrHmd HMD;
	static ovrGraphicsLuid luid;
	static ovrResult result;
	static ovrHmdDesc hmdDesc;
	static bool retryCreate;
	static ovrEyeRenderDesc EyeRenderDesc[2];

	static Matrix4f view;
	static Matrix4f proj;


	bool start()
	{
		OVR::System::Init();
		result = ovr_Initialize(NULL);
		if (!OVR_SUCCESS(result))
		{
			return false;
		}
		// Initializes LibOVR, and the 
		result = ovr_Create(&HMD, &luid);
		if (!OVR_SUCCESS(result))
		{
			return false;
		}
		hmdDesc = ovr_GetHmdDesc(HMD);
		//ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };
		return true;
	}


	bool init()
	{
		retryCreate = false;
		//TODO: make these variables configurable!;
		eyeDepthBuffer[0] = NULL;
		eyeDepthBuffer[1] = NULL;
		mirrorTexture = nullptr;
		GLuint          mirrorFBO = 0;

		//ovrResult result = ovr_Create(&HMD, &luid);
		//if (!OVR_SUCCESS(result))
		//	return retryCreate;

		//hmdDesc = ovr_GetHmdDesc(HMD);

		VALIDATE(Platform.InitWindow(0, L"XR render (GL)"), "Failed to open window.");
		ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };
		if (!Platform.InitDevice(windowSize.w, windowSize.h, reinterpret_cast<LUID*>(&luid)))
			return false;

		// Make eye render buffers
		for (int eye = 0; eye < 2; ++eye)
		{
			ovrSizei idealTextureSize = ovr_GetFovTextureSize(HMD, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
			eyeRenderTexture[eye] = new TextureBuffer(HMD, true, true, idealTextureSize, 1, NULL, 1);
			eyeDepthBuffer[eye] = new DepthBuffer(eyeRenderTexture[eye]->GetSize(), 0);

			if (!eyeRenderTexture[eye]->TextureSet)
			{
				if (retryCreate) return false;
				VALIDATE(false, "Failed to create texture.");
			}
		}

		// Create mirror texture and an FBO used to copy mirror texture to back buffer
		result = ovr_CreateMirrorTextureGL(HMD, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h, reinterpret_cast<ovrTexture**>(&mirrorTexture));
		if (!OVR_SUCCESS(result))
		{
			if (retryCreate) return false;
			VALIDATE(false, "Failed to create mirror texture.");
		}

		// Configure the mirror read buffer
		glGenFramebuffers(1, &mirrorFBO);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
		glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		EyeRenderDesc[0] = ovr_GetRenderDesc(HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
		EyeRenderDesc[1] = ovr_GetRenderDesc(HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

		// Turn off vsync to let the compositor do its magic
		wglSwapIntervalEXT(0);

		return true;
	}
	bool update(double time, XREntity* scene)
	{
		if (Platform.HandleMessages())
		{
			// Keyboard inputs to adjust player orientation
			static float Yaw(3.141592f);
			if (Platform.Key[VK_LEFT])  Yaw += 0.02f;
			if (Platform.Key[VK_RIGHT]) Yaw -= 0.02f;

			// Keyboard inputs to adjust player position
			static Vector3f Pos2(0.0f, 1.6f, -5.0f);
			if (Platform.Key['W'] || Platform.Key[VK_UP])     Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(0, 0, -0.05f));
			if (Platform.Key['S'] || Platform.Key[VK_DOWN])   Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(0, 0, +0.05f));
			if (Platform.Key['D'])                          Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(+0.05f, 0, 0));
			if (Platform.Key['A'])                          Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(-0.05f, 0, 0));
			Pos2.y = ovr_GetFloat(HMD, OVR_KEY_EYE_HEIGHT, Pos2.y);

			// Animate the cube
			//static float cubeClock = 0;
			//roomScene->Models[0]->Pos = Vector3f(9 * sin(cubeClock), 3, 9 * cos(cubeClock += 0.015f));

			// Get eye poses, feeding in correct IPD offset
			ovrVector3f               ViewOffset[2] = { EyeRenderDesc[0].HmdToEyeViewOffset,
				EyeRenderDesc[1].HmdToEyeViewOffset };
			ovrPosef                  EyeRenderPose[2];

			double           ftiming = ovr_GetPredictedDisplayTime(HMD, 0);
			// Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
			double           sensorSampleTime = ovr_GetTimeInSeconds();
			ovrTrackingState hmdState = ovr_GetTrackingState(HMD, ftiming, ovrTrue);
			ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ViewOffset, EyeRenderPose);


			for (int eye = 0; eye < 2; ++eye)
			{
				// Increment to use next texture, just before writing
				eyeRenderTexture[eye]->TextureSet->CurrentIndex = (eyeRenderTexture[eye]->TextureSet->CurrentIndex + 1) % eyeRenderTexture[eye]->TextureSet->TextureCount;

				// Switch to eye render target
				eyeRenderTexture[eye]->SetAndClearRenderSurface(eyeDepthBuffer[eye]);

				// Get view and projection matrices
				Matrix4f rollPitchYaw = Matrix4f::RotationY(Yaw);
				Matrix4f finalRollPitchYaw = rollPitchYaw * Matrix4f(EyeRenderPose[eye].Orientation);
				Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
				Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
				Vector3f shiftedEyePos = Pos2 + rollPitchYaw.Transform(EyeRenderPose[eye].Position);

				view = Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);
				proj = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[eye], 0.2f, 1000.0f, ovrProjection_RightHanded);
				
				//clear buffer
				GLfloat defaultBufferColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

				// Render world
				//roomScene->Render(view, proj);
				scene->update(time);
				


				// Avoids an error when calling SetAndClearRenderSurface during next iteration.
				// Without this, during the next while loop iteration SetAndClearRenderSurface
				// would bind a framebuffer with an invalid COLOR_ATTACHMENT0 because the texture ID
				// associated with COLOR_ATTACHMENT0 had been unlocked by calling wglDXUnlockObjectsNV.
				eyeRenderTexture[eye]->UnsetRenderSurface();
			}

			// Do distortion rendering, Present and flush/sync

			// Set up positional data.
			ovrViewScaleDesc viewScaleDesc;
			viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
			viewScaleDesc.HmdToEyeViewOffset[0] = ViewOffset[0];
			viewScaleDesc.HmdToEyeViewOffset[1] = ViewOffset[1];

			ovrLayerEyeFov ld;
			ld.Header.Type = ovrLayerType_EyeFov;
			ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

			for (int eye = 0; eye < 2; ++eye)
			{
				ld.ColorTexture[eye] = eyeRenderTexture[eye]->TextureSet;
				ld.Viewport[eye] = Recti(eyeRenderTexture[eye]->GetSize());
				ld.Fov[eye] = hmdDesc.DefaultEyeFov[eye];
				ld.RenderPose[eye] = EyeRenderPose[eye];
				ld.SensorSampleTime = sensorSampleTime;
			}

			ovrLayerHeader* layers = &ld.Header;
			ovrResult result = ovr_SubmitFrame(HMD, 0, &viewScaleDesc, &layers, 1);
			// exit the rendering loop if submit returns an error, will retry on ovrError_DisplayLost
			if (!OVR_SUCCESS(result))
				return false;

			// Blit mirror texture to back buffer
			glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			GLint w = mirrorTexture->OGL.Header.TextureSize.w;
			GLint h = mirrorTexture->OGL.Header.TextureSize.h;
			glBlitFramebuffer(0, h, w, 0,
				0, 0, w, h,
				GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

			SwapBuffers(Platform.hDC);
		}

		return true;
	}


	bool destroy()
	{
		if (mirrorFBO) glDeleteFramebuffers(1, &mirrorFBO);
		if (mirrorTexture) ovr_DestroyMirrorTexture(HMD, reinterpret_cast<ovrTexture*>(mirrorTexture));
		for (int eye = 0; eye < 2; ++eye)
		{
			delete eyeRenderTexture[eye];
			delete eyeDepthBuffer[eye];
		}
		Platform.ReleaseDevice();
		ovr_Destroy(HMD);
		OVR::System::Destroy();

		// Retry on ovrError_DisplayLost
		return retryCreate || OVR_SUCCESS(result) || (result == ovrError_DisplayLost);
		return true;
	}

	glm::mat4 getWorld2Clip()
	{
		glm::mat4 matPerspProj = getPersProj();
		glm::mat4 matWorld2View = getWorld2View();;
		return matPerspProj * matWorld2View;
	}

	glm::mat4 getWorld2View()
	{
		float *ptr = (FLOAT*)&view;
		glm::mat4 view(ptr[0], ptr[4], ptr[8], ptr[12],
			ptr[1], ptr[5], ptr[9], ptr[13],
			ptr[2], ptr[6], ptr[10], ptr[14],
			ptr[3], ptr[7], ptr[11], ptr[15]);
		return view;
	}

	glm::mat4 getPersProj()
	{
		float *ptr = (FLOAT*)&proj;
		glm::mat4 persproj(ptr[0], ptr[4], ptr[8], ptr[12],
			ptr[1], ptr[5], ptr[9], ptr[13],
			ptr[2], ptr[6], ptr[10], ptr[14],
			ptr[3], ptr[7], ptr[11], ptr[15]);
		return persproj;
	}


}


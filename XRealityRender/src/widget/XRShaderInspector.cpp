#include "XRShaderInspector.h"
#include "../XRDebug.h"
#include "../XRIMUI.h"
#include "../XRShaderManager.h"


const char * type_to_name(GLenum type);

bool XRShaderInspector::init()
{
	return true;
}

bool XRShaderInspector::update(double time)
{
	ImGui::Begin("Shaders");

	vector<XRShaderManger::ProgramDesciptor> descriptors;
	XRShaderManger::getAllDescriptors(descriptors);

	for (int i = 0; i < descriptors.size(); i++)
	{
		const XRShaderManger::ProgramDesciptor& descriptor = descriptors[i];
		if (ImGui::TreeNode(descriptor.name.c_str()))
		{
			ImGui::TextWrapped(descriptor.description.c_str());

			if (!descriptor.ready)
			{
				ImGui::TextColored(ImVec4(1, 0, 1, 1), "Not Ready");
			}
			else
			{
				if (ImGui::TreeNode("Input:"))
				{
					//get the number of active input resources
					GLint inputs;
					glGetProgramInterfaceiv(descriptor.program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &inputs);

					int length;
					char name[100];
					const static GLenum props[] = { GL_TYPE, GL_LOCATION };
					GLint paras[2];
					for (int i = 0; i < inputs; i++)
					{
						//get resource's name
						glGetProgramResourceName(descriptor.program, GL_PROGRAM_INPUT, i, 100, &length, name);

						//get other property 
						glGetProgramResourceiv(descriptor.program, GL_PROGRAM_INPUT, i, 2, props, 2, NULL, paras);

						//get type_name and location
						const char* type_name = type_to_name(paras[0]);
						int location = paras[1];

						ImGui::TextColored(ImVec4(0, 1, 1, 1), "Index %d: %s %s @ location %d", i, type_name, name, location);

					}
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Uniform:"))
				{
					//get the number of active input resources
					GLint uniforms;
					glGetProgramInterfaceiv(descriptor.program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniforms);

					int length;
					char name[100];
					const static GLenum props[] = { GL_TYPE, GL_LOCATION };
					GLint paras[2];
					for (int i = 0; i < uniforms; i++)
					{
						//get resource's name
						glGetProgramResourceName(descriptor.program, GL_UNIFORM, i, 100, &length, name);

						//get other property 
						glGetProgramResourceiv(descriptor.program, GL_UNIFORM, i, 2, props, 2, NULL, paras);

						//get type_name and location
						const char* type_name = type_to_name(paras[0]);
						int location = paras[1];

						ImGui::TextColored(ImVec4(0, 1, 1, 1), "Index %d: %s %s @ location %d", i, type_name, name, location);

					}
					ImGui::TreePop();
				}

			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
	return true;
}

bool XRShaderInspector::destroy()
{
	return true;
}


const struct type_to_name_entry
{
	GLenum type;
	const char * name;
}

type_to_name_table[] =
{
	GL_FLOAT, "float",
	GL_FLOAT_VEC2, "vec2",
	GL_FLOAT_VEC3, "vec3",
	GL_FLOAT_VEC4, "vec4",
	GL_DOUBLE, "double",
	GL_DOUBLE_VEC2, "dvec2",
	GL_DOUBLE_VEC3, "dvec3",
	GL_DOUBLE_VEC4, "dvec4",
	GL_INT, "int",
	GL_INT_VEC2, "ivec2",
	GL_INT_VEC3, "ivec3",
	GL_INT_VEC4, "ivec4",
	GL_UNSIGNED_INT, "uint",
	GL_UNSIGNED_INT_VEC2, "uvec2",
	GL_UNSIGNED_INT_VEC3, "uvec3",
	GL_UNSIGNED_INT_VEC4, "uvec4",
	GL_BOOL, "bool",
	GL_BOOL_VEC2, "bvec2",
	GL_BOOL_VEC3, "bvec3",
	GL_BOOL_VEC4, "bvec4",
	GL_FLOAT_MAT4, "mat4",
	GL_FLOAT_MAT3, "mat3",
	GL_FLOAT_MAT2, "mat2",
	GL_SAMPLER_2D, "sampler2d",
	GL_NONE, NULL
};

const char * type_to_name(GLenum type)
{
	for (const type_to_name_entry * ptr = &type_to_name_table[0]; ptr->name != NULL; ptr++)
	{
		if (ptr->type == type)
			return ptr->name;
	}

	return NULL;
}
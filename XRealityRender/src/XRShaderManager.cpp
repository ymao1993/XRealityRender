#include "XRShaderManager.h"
#include "XRShaderDescriptor.h"
#include "XRDebug.h"
#include "utils/XRShaderUtils.h"
#include "utils/XRBinaryIO.h"

using namespace std;

namespace XRShaderManger{

	//DATA
	static const string SHADER_FOLDER_PATH = "./res/shader/";
	static const string BINARY_FORMAT_PATH = "./res/shader/BinaryFormat";
	static map<XR_SHADER_PROGRAM_TOKEN, ProgramDesciptor> programs =
	{
		{ XR_SHADER_PROGRAM_SINGLE_COLOR,	  { "SingleColor", "SingleColor is a a simple shader that draw everything in a single color. It is used mainly for debugging visualization." } },
		{ XR_SHADER_PROGRAM_TEXTURE_MAPPING,  { "TextureMapping", "TextureMapping performs texture mapping on the mesh." } },
		{ XR_SHADER_PROGRAM_PHONGLIGHTING_GS, { "PhongLighting(GS)", "PhongLighting(GS) performs phong lighting and gouraud shading." } },
		{ XR_SHADER_PROGRAM_TEXTURE_MAPPING_INSTANCED, { "TextureMappingInstanced", "Texture Mapping shader that supports instance rendering." } },
		{ XR_SHADER_PROGRAM_TERRAIN_RENDERING,{ "TerrainRendering", "Rendering Terrain using displacement mapping." } },

	};

	//forward declarations
	static int binaryFormat = -1;
	static void buildPrograms(bool binary = true);
	static void loadPrograms();
	static void buildMonoProgram(ProgramDesciptor& descriptor, bool binary = true);
	static void loadMonoProgram(ProgramDesciptor& descriptor);
	static void readBinaryFormat();
	static void writeBinaryFormat();
	static void collectAllShaderFiles(const string& folder, vector<string>& files, vector<GLenum>& stages);

	//init all shaders
	void init(bool binary)
	{
		if (!binary)
		{
			buildPrograms();
		}
		else
		{
			loadPrograms();
		}
		return;
	}

	//get monolithic shader program
	GLuint getShaderProgram(XR_SHADER_PROGRAM_TOKEN token)
	{
		if (!programs[token].ready)
		{
			buildMonoProgram(programs[token]);
		}
		return programs[token].program;
	}

	//get pre-configured shader pipeline
	GLuint getShaderPipeline(XR_SHADER_PROGRAM_TOKEN token)
	{
		//not implemented
		return 0;
	}

	//delete monolithic shader program
	void deleteShaderProgram(XR_SHADER_PROGRAM_TOKEN token)
	{
		if (programs[token].ready)
		{
			glDeleteProgram(programs[token].program);
			programs[token].program = 0;
			programs[token].ready = false;
		}
	}

	//delete all the programs
	void free()
	{
		map<XR_SHADER_PROGRAM_TOKEN, ProgramDesciptor>::iterator iter;
		for (iter = programs.begin(); iter != programs.end(); iter++)
		{
			ProgramDesciptor& descriptor = iter->second;
			glDeleteProgram(descriptor.program);
			descriptor.ready = false;
		}
	}

	void getAllDescriptors(vector<ProgramDesciptor>& descriptors)
	{
		map<XR_SHADER_PROGRAM_TOKEN, ProgramDesciptor>::iterator iter;
		for (iter = programs.begin(); iter != programs.end(); iter++)
		{
			descriptors.push_back(iter->second);
		}
	}

	//build shaders according to shader descriptions
	static void buildPrograms(bool binary)
	{
		map<XR_SHADER_PROGRAM_TOKEN, ProgramDesciptor>::iterator iter;
		for (iter = programs.begin(); iter != programs.end(); iter++)
		{
			ProgramDesciptor& descriptor = iter->second;
			XRDebug::logI("building %s", descriptor.name.c_str());
			buildMonoProgram(descriptor,binary);
		}
	}

	//build a monolithic program
	static void buildMonoProgram(ProgramDesciptor& descriptor, bool binary)
	{
		string folderPath = SHADER_FOLDER_PATH + descriptor.name;
		vector<string> shaderFiles;
		vector<GLenum> stages;
		collectAllShaderFiles(folderPath.c_str(), shaderFiles, stages);

		//compile shader objects
		GLuint shaders[10];
		int numShader = shaderFiles.size();
		for (int i = 0; i < numShader; i++)
		{
			shaders[i] = XRShaderUtils::loadShader(shaderFiles[i].c_str(), stages[i]);
		}

		//link shader program
		GLuint program;
		program = XRShaderUtils::linkShaderProgram(shaders, numShader, true, binary);

		if (binary)
		{
			GLint size;
			GLenum format;
			vector<char> data;
			glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &size);
			data.resize(size);
			glGetProgramBinary(program, size, NULL, &format, &data[0]);
			string binaryFileName = SHADER_FOLDER_PATH + descriptor.name + "/" + descriptor.name + ".bin";
			XRBinaryIO::writeBinary(binaryFileName, data);
			if (binaryFormat == -1)
			{
				binaryFormat = format;
				writeBinaryFormat();
			}
		}

		//mark the program as ready
		descriptor.program = program;
		descriptor.ready = true;
		
		return;
	}

	static void loadPrograms()
	{
		map<XR_SHADER_PROGRAM_TOKEN, ProgramDesciptor>::iterator iter;
		for (iter = programs.begin(); iter != programs.end(); iter++)
		{
			ProgramDesciptor& descriptor = iter->second;
			XRDebug::logI("loading %s", descriptor.name.c_str());
			loadMonoProgram(descriptor);
		}
	}

	//build a monolithic program
	static void loadMonoProgram(ProgramDesciptor& descriptor)
	{
		if (binaryFormat == -1)
		{
			readBinaryFormat();
			if (binaryFormat == -1)
			{
				buildMonoProgram(descriptor);
				return;
			}
		}

		string filePath = SHADER_FOLDER_PATH + descriptor.name + "/" + descriptor.name + ".bin";
		vector<char> binary;
		XRBinaryIO::readBinary(filePath, binary);
		if (binary.size() == 0)
		{
			buildMonoProgram(descriptor);
			return;
		}
		GLuint program;
		program = glCreateProgram();
		glProgramBinary(program, binaryFormat, &binary[0], binary.size());

		//mark the program as ready
		descriptor.program = program;
		descriptor.ready = true;

		return;
	}

	static void readBinaryFormat()
	{
		ifstream fin;
		fin.open(BINARY_FORMAT_PATH);
		if (!fin.good()) return;
		fin >> binaryFormat;
		fin.close();
	}

	static void writeBinaryFormat()
	{
		ofstream fout;
		fout.open(BINARY_FORMAT_PATH);
		fout << binaryFormat;
		fout.close();
	}

	static void collectAllShaderFiles(const string& folder, vector<string>& files, vector<GLenum>& stages)
	{
		WIN32_FIND_DATA file;
		HANDLE fileHandle;
		char wildCard[1024];
		sprintf(wildCard, "%s\\*", folder.c_str());
		fileHandle = FindFirstFile(wildCard, &file);

		do
		{
			string filename = file.cFileName;

			// It is a directory
			if ((file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				if (filename != "."
					&& filename != "..")
				{
					collectAllShaderFiles(folder + "/" + file.cFileName, files, stages);
				}
			}
			else  // It is a file
			{

				if (filename.find(".vs.") != string::npos)
				{
					files.push_back(folder + "/" + filename);
					stages.push_back(GL_VERTEX_SHADER);
				}
				else if (filename.find(".fs.") != string::npos)
				{
					files.push_back(folder + "/" + filename);
					stages.push_back(GL_FRAGMENT_SHADER);
				}
				else if (filename.find(".tcs.") != string::npos)
				{
					files.push_back(folder + "/" + filename);
					stages.push_back(GL_TESS_CONTROL_SHADER);
				}
				else if (filename.find(".tes.") != string::npos)
				{
					files.push_back(folder + "/" + filename);
					stages.push_back(GL_TESS_EVALUATION_SHADER);
				}
			}
		} while (FindNextFile(fileHandle, &file));
	}
}
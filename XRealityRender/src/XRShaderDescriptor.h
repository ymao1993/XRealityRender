#ifndef XRSHADERDESCRIPTOR_H
#define XRSHADERDESCRIPTOR_H

#include "XRCommon.h"
#include <map>

namespace XRShaderManger{

	struct ProgramDesciptor
	{
		/*shader name*/
		string name;

		/*detailed description*/
		string description;

		/*shader program*/
		GLuint program;

		/*ready?*/
		bool ready;

		/*constructor*/
		ProgramDesciptor():program(0), ready(false){}
		ProgramDesciptor(string shaderName, string description) : name(shaderName), description(description), program(0), ready(false){}
	};

}



#endif
#ifndef XRCOMMON_H
#define XRCOMMON_H

/*gl loading library*/
#include "gl3w\gl3w.h"

/*glmath types*/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mat3;
using glm::mat4;
using glm::rotate;
using glm::translate;
using glm::scale;
using glm::value_ptr;

/*common engine headers*/
#include "XRDebug.h"
#include "XRDevice.h"

/*common stl functions*/
#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::max;
using std::min;
using std::vector;
using std::string;

/* constant*/
#define INF_D 1e100

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1500

#endif
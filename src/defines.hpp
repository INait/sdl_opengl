#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

#define GL_GLEXT_PROTOTYPES

#ifdef _WIN32
	#include <Windows.h>
	#include <SOIL.h>
	#include <SDL.h>

	#define GLM_FORCE_RADIANS
	#include <glm/glm.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include <glm/gtx/transform.hpp>
	#include <glm/gtx/quaternion.hpp>

	#undef main
	#define _CRT_SECURE_NO_WARNINGS
#else
	#include <SOIL/SOIL.h>
	#include <SDL2/SDL.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "wavefront_loader/wavefront_loader.hpp"

#endif // _DEFINES_HPP_

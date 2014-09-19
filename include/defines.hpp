#ifndef _DEFINES_HPP_
#define _DEFINES_HPP_

#define GL_GLEXT_PROTOTYPES

#ifdef _WIN32
	#include <Windows.h>
	#include <SOIL.h>
	#include <SDL.h>

	#undef main
	#define _CRT_SECURE_NO_WARNINGS
#else
	#include <SOIL/SOIL.h>
	#include <SDL2/SDL.h>
#endif

#include <GL/gl.h> // Библиотека OpenGL
#include <GL/glu.h> // Библиотека GLU
#include <GL/glext.h>

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#endif // _DEFINES_HPP_

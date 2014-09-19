
#include <string>
#include <streambuf>
#include <iostream> // стандартная библиотека ввода-вывода в C++
#include <fstream>

#include "../3rd/wavefront_loader/wavefront_loader.hpp"
#include "window.hpp"

int main(int argc, char *argv[])
{
	SdlWindow sdl_window( 640, 480 );
	sdl_window.Init( "../res/res.xml" ); // инициализация

	sdl_window.GameLoop();

	return 0;
}

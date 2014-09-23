#include "engine/sdl_engine.hpp"

int main(int argc, char *argv[])
{
	if( argc < 2 )
	{
		std::cout << "please provide res.xml file: ./build/sdl_opengl res/res.xml" << std::endl;
		return -1;
	}

	SdlEngine sdl_engine;

	int error_code = sdl_engine.Init(640, 480, argv[ 1 ]);
	if (error_code < 0)
		return error_code;

	sdl_engine.GameLoop();

	return 0;
}

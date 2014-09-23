#include "engine/sdl_engine.hpp"

int main(int argc, char *argv[])
{
	if( argc < 2 )
	{
		std::cout << "please provide res.xml file: ./build/sdl_opengl res/res.xml" << std::endl;
		return -1;
	}

	SdlEngine sdl_engine;

	try{
		sdl_engine.Init(640, 480, argv[1]);
		sdl_engine.GameLoop();
	}
	catch (std::exception & exception)
	{
		std::cout << exception.what() << std::endl;
	}

	return 0;
}

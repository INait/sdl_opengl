#include "sdl_engine.hpp"

int main(int argc, char *argv[])
{
	SdlEngine sdl_engine;

	sdl_engine.Init(640, 480, "../res/res.xml");

	sdl_engine.GameLoop();

	return 0;
}
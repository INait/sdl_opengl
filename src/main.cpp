#include "sdl_engine.hpp"

int main(int argc, char *argv[])
{
	SdlEngine sdl_engine;

	int error_code = sdl_engine.Init(640, 480, "../res/res.xml");
	if (error_code < 0)
		return error_code;

	sdl_engine.GameLoop();

	return 0;
}

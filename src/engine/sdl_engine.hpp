#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "defines.hpp"
#include "gfx/model.hpp"

class ResourceManager;
class ShaderProgram;

class SdlEngine
{
public:
	SdlEngine();
	~SdlEngine();

	void Init( int width, int height, const std::string & res_location);

	void GameLoop();

private:

	// Auxiliary struct for correct SDL window deletion
	struct SdlDeleter
	{
		void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }
	};

	std::unique_ptr< SDL_Window, SdlDeleter >		window;
	std::shared_ptr< ResourceManager >				resource_manager_;

	int width_;
	int height_;
};

#endif // _WINDOW_HPP_

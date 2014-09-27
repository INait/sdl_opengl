#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "defines.hpp"
#include "gfx/model.hpp"

class ResourceManager;
class ShaderProgram;

class SdlEngine
{
public:
	friend class Model;

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

	void CreateModel(const char* obj_file, const char* tex_file, const char* vsh_file, const char* fsh_file);
};

#endif // _WINDOW_HPP_

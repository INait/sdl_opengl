#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "defines.hpp"
#include "asset.hpp"

class ResourceLoader;
class ShaderProgram;

class SdlEngine
{
public:
	friend class Asset;

	SdlEngine();
	~SdlEngine();

	int Init( int width, int height, const std::string & res_location);

	void GameLoop();

	GLuint perspectiveMatrixID;
	GLfloat P[16];				// The perspective matrix for the camera (to give the scene depth); initialize this ONLY ONCE!

	void InitCamera();

private:
	SDL_Window *window;
	std::shared_ptr< ResourceLoader > resource_loader_;

	int width_;
	int height_;

	void CreateAsset(const char* obj_file, const char* tex_file, const char* vsh_file, const char* fsh_file);

	// array of game objects
	std::vector< AssetPtr > assets;

	std::vector< Vec4 > light;
};

#endif // _WINDOW_HPP_

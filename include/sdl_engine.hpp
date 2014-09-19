#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "defines.hpp"
#include "asset.hpp"

class SdlEngine
{
public:
	SdlEngine();
	~SdlEngine();

	void Init( int width, int height, const std::string & res_location);

	void GameLoop();

private:
	SDL_Window *window;

	int width_;
	int height_;

	GLuint shaderProgramID;

	std::string readShaderFile( const char* filename );
	GLuint makeVertexShader( const char* shaderSource );
	GLuint makeFragmentShader( const char* shaderSource );
	GLuint makeShaderProgram( GLuint vertexShaderID, GLuint fragmentShaderID );

	std::vector< AssetPtr > assets;
};

#endif // _WINDOW_HPP_

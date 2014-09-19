#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "../include/defines.hpp"
#include "../include/asset.hpp"

class SdlWindow
{
public:
	SdlWindow( int width, int height );
	~SdlWindow();

	void Init( const std::string & res_location );

	void GameLoop();

private:
	SDL_Window *window;

	int width;
	int height;

	GLuint shaderProgramID;

	std::string readShaderFile( const char* filename );
	GLuint makeVertexShader( const char* shaderSource );
	GLuint makeFragmentShader( const char* shaderSource );
	GLuint makeShaderProgram( GLuint vertexShaderID, GLuint fragmentShaderID );

	std::vector< AssetPtr > assets;
};

#endif // _WINDOW_HPP_

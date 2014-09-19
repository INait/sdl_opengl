#ifndef _ASSET_HPP_
#define _ASSET_HPP_

#include "../3rd/wavefront_loader/wavefront_loader.hpp"
#include "../include/defines.hpp"

class Asset
{
private:
	std::vector< Vec3 > vertices;
	std::vector< Vec2 > uvs;
	std::vector< Vec3 > normals;

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	GLuint texture_id;

	GLuint shaderProgramID;
public:

	Asset( const char * object_path, const char * texture_path );
	~Asset();

	void Draw(float xrf, float yrf, float zrf);

	void SetShaderProgramID( const GLuint shaderProgramID ) { this->shaderProgramID = shaderProgramID; }
};

typedef std::shared_ptr< Asset > AssetPtr;

#endif // _ASSET_HPP_

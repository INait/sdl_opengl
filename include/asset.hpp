#ifndef _ASSET_HPP_
#define _ASSET_HPP_

#include "defines.hpp"
#include "object.hpp"

class SdlEngine;

class Asset : public Object
{
private:
	std::vector< Vec3 > vertices;
	std::vector< Vec2 > uvs;
	std::vector< Vec3 > normals;
	std::vector< Vec4 > colors;

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	GLuint texture_id;

	SdlEngine* engine_;
public:

	Asset( const char * object_path, const char * texture_path );
	virtual ~Asset();

	virtual void Draw();

	void SetSdlEngine(SdlEngine* engine) { engine_ = engine; }
};

typedef std::shared_ptr< Asset > AssetPtr;

#endif // _ASSET_HPP_

#ifndef _ASSET_HPP_
#define _ASSET_HPP_

#include "defines.hpp"
#include "object.hpp"

class SdlEngine;
class ShaderProgram;

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

	std::shared_ptr< ShaderProgram >  shader_program_;

	GLuint viewMatrixID;
	GLuint modelMatrixID;
	GLuint allRotsMatrixID; //NEW

	GLfloat* rotXMatrix;	// Matrix for rotations about the X axis
	GLfloat* rotYMatrix;	// Matrix for rotations about the Y axis
	GLfloat* rotZMatrix;	// Matrix for rotations about the Z axis
	GLfloat* transMatrix;	// Matrix for changing the position of the object
	GLfloat* scaleMatrix;	// Duh..
	GLfloat* tempMatrix1;	// A temporary matrix for holding intermediate multiplications
	GLfloat* allRotsMatrix;

	GLfloat* M;				// The final model matrix M to change into world coordinates
	GLfloat* V;				// The camera matrix (for position/rotation) to change into camera coordinates

	GLfloat  theta;			// An amount of rotation along one axis
	GLfloat	 scaleAmount;	// In case the object is too big or small
public:

	Asset( const char * object_path, const char * texture_path,
		   const char * vsh_path, const char * fsh_path );

	virtual ~Asset();

	virtual void Draw();
	void InitMatrices();

	void SetSdlEngine(SdlEngine* engine);
};

typedef std::shared_ptr< Asset > AssetPtr;

#endif // _ASSET_HPP_

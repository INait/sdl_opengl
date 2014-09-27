#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "defines.hpp"
#include "gfx/object.hpp"

class SdlEngine;
class ShaderProgram;

class Model : public Object
{
private:
	std::vector< Vec3 > vertices;
	std::vector< Vec2 > uvs;
	std::vector< Vec3 > normals;
	std::vector< Vec4 > colors;

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	GLuint texture_id_;
	GLuint shader_program_id_;

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

	Model( const char * object_path );

	virtual ~Model();

	virtual void Draw();
	void InitMatrices();

	void ActivateShaderProgram(GLuint shader_program_id);

	void SetTextureID(GLuint texture_id) { texture_id_ = texture_id; }
};

typedef std::shared_ptr< Model > ModelPtr;

#endif // _MODEL_HPP_

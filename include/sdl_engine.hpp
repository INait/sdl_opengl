#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "defines.hpp"
#include "asset.hpp"

class SdlEngine
{
public:
	friend class Asset;

	SdlEngine();
	~SdlEngine();

	int Init( int width, int height, const std::string & res_location);

	void GameLoop();

private:
	SDL_Window *window;

	int width_;
	int height_;

	void InitMatrices();

	// array of game objects
	std::vector< AssetPtr > assets;

	GLuint perspectiveMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;
	GLuint	allRotsMatrixID; //NEW
	GLuint	lightID;		// NEW

	GLfloat* rotXMatrix;	// Matrix for rotations about the X axis
	GLfloat* rotYMatrix;	// Matrix for rotations about the Y axis
	GLfloat* rotZMatrix;	// Matrix for rotations about the Z axis
	GLfloat* transMatrix;	// Matrix for changing the position of the object
	GLfloat* scaleMatrix;	// Duh..
	GLfloat* tempMatrix1;	// A temporary matrix for holding intermediate multiplications
	GLfloat* allRotsMatrix;
	GLfloat* M;				// The final model matrix M to change into world coordinates

	GLfloat* V;				// The camera matrix (for position/rotation) to change into camera coordinates
	GLfloat* P;				// The perspective matrix for the camera (to give the scene depth); initialize this ONLY ONCE!

	GLfloat  theta;			// An amount of rotation along one axis
	GLfloat	 scaleAmount;	// In case the object is too big or small

	std::vector< Vec4 > light;
};

#endif // _WINDOW_HPP_

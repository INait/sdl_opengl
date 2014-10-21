#ifndef _BILLIARD_BALL_HPP_
#define _BILLIARD_BALL_HPP_

#include "defines.hpp"
#include "models/mesh.hpp"

class SdlEngine;
class ShaderProgram;

class BilliardBall : public Mesh
{
private:
	GLuint uniformMVP;
	GLuint uniformModelMatrix;
	GLuint uniformEyePosW;

	// Light properties.
	GLuint uniformLightPosW;
	GLuint uniformLightColor;

	// Global ambient.
	GLuint uniformAmbient;

	// Material properties.
	GLuint uniformMaterialEmissive;
	GLuint uniformMaterialDiffuse;
	GLuint uniformMaterialSpecular;
	GLuint uniformMaterialShininess;

	void GenerateSphere(float cx, float cy, float cz, float r, int p);
	void GenerateBackground();

	float rotation_;

public:

	BilliardBall();

	virtual ~BilliardBall();

	virtual void Draw();

	virtual void ApplyShaderProgram(GLuint shader_program_id);
};

#endif // _BILLIARD_BALL_HPP_

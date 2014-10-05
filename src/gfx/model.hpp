#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "defines.hpp"
#include "gfx/object.hpp"

class SdlEngine;
class ShaderProgram;

class Model : public Object
{
private:
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	GLuint vertex_array_id_;	// VAO

	GLuint vbos_[3];

	GLuint texture_id_;
	GLuint shader_program_id_;

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

	Model(const std::string& obj_path);

	virtual ~Model();

	virtual void Draw();

	void ActivateShaderProgram(GLuint shader_program_id);

	void ApplyTexture(GLuint texture_id);

	glm::vec4 light_pos_;
};

typedef std::shared_ptr< Model > ModelPtr;

#endif // _MODEL_HPP_

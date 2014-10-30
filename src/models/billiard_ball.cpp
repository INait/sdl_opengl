#include "models/billiard_ball.hpp"
#include "defines.hpp"
#include "engine/sdl_engine.hpp"
#include "gfx/shader_program.hpp"
#include "utils/resource_manager.hpp"
#include "gfx/renderer.hpp"
#include "gfx/camera.hpp"
#include "utils/geometry_primitives_generator.hpp"

BilliardBall::BilliardBall() :
	Mesh({ 0, 0, 0 }, { 0, 0, 0 })
{
	GeometryPrimitivesGen::Sphere(vertices, normals, uvs, 0, 0, 0, 1, 200);

	// loader for obj files	
	//	ObjWavefrontLoader(obj_path.c_str(), vertices, uvs, normals);

	SetupGlBuffers();
}

BilliardBall::~BilliardBall()
{
}

void BilliardBall::ApplyShaderProgram(GLuint shader_program_id)
{
	shader_program_id_ = shader_program_id;

	uniformMVP = glGetUniformLocation(shader_program_id_, "ModelViewProjectionMatrix");
	uniformModelMatrix = glGetUniformLocation(shader_program_id_, "ModelMatrix");
	uniformEyePosW = glGetUniformLocation(shader_program_id_, "EyePosW");

	// Light properties.
	uniformLightPosW = glGetUniformLocation(shader_program_id_, "LightPosW");
	uniformLightColor = glGetUniformLocation(shader_program_id_, "LightColor");

	// Global ambient.
	uniformAmbient = glGetUniformLocation(shader_program_id_, "Ambient");

	// Material properties.
	uniformMaterialEmissive = glGetUniformLocation(shader_program_id_, "MaterialEmissive");
	uniformMaterialDiffuse = glGetUniformLocation(shader_program_id_, "MaterialDiffuse");
	uniformMaterialSpecular = glGetUniformLocation(shader_program_id_, "MaterialSpecular");
	uniformMaterialShininess = glGetUniformLocation(shader_program_id_, "MaterialShininess");
}

void BilliardBall::Draw()
{
	glBindVertexArray(vertex_array_id_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, reflection_texture_id_);

	glUseProgram(shader_program_id_);

	const glm::vec4 white{ 1 };
	const glm::vec4 specular_light{ 1 };
	const glm::vec4 black{ 0 };
	const glm::vec4 ambient{ 0.5f, 0.5f, 0.5f, 1.0f };

	// Set the light position
	glUniform4fv(uniformLightPosW, 1, glm::value_ptr(Renderer::GetInstance().light_pos_));
	glUniform4fv(uniformLightColor, 1, glm::value_ptr(white));
	glUniform4fv(uniformAmbient, 1, glm::value_ptr(ambient));

	glm::mat4 model_matrix = glm::translate(glm::vec3(position_.x, position_.y, 0)) *
							 glm::rotate(orientation_.x, glm::vec3(1, 0, 0)) *
							 glm::rotate(orientation_.y, glm::vec3(0, 1, 0)) *
							 glm::scale(glm::vec3(position_.z));

	auto camera_ptr = Renderer::GetInstance().camera_ptr_;
	glm::vec4 eyePosW = glm::vec4(camera_ptr->GetPosition(), 1);
	glm::mat4 mvp = camera_ptr->GetProjectionMatrix() * camera_ptr->GetViewMatrix() * model_matrix;

	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform4fv(uniformEyePosW, 1, glm::value_ptr(eyePosW));

	// Material properties.
	glUniform4fv(uniformMaterialEmissive, 1, glm::value_ptr(black));
	glUniform4fv(uniformMaterialDiffuse, 1, glm::value_ptr(white));
	glUniform4fv(uniformMaterialSpecular, 1, glm::value_ptr(specular_light));
	glUniform1f(uniformMaterialShininess, 50.0f);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}

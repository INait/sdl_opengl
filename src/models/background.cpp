#include "models/background.hpp"
#include "gfx/renderer.hpp"
#include "gfx/camera.hpp"
#include "utils/geometry_primitives_generator.hpp"

Background::Background() :
	Mesh({ 0, 0, 0 }, { 0, 0, 0 })
{
	GeometryPrimitivesGen::Quad(vertices, normals, uvs,
		glm::vec3{ -1.33, -1.0, 0.0 }, glm::vec3{ 1.33, -1.0, 0.0 }, glm::vec3{ -1.33, 1.0, 0.0 }, glm::vec3{ 1.33, 1.0, 0.0 });

	SetupGlBuffers();
}

Background::~Background()
{
}

void Background::ApplyShaderProgram(GLuint shader_program_id)
{
	shader_program_id_ = shader_program_id;

	uniformMVP = glGetUniformLocation(shader_program_id_, "ModelViewProjectionMatrix");
}

void Background::Draw()
{
	glBindVertexArray(vertex_array_id_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glUseProgram(shader_program_id_);

	auto camera_ptr = Renderer::GetInstance().camera_ptr_;
	glm::vec4 eyePosW = glm::vec4(camera_ptr->GetPosition(), 1);
	glm::mat4 mvp = camera_ptr->GetProjectionMatrix() * camera_ptr->GetViewMatrix() * glm::scale(glm::vec3(30));

	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}
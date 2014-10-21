#include "models/background.hpp"
#include "gfx/renderer.hpp"
#include "gfx/camera.hpp"

Background::Background() :
	Mesh({ 0, 0, 0 }, { 0, 0, 0 })
{
	GenerateBackground();

	glGenVertexArrays(1, &vertex_array_id_);
	glBindVertexArray(vertex_array_id_);

	glGenBuffers(3, vbos_);
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(POSITION_ATTRIBUTE);

	// Give normals
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

	// Give uvs
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(TEXCOORD0_ATTRIBUTE);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Background::~Background()
{
}

void Background::ApplyShaderProgram(GLuint shader_program_id)
{
	shader_program_id_ = shader_program_id;

	uniformMVP = glGetUniformLocation(shader_program_id_, "ModelViewProjectionMatrix");
}

void Background::GenerateBackground()
{
	vertices.emplace_back(glm::vec3{ -1.33, -1.0, 0.0 });
	vertices.emplace_back(glm::vec3{ 1.33, -1.0, 0.0 });
	vertices.emplace_back(glm::vec3{ -1.33, 1.0, 0.0 });
	vertices.emplace_back(glm::vec3{ 1.33, 1.0, 0.0 });

	normals.emplace_back(glm::vec3{ -1.33, -1.0, 0.0 });
	normals.emplace_back(glm::vec3{ 1.33, -1.0, 0.0 });
	normals.emplace_back(glm::vec3{ -1.33, 1.0, 0.0 });
	normals.emplace_back(glm::vec3{ 1.33, 1.0, 0.0 });

	uvs.emplace_back(glm::vec2{ 0.0, 0.0 });
	uvs.emplace_back(glm::vec2{ 1.0, 0.0 });
	uvs.emplace_back(glm::vec2{ 0.0, 1.0 });
	uvs.emplace_back(glm::vec2{ 1.0, 1.0 });
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
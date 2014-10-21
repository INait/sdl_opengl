#include "models/background.hpp"

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

void Background::GenerateBackground()
{
	vertices.emplace_back(glm::vec3{ -1.0, -1.0, 0.0 });
	vertices.emplace_back(glm::vec3{ 1.0, -1.0, 0.0 });
	vertices.emplace_back(glm::vec3{ -1.0, 1.0, 0.0 });
	vertices.emplace_back(glm::vec3{ 1.0, 1.0, 0.0 });

	normals.emplace_back(glm::vec3{ -1.0, -1.0, 0.0 });
	normals.emplace_back(glm::vec3{ 1.0, -1.0, 0.0 });
	normals.emplace_back(glm::vec3{ -1.0, 1.0, 0.0 });
	normals.emplace_back(glm::vec3{ 1.0, 1.0, 0.0 });

	uvs.emplace_back(glm::vec2{ 0.0, 0.0 });
	uvs.emplace_back(glm::vec2{ 1.0, 0.0 });
	uvs.emplace_back(glm::vec2{ 0.0, 1.0 });
	uvs.emplace_back(glm::vec2{ 1.0, 1.0 });
}

void Background::Draw()
{
	return;

	glBindVertexArray(vertex_array_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glUseProgram(shader_program_id_);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}
#include "models/background.hpp"

Background::Background() :
	Mesh({ 0, 0, 0 }, { 0, 0, 0 })
{
	GenerateBackground();
}

Background::~Background()
{
}

void Background::GenerateBackground()
{
	vertices.emplace_back(glm::vec3{ 0.0, 0.0, -1.0 });
	vertices.emplace_back(glm::vec3{ 1.0, 0.0, -1.0 });
	vertices.emplace_back(glm::vec3{ 0.0, 1.0, -1.0 });
	vertices.emplace_back(glm::vec3{ 1.0, 1.0, -1.0 });

	normals.emplace_back(glm::vec3{ 0.0, 0.0, -1.0 });
	normals.emplace_back(glm::vec3{ 1.0, 0.0, -1.0 });
	normals.emplace_back(glm::vec3{ 0.0, 1.0, -1.0 });
	normals.emplace_back(glm::vec3{ 1.0, 1.0, -1.0 });

	uvs.emplace_back(glm::vec2{ 0.0, 0.0 });
	uvs.emplace_back(glm::vec2{ 1.0, 0.0 });
	uvs.emplace_back(glm::vec2{ 0.0, 1.0 });
	uvs.emplace_back(glm::vec2{ 1.0, 1.0 });
}

void Background::Draw()
{

}
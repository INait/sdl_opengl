#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "defines.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define POSITION_ATTRIBUTE 0
#define NORMAL_ATTRIBUTE 2
#define DIFFUSE_ATTRIBUTE 3
#define SPECULAR_ATTRIBUTE 4
#define TEXCOORD0_ATTRIBUTE 8
#define TEXCOORD1_ATTRIBUTE 9
#define TEXCOORD2_ATTRIBUTE 10

class Mesh
{
public:
	Mesh(const glm::vec3 & position, const glm::vec3 & orientation) : position_(position), orientation_(orientation) { }
	virtual ~Mesh() {}

	virtual void SetPosition(const glm::vec3 & pos) { position_ = pos; }
	virtual void SetOrientation(const glm::vec3 & orient) { orientation_ = orient; }

	virtual void ApplyTexture(GLuint texture_id) { texture_id_ = texture_id; }
	virtual void ApplyReflTexture(GLuint texture_id) { reflection_texture_id_ = texture_id; }
	virtual void ApplyShaderProgram(GLuint shader_program_id) { shader_program_id_ = shader_program_id; }
	
	virtual void Draw() = 0;

protected:
	glm::vec3 position_;
	glm::vec3 orientation_;

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	GLuint vertex_array_id_;

	GLuint vbos_[3];

	GLuint texture_id_;
	GLuint reflection_texture_id_;
	GLuint shader_program_id_;
};

typedef std::shared_ptr< Mesh > MeshPtr;

#endif // _OBJECT_HPP_

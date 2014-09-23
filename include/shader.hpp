#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <string>
#include "defines.hpp"

class Shader
{
public:

	Shader(const char * filename, int shader_type);
	~Shader();

	GLuint shaderProgramID;

	std::string ReadShaderFile(const char* filename);
	GLuint MakeShader();
	void ShaderLog();

	int GetID() const { return shader_id_; }
	int GetType() const { return shader_type_; }

private:

	// identifier of created gl shader
	int			shader_id_;

	// type of shader
	// GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	int			shader_type_;

	std::string	shader_source_;
};

typedef std::shared_ptr< Shader > ShaderPtr;

#endif // _SHADER_HPP_

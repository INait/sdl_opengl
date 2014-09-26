#ifndef _SHADER_PROGRAM_HPP_
#define _SHADER_PROGRAM_HPP_

#include "defines.hpp"
#include "gfx/shader.hpp"

class ShaderProgram
{
private:
	std::vector< ShaderPtr > shaders_;

	GLuint shader_program_id_;

	void MakeProgram();

public:
	ShaderProgram( const char* vertex_shader_path, const char* fragment_shader_path );
	~ShaderProgram();

	GLuint GetID() { return shader_program_id_; }
};

typedef std::make_shared< ShaderProgram > ShaderProgramPtr;

#endif // _SHADER_PROGRAM_HPP_

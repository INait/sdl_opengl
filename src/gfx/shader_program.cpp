#include "gfx/shader_program.hpp"

ShaderProgram::ShaderProgram( const char* vertex_shader_path, const char* fragment_shader_path )
{
	ShaderPtr vertex_shader = std::make_shared< Shader >( vertex_shader_path, GL_VERTEX_SHADER );
	ShaderPtr fragment_shader = std::make_shared< Shader >( fragment_shader_path, GL_FRAGMENT_SHADER );

	shaders_.push_back( vertex_shader );
	shaders_.push_back( fragment_shader );

	MakeProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram( shader_program_id_ );
}

void ShaderProgram::MakeProgram()
{
	shader_program_id_ = glCreateProgram();

	for( auto & shader : shaders_ )
	{
		shader->MakeShader();
		glAttachShader( shader_program_id_, shader->GetID() );
	}

	glLinkProgram( shader_program_id_ );

	int link_ok;
	glGetProgramiv(shader_program_id_, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
		throw std::runtime_error("Cannot attach shader");
}

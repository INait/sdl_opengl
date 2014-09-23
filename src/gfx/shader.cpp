#include "gfx/shader.hpp"

#include <fstream>


Shader::Shader(const char * filename, int shader_type) : 
	shader_type_(shader_type)
{
	shader_source_ = ReadShaderFile(filename);
}

Shader::~Shader()
{
}

// FIXME: rewrite to use C++ vector instead of char*
void Shader::ShaderLog()
{
	int   infologLen = 0;
	int   charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 1)
	{
		infoLog = new char[infologLen];
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate InfoLog buffer\n";
			return;
		}
		glGetShaderInfoLog(shader_id_, infologLen, &charsWritten, infoLog);
		std::cout << "InfoLog: " << infoLog << "\n\n\n";
		delete[] infoLog;
	}
}

std::string Shader::ReadShaderFile(const char* filename)
{
	std::ifstream t(filename, std::ios::in);

	if (!t.is_open())
		throw std::runtime_error(std::string("Cannot open shaders file") + filename);

	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve((size_t)t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return str;
}

GLuint Shader::MakeShader()
{
	shader_id_ = glCreateShader(shader_type_);
	const char* c_str = shader_source_.c_str();
	glShaderSource(shader_id_, 1, (const GLchar**)& c_str, NULL);
	glCompileShader(shader_id_);
	ShaderLog();
	return shader_id_;
}

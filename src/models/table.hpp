#ifndef _TABLE_HPP_
#define _TABLE_HPP_

#include "defines.hpp"
#include "models/mesh.hpp"

class Table : public Mesh
{
public:
	Table();
	~Table();

	virtual void Draw();

	virtual void ApplyShaderProgram(GLuint shader_program_id);
private:

	GLuint uniformMVP;
};

#endif // _TABLE_HPP_
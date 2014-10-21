#ifndef _BACKGROUND_HPP_
#define _BACKGROUND_HPP_

#include "defines.hpp"
#include "models/mesh.hpp"

class Background : public Mesh
{
public:
	Background();
	~Background();

	virtual void Draw();

	virtual void ApplyShaderProgram(GLuint shader_program_id);
private:

	void GenerateBackground();

	GLuint uniformMVP;
};

#endif // _BACKGROUND_HPP_
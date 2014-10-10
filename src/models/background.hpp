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

private:

	void GenerateBackground();
};

#endif // _BACKGROUND_HPP_
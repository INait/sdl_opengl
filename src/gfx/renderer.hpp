#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "utils/singleton.hpp"
#include <memory>

class Camera;

class Renderer : public ISingletonable< Renderer >
{
public:
	Renderer(int width, int height);
	~Renderer();

	void Draw();

	std::shared_ptr< Camera > camera_ptr_;
private:
	

};

#endif // _RENDERER_HPP_

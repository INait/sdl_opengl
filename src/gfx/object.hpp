#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "defines.hpp"

class Object
{
public:
	Object(const glm::vec3 & position, const glm::vec3 & orientation) : position_(position), orientation_(orientation) { }

	virtual void SetPosition(const glm::vec3 & pos) { position_ = pos; }
	virtual void SetOrientation(const glm::vec3 & orient) { orientation_ = orient; }

	virtual void Draw() = 0;

	virtual ~Object() {}
protected:
	glm::vec3 position_;
	glm::vec3 orientation_;
};

#endif // _OBJECT_HPP_

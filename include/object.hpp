#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "defines.hpp"

class Object
{
public:
	Object(const Vec3 & position, const Vec3 & orientation) : position_(position), orientation_(orientation) { }

	virtual void SetPosition(const Vec3 & pos) { position_ = pos; }
	virtual void SetOrientation(const Vec3 & orient) { orientation_ = orient; }

	virtual void Draw() = 0;

	virtual ~Object() {}
protected:
	Vec3 position_;
	Vec3 orientation_;
};

#endif // _OBJECT_HPP_
#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "defines.hpp"
#include <memory>

class Texture
{
public:
	Texture(const char* texture_path);
	~Texture();

	GLuint GetID() { return texture_id_; }
private:

	GLuint texture_id_;
};

typedef std::shared_ptr< Texture > TexturePtr;

#endif // _TEXTURE_HPP_

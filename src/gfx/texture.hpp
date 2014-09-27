#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <memory>

class Texture
{
public:
	Texture();
	~Texture();

	int GetID() { return texture_id_; }
private:

	int texture_id_;
};

typedef std::shared_ptr< Texture > TexturePtr;

#endif // _TEXTURE_HPP_

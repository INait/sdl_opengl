#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <memory>

class Texture
{
public:
	Texture();
	~Texture();

private:

	int texture_id_;
};

typedef std::shared_ptr< Texture > TexturePtr;

#endif // _TEXTURE_HPP_

#ifndef _RESOURCE_LOADER_HPP_
#define _RESOURCE_LOADER_HPP_

#include "../3rd/tinyxml/tinyxml.h"
#include "../3rd/wavefront_loader/wavefront_loader.hpp"

#include <vector>
#include <string>

class ResourceLoader
{
public:
	ResourceLoader();
	~ResourceLoader();

	void LoadXMLResources( const std::string & res_location );

private:

};

#endif // _RESOURCE_LOADER_HPP_

#include <iostream>
#include <stdexcept>

#include "../include/resource_loader.hpp"

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::LoadXMLResources( const std::string & res_location )
{
	TiXmlDocument doc( res_location.c_str() );
	if (!doc.LoadFile())
	{
		std::cout << "xml wasn't loaded" << std::endl;
		return;
	}

	TiXmlNode* pResElem = doc.FirstChild("resources");
	if (!pResElem)
	{
		std::cout << "wrong XML format: no 'resources' node" << std::endl;
		return;
	}

	// check if set param exists
	TiXmlElement* pObjElem = pResElem->FirstChildElement("objects");
	std::string resPrefix = (pObjElem && pObjElem->Attribute("path")) ? pObjElem->Attribute("path") : "";

	if (pObjElem)
	{
		try {
			for (TiXmlElement* pObjectElem = pObjElem->FirstChildElement("obj"); pObjectElem; pObjectElem = pObjectElem->NextSiblingElement("obj"))
			{
				std::string name = pObjectElem->Attribute("name");
				std::string file = resPrefix + pObjectElem->Attribute("file");

				// load asset here
			}
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
	}

	TiXmlElement* pTexturesElem = pResElem->FirstChildElement( "textures" );
	std::string texPrefix = ( pTexturesElem && pTexturesElem->Attribute( "path") ) ? pTexturesElem->Attribute( "path" ) : "";

	if (pTexturesElem)
	{
		try {
			for (TiXmlElement* pTextureElem = pTexturesElem->FirstChildElement("obj"); pTextureElem; pTextureElem = pTextureElem->NextSiblingElement("obj"))
			{
				std::string name = pTextureElem->Attribute("name");
				std::string file = texPrefix + pTextureElem->Attribute("file");

				// load texture here
			}
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
	}
}


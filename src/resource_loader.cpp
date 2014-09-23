#include <iostream>
#include <stdexcept>

#include "resource_loader.hpp"

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
	TiXmlElement* pObjElem = pResElem->FirstChildElement("object");
	std::string resPrefix = (pObjElem && pObjElem->Attribute("path")) ? pObjElem->Attribute("path") : "";

	if (pObjElem)
	{
		try {
			for (TiXmlElement* pObjectElem = pObjElem->FirstChildElement("obj"), * pTextureElem = pObjElem->FirstChildElement( "texture" );
				 pObjectElem && pTextureElem;
				 pObjectElem = pObjectElem->NextSiblingElement("obj"),
				 pTextureElem = pTextureElem->NextSiblingElement("texture"))
			{
				std::string obj_name = pObjectElem->Attribute("name");
				std::string obj_file = resPrefix + pObjectElem->Attribute("file");

				std::string tex_name = pTextureElem->Attribute("name");
				std::string tex_file = resPrefix + pTextureElem->Attribute("file");

				new_asset_function_( obj_file.c_str(), tex_file.c_str() );
			}
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
	}
}


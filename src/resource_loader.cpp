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

	for( TiXmlElement* pObjElem = pResElem->FirstChildElement("object"); pObjElem; pObjElem = pObjElem->NextSiblingElement("object") )
	{
		std::string resPrefix = (pObjElem && pObjElem->Attribute("path")) ? pObjElem->Attribute("path") : "";
		try {
			std::string obj_file = resPrefix + pObjElem->FirstChildElement("obj")->Attribute("file");
			std::string tex_file = resPrefix + pObjElem->FirstChildElement("texture")->Attribute("file");
			std::string vsh_file = resPrefix + pObjElem->FirstChildElement("vsh")->Attribute("file");
			std::string fsh_file = resPrefix + pObjElem->FirstChildElement("fsh")->Attribute("file");

			// create new asset with retrieved information
			new_asset_function_( obj_file.c_str(), tex_file.c_str(), vsh_file.c_str(), fsh_file.c_str() );
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
	}
}


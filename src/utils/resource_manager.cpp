#include "utils/resource_manager.hpp"
#include "gfx/model.hpp"
#include "gfx/texture.hpp"
#include "gfx/shader_program.hpp"

#include "../3rd/tinyxml/tinyxml.h"

DECLARE_SINGLETON_VAR( ResourceManager )

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::FreeResources()
{
	textures_.clear();
	shader_programs_.clear();
	models_.clear();
}

void ResourceManager::LoadXMLResources(const std::string & res_location)
{
	TiXmlDocument doc(res_location.c_str());
	if (!doc.LoadFile())
		throw std::runtime_error("Cannot load XML resource file");

	TiXmlNode* pResElem = doc.FirstChild("resources");
	if (!pResElem)
		throw std::runtime_error("Wrong XML resource format");

	// Fill models
	TiXmlElement* pObjectsElem = pResElem->FirstChildElement("objects");
	std::string prefix = pObjectsElem->Attribute("path");
	for (TiXmlElement* pModelElem = pObjectsElem->FirstChildElement("model"); pModelElem; pModelElem = pModelElem->NextSiblingElement("model"))
	{
		std::string model_path = prefix + pModelElem->Attribute("file");
		std::string model_name = pModelElem->Attribute("name");

		ModelPtr model_ptr = std::make_shared< Model >(model_path);
		models_.emplace(model_name, model_ptr);
	}

	// Fill textures
	TiXmlElement* pTexturesElem = pResElem->FirstChildElement("textures");
	prefix = pTexturesElem->Attribute("path");
	for (TiXmlElement* pTextureElem = pTexturesElem->FirstChildElement("texture"); pTextureElem; pTextureElem = pTextureElem->NextSiblingElement("texture"))
	{
		std::string texture_path = prefix + pTextureElem->Attribute("file");
		std::string texture_name = pTextureElem->Attribute("name");

		TexturePtr texture_ptr = std::make_shared< Texture >(texture_path.c_str());
		textures_.emplace(texture_name, texture_ptr);
	}

	// Fill shaders
	TiXmlElement* pShadersElem = pResElem->FirstChildElement("shaders");
	prefix = pShadersElem->Attribute("path");
	for (TiXmlElement* pShaderElem = pShadersElem->FirstChildElement("shader"); pShaderElem; pShaderElem = pShaderElem->NextSiblingElement("shader"))
	{
		std::string vsh_file = prefix + pShaderElem->Attribute("vsh_file");
		std::string fsh_file = prefix + pShaderElem->Attribute("fsh_file");
		std::string shader_name = pShaderElem->Attribute("name");

		ShaderProgramPtr shader_program_ptr = std::make_shared< ShaderProgram >(vsh_file.c_str(), fsh_file.c_str());
		shader_programs_.emplace(shader_name, shader_program_ptr);
	}

	// Fill Model - Texture - Shader mapping
	TiXmlElement* pMTSObjElem = pResElem->FirstChildElement("mts_objects");
	for (TiXmlElement* pMTSElem = pMTSObjElem->FirstChildElement("mts"); pMTSElem; pMTSElem = pMTSElem->NextSiblingElement("mts"))
	{
		MTS_mapping mts;
		mts.m = pMTSElem->Attribute("model");
		mts.t = pMTSElem->Attribute("texture");
		mts.s = pMTSElem->Attribute("shader");

		mts_mapping_.emplace_back(mts);

		auto model_it = models_.find(mts.m);

		model_it->second->ActivateShaderProgram(shader_programs_.find(mts.s)->second->GetID());
		model_it->second->ApplyTexture(textures_.find(mts.t)->second->GetID());
	}
}
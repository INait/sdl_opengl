#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <map>
#include <string>

class Model;
class Texture;
class ShaderProgram;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void LoadXMLResources(const std::string & res_location);

	struct MTS_mapping
	{
		std::string m;  // model_id
		std::string t;  // texture_id
		std::string s;  // shader_program_id
	};

private:

	std::map< std::string /* model_id */, Model >				models_;
	std::map< std::string /* texture_id */, Texture >			textures_;
	std::map< std::string /* shader_id */, ShaderProgram >		shader_programs_;

	std::vector< MTS_mapping >							mts_mapping_;
};

#endif // _RESOURCE_MANAGER_HPP_

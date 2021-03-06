#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <map>
#include <string>
#include <vector>

#include "utils/singleton.hpp"

#include "models/mesh.hpp"
#include "gfx/texture.hpp"
#include "gfx/shader_program.hpp"

class ResourceManager : public ISingletonable< ResourceManager >
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

	void FreeResources();

	std::map< std::string /* model_id */, MeshPtr >					models_;

private:
	std::map< std::string /* texture_id */, TexturePtr >			textures_;
	std::map< std::string /* shader_id */, ShaderProgramPtr >		shader_programs_;

	std::vector< MTS_mapping >										mts_mapping_;
};

#endif // _RESOURCE_MANAGER_HPP_

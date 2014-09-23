#ifndef _RESOURCE_LOADER_HPP_
#define _RESOURCE_LOADER_HPP_

#include <vector>
#include <string>
#include <functional>

class ResourceLoader
{
public:

	typedef std::function< void( const char* obj_file, const char* texture_file, const char* vsh_path, const char* fsh_path ) > NewAssetFunction;

	ResourceLoader();
	~ResourceLoader();

	void LoadXMLResources( const std::string & res_location );

	NewAssetFunction new_asset_function_;

	void SetNewAssetFuction( NewAssetFunction new_asset_function ) { new_asset_function_ = new_asset_function; }
};

#endif // _RESOURCE_LOADER_HPP_
#ifndef WAVEFRONT_LOADER_HPP_
#define WAVEFRONT_LOADER_HPP_

#include <vector>
#include "defines.hpp"

void ObjWavefrontLoader(
		const char* path,
		std::vector< glm::vec3 > & vertices,
		std::vector< glm::vec2 > & uvs,
		std::vector< glm::vec3 > & normals
	);

#endif /* WAVEFRONT_LOADER_HPP_ */

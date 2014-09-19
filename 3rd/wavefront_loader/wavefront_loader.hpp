#ifndef WAVEFRONT_LOADER_HPP_
#define WAVEFRONT_LOADER_HPP_

#include <vector>

struct Vec2
{
	float x;
	float y;
};

struct Vec3
{
	float x;
	float y;
	float z;
};

bool ObjWavefrontLoader(
		const char* path,
		std::vector< Vec3 > & vertices,
		std::vector< Vec2 > & uvs,
		std::vector< Vec3 > & normals
	);

#endif /* WAVEFRONT_LOADER_HPP_ */

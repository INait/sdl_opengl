#ifndef _GEOMETRY_PRIMITIVES_GENERATOR_HPP_
#define _GEOMETRY_PRIMITIVES_GENERATOR_HPP_

#include "defines.hpp"

class GeometryPrimitivesGen
{
public:
	static void Sphere(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::vector< glm::vec2 >& uvs,
		float cx, float cy, float cz, float r, int p)
	{
		const float TWOPI = 6.28318530717958f;

		float theta1 = 0.0;
		float theta2 = 0.0;
		float phi1 = 0.0;

		float ex = 0.0f;
		float ey = 0.0f;
		float ez = 0.0f;

		float px = 0.0f;
		float py = 0.0f;
		float pz = 0.0f;

		for (int i = 0; i < p / 2; ++i)
		{
			theta1 = i * TWOPI / p;
			theta2 = (i + 1) * TWOPI / p;

			for (int j = 0; j <= p; ++j)
			{
				phi1 = j * TWOPI / p;

				// first triangle
				ex = sinf(theta1) * sinf(phi1);
				ez = sinf(theta1) * cosf(phi1);
				ey = cosf(theta1);
				px = cx + r * ex;
				py = cy + r * ey;
				pz = cz + r * ez;

				vertices.emplace_back(glm::vec3{ px, py, pz });
				normals.emplace_back(glm::vec3{ ex, ey, ez });
				uvs.push_back(glm::vec2{ -(j / (float)p), 2 * i / (float)p });

				ex = sinf(theta2) * sinf(phi1);
				ez = sinf(theta2) * cosf(phi1);
				ey = cosf(theta2);
				px = cx + r * ex;
				py = cy + r * ey;
				pz = cz + r * ez;

				vertices.emplace_back(glm::vec3{ px, py, pz });
				normals.emplace_back(glm::vec3{ ex, ey, ez });
				uvs.push_back(glm::vec2{ -(j / (float)p), 2 * (i + 1) / (float)p });
			}
		}
	}

	static void Quad(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::vector< glm::vec2 >& uvs,
		const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3, const glm::vec3 & p4)
	{
		vertices.emplace_back(p1);
		vertices.emplace_back(p2);
		vertices.emplace_back(p3);
		vertices.emplace_back(p4);

		normals.emplace_back(p1);
		normals.emplace_back(p2);
		normals.emplace_back(p3);
		normals.emplace_back(p4);

		uvs.emplace_back(glm::vec2{ 0.0, 0.0 });
		uvs.emplace_back(glm::vec2{ 1.0, 0.0 });
		uvs.emplace_back(glm::vec2{ 0.0, 1.0 });
		uvs.emplace_back(glm::vec2{ 1.0, 1.0 });
	}
};

#endif // _GEOMETRY_PRIMITIVES_GENERATOR_HPP_
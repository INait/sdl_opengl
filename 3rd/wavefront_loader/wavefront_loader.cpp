#include <vector>
#include <iostream>
#include <iterator>
#include <cstdio>
#include <cstring>

#include "wavefront_loader.hpp"

struct Face
{
	std::vector< int > v;
	std::vector< int > vt;
	std::vector< int > vn;
};

bool ObjWavefrontLoader(
		const char* path,
		std::vector< Vec3 > & vertices,
		std::vector< Vec2 > & uvs,
		std::vector< Vec3 > & normals
	)
{
	FILE* file = fopen( path, "r" );

	if( file == NULL )
	{
		std::cout << "wrong file" << std::endl;
		return false;
	}

	std::vector< Vec3 > v;
	std::vector< Vec2 > vt;
	std::vector< Vec3 > vn;
	std::vector< Face > f;

	while( 1 )
	{
		char lineHeader[ 128 ];
		int res = fscanf( file, "%s", lineHeader );
		if( res == EOF )
			break; // end of file

		if( strcmp( lineHeader, "v" ) == 0 )
		{
			Vec3 vertex;
			fscanf( file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			v.push_back( vertex );
		}
		else if( strcmp( lineHeader, "vt" ) == 0 )
		{
			Vec2 uv;
			fscanf( file, "%f %f\n", &uv.x, &uv.y );
			vt.push_back( uv );
		}
		else if( strcmp( lineHeader, "vn" ) == 0 )
		{
			Vec3 normal;
			fscanf( file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			vn.push_back( normal );
		}
		else if( strcmp( lineHeader, "f" ) == 0 )
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[ 3 ], uvIndex[ 3 ], normalIndex[ 3 ];
			int matches = fscanf( file, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if( matches != 9 )
			{
				std::cout << "faces parsing error" << std::endl;
				return false;
			}

			Face face;
			std::copy( &vertexIndex[ 0 ], &vertexIndex[ 3 ], std::back_inserter( face.v ) );
			std::copy( &uvIndex[ 0 ], &uvIndex[ 3 ], std::back_inserter( face.vt ) );
			std::copy( &normalIndex[ 0 ], &normalIndex[ 3 ], std::back_inserter( face.vn ) );
			f.push_back( face );
		}
	}

	fclose( file );

	for( std::vector< Face >::const_iterator face_it = f.begin(); face_it != f.end(); ++face_it )
	{
		for( std::vector< int >::const_iterator v_it = face_it->v.begin(); v_it != face_it->v.end(); ++v_it )
		{
			Vec3 vertex = v[ *v_it - 1 ];
			vertices.push_back( vertex );
		}
		for( std::vector< int >::const_iterator vt_it = face_it->vt.begin(); vt_it != face_it->vt.end(); ++vt_it )
		{
			Vec2 uv = vt[ *vt_it - 1 ];
			uvs.push_back( uv );
		}
		for( std::vector< int >::const_iterator vn_it = face_it->vn.begin(); vn_it != face_it->vn.end(); ++vn_it )
		{
			Vec3 normal = vn[ *vn_it - 1 ];
			normals.push_back( normal );
		}
	}

	return true;
}


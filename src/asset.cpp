#include "asset.hpp"
#include "defines.hpp"
#include "../3rd/wavefront_loader/wavefront_loader.hpp"

Asset::Asset( const char * obj_path, const char * texture_path )
{
	if( ! ObjWavefrontLoader( obj_path, vertices, uvs, normals ) )
		std::cout << "cannot load asset" << std::endl;

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertices.size(), &vertices[ 0 ], GL_STATIC_DRAW);

	/* load an image file directly as a new OpenGL texture */
	texture_id = SOIL_load_OGL_texture
		(
		 texture_path,
		 SOIL_LOAD_AUTO,
		 SOIL_CREATE_NEW_ID,
		 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	int img_width, img_height;
	unsigned char* img = SOIL_load_image(texture_path, &img_width, &img_height, NULL, 0);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);
}

Asset::~Asset()
{
}

void Asset::Draw(float xrf, float yrf, float zrf)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef( 0.0f, 0.0f, -6.0f );
	glRotatef(xrf, 1.0f, 0.0f, 0.0f);
	glRotatef(yrf, 0.0f, 1.0f, 0.0f);
	glRotatef(zrf, 0.0f, 0.0f, 1.0f);

	GLuint positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	glVertexAttribPointer( positionID, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glUseProgram( shaderProgramID );
	glEnableVertexAttribArray( positionID );

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);
}

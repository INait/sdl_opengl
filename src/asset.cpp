#include "asset.hpp"
#include "defines.hpp"
#include "matrix_math.hpp"
#include "sdl_engine.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Asset::Asset(const char * obj_path, const char * texture_path) : Object({ 0, 0, 0 }, { 0, 0, 0 })
{
	if (!ObjWavefrontLoader(obj_path, vertices, uvs, normals))
		std::cout << "cannot load asset" << std::endl;

	/*
	vertices.push_back({ -0.5f, -0.5f, 0.0f });
	vertices.push_back({ 0.5f, -0.5f, 0.0f });
	vertices.push_back({ 0.0f, 0.5f, 0.0f });
	for (int i = 0; i < vertices.size() / 3; i++)
	{
	colors.push_back({ 1.0f, 0.0f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 1.0f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });
	}

	vertices.push_back({ -0.5f, -0.5f, 0.0f });
	vertices.push_back({ 0.0f, -0.5f, 0.0f });
	vertices.push_back({ -0.25f, 0.0f, 0.0f });
	vertices.push_back({ -0.25f, 0.0f, 0.0f });
	vertices.push_back({ 0.25f, 0.0f, 0.0f });
	vertices.push_back({ 0.0f, 0.5f, 0.0f });
	vertices.push_back({ 0.0f, -0.5f, 0.0f });
	vertices.push_back({ 0.5f, -0.5f, 0.0f });
	vertices.push_back({ 0.25f, 0.0f, 0.0f });

	colors.push_back({ 1.0f, 0.00f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 1.00f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });
	colors.push_back({ 1.0f, 0.00f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 1.00f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });
	colors.push_back({ 1.0f, 0.00f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 1.00f, 0.0f, 1.0f });
	colors.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });
	*/

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, ( sizeof(Vec3) + sizeof(Vec3) ) * vertices.size(), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vec3), &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), normals.size() * sizeof(Vec3), &normals[0]);

	/* load an image file directly as a new OpenGL texture
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

	*/
}

Asset::~Asset()
{
	engine_ = NULL;
}

void Asset::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLuint positionID = glGetAttribLocation(engine_->shaderProgramID, "s_vPosition");
	GLuint normalID = glGetAttribLocation(engine_->shaderProgramID, "s_vNormal");
	GLuint lightID = glGetUniformLocation(engine_->shaderProgramID, "vLight");	// NEW

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size() * sizeof(Vec3)));

	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(normalID);

	glUseProgram(engine_->shaderProgramID);
	engine_->theta += 0.01f;
	engine_->scaleAmount = sin(engine_->theta);

	// Fill the matrices with valid data
	MatrixMath::makeScale(engine_->scaleMatrix, 0.1f, 0.1f, 0.1f);			// Fill the scaleMatrix variable
	MatrixMath::makeRotateY(engine_->rotYMatrix, engine_->theta);						// Fill the rotYMatrix variable
	MatrixMath::makeTranslate(engine_->transMatrix, 0.0f, 0.0f, position_.z);		// Fill the transMatrix to push the model back 1 "unit" into the scene
	// Multiply them together 
	MatrixMath::matrixMult4x4(engine_->tempMatrix1, engine_->rotYMatrix, engine_->scaleMatrix);	// Scale, then rotate...
	MatrixMath::matrixMult4x4(engine_->M, engine_->transMatrix, engine_->tempMatrix1);	// ... then multiply THAT by the translate

	// NEW!  Copy the rotations into the allRotsMatrix
	MatrixMath::copyMatrix(engine_->rotYMatrix, engine_->allRotsMatrix);

	// Important! Pass that data to the shader variables
	glUniformMatrix4fv(engine_->modelMatrixID, 1, GL_TRUE, engine_->M);
	glUniformMatrix4fv(engine_->viewMatrixID, 1, GL_TRUE, engine_->V);
	glUniformMatrix4fv(engine_->perspectiveMatrixID, 1, GL_TRUE, engine_->P);
	glUniformMatrix4fv(engine_->allRotsMatrixID, 1, GL_TRUE, engine_->allRotsMatrix);
	glUniform4fv(engine_->lightID, 1, (GLfloat *)&engine_->light[0]);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
}

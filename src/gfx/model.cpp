#include "gfx/model.hpp"
#include "defines.hpp"
#include "utils/matrix_math.hpp"
#include "engine/sdl_engine.hpp"
#include "gfx/shader_program.hpp"
#include "utils/resource_manager.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Model::Model(const char * obj_path) :
	Object({ 0, 0, 0 }, { 0, 0, 0 })
{
	ObjWavefrontLoader(obj_path, vertices, uvs, normals);

	InitMatrices();

	//
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

void Model::ActivateShaderProgram(GLuint shader_program_id)
{
	shader_program_id_ = shader_program_id;

	// ============ New! glUniformLocation is how you pull IDs for uniform variables===============
	viewMatrixID = glGetUniformLocation(shader_program_id_, "mV");
	modelMatrixID = glGetUniformLocation(shader_program_id_, "mM");
	allRotsMatrixID = glGetUniformLocation(shader_program_id_, "mRotations");	// NEW
	ResourceManager::GetInstance().perspectiveMatrixID = glGetUniformLocation(shader_program_id_, "mP");
	//=============================================================================================
}

void Model::InitMatrices()
{
	theta = 0.0f;
	scaleAmount = 1.0f;

	// Allocate memory for the matrices and initialize them to the Identity matrix
	rotXMatrix = new GLfloat[16];	MatrixMath::makeIdentity(rotXMatrix);
	rotYMatrix = new GLfloat[16];	MatrixMath::makeIdentity(rotYMatrix);
	rotZMatrix = new GLfloat[16];	MatrixMath::makeIdentity(rotZMatrix);
	transMatrix = new GLfloat[16];	MatrixMath::makeIdentity(transMatrix);
	scaleMatrix = new GLfloat[16];	MatrixMath::makeIdentity(scaleMatrix);
	tempMatrix1 = new GLfloat[16];	MatrixMath::makeIdentity(tempMatrix1);
	allRotsMatrix = new GLfloat[16]; MatrixMath::makeIdentity(allRotsMatrix);
	M = new GLfloat[16];			MatrixMath::makeIdentity(M);
	V = new GLfloat[16];			MatrixMath::makeIdentity(V);
}

Model::~Model()
{
}

void Model::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLuint positionID = glGetAttribLocation(shader_program_id_, "s_vPosition");
	GLuint normalID = glGetAttribLocation(shader_program_id_, "s_vNormal");
	GLuint lightID = glGetUniformLocation(shader_program_id_, "vLight");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertices.size() * sizeof(Vec3)));

	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(normalID);

	glUseProgram(shader_program_id_);
	theta += 0.01f;
	scaleAmount = sin(theta);

	// Fill the matrices with valid data
	MatrixMath::makeScale(scaleMatrix, 0.1f, 0.1f, 0.1f);			// Fill the scaleMatrix variable
	MatrixMath::makeRotateY(rotYMatrix, theta);						// Fill the rotYMatrix variable
	MatrixMath::makeTranslate(transMatrix, 0.0f, 0.0f, -2.0f);		// Fill the transMatrix to push the model back 1 "unit" into the scene
	// Multiply them together 
	MatrixMath::matrixMult4x4(tempMatrix1, rotYMatrix, scaleMatrix);	// Scale, then rotate...
	MatrixMath::matrixMult4x4(M, transMatrix, tempMatrix1);	// ... then multiply THAT by the translate

	// NEW!  Copy the rotations into the allRotsMatrix
	MatrixMath::copyMatrix(rotYMatrix, allRotsMatrix);

	// NEW! Change the (V)iew matrix if you want to "move" around the scene
	MatrixMath::makeRotateY(rotYMatrix, orientation_.y);
	MatrixMath::makeTranslate(transMatrix, position_.x, position_.y, position_.z);
	MatrixMath::matrixMult4x4(V, rotYMatrix, transMatrix);

	// Important! Pass that data to the shader variables
	glUniformMatrix4fv(modelMatrixID, 1, GL_TRUE, M);
	glUniformMatrix4fv(viewMatrixID, 1, GL_TRUE, V);
	glUniformMatrix4fv(ResourceManager::GetInstance().perspectiveMatrixID, 1, GL_TRUE, ResourceManager::GetInstance().P);
	glUniformMatrix4fv(allRotsMatrixID, 1, GL_TRUE, allRotsMatrix);

	std::vector< Vec4 > light = ResourceManager::GetInstance().light;

	glUniform4fv(lightID, 1, (GLfloat *)&light[0]);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
}

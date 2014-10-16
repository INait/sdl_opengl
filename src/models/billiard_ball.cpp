#include "models/billiard_ball.hpp"
#include "defines.hpp"
#include "engine/sdl_engine.hpp"
#include "gfx/shader_program.hpp"
#include "utils/resource_manager.hpp"
#include "gfx/renderer.hpp"
#include "gfx/camera.hpp"

BilliardBall::BilliardBall() :
	Mesh({ 0, 0, 0 }, { 0, 0, 0 })
{
	GenerateSphere(0, 0, 0, 1, 200);

	// loader for obj files	
	//	ObjWavefrontLoader(obj_path.c_str(), vertices, uvs, normals);

	rotation_ = 0;

	glGenVertexArrays(1, &vertex_array_id_);
	glBindVertexArray(vertex_array_id_);

	glGenBuffers(3, vbos_);
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(POSITION_ATTRIBUTE);

	// Give normals
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(NORMAL_ATTRIBUTE);

	// Give uvs
	glBindBuffer(GL_ARRAY_BUFFER, vbos_[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(TEXCOORD0_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(TEXCOORD0_ATTRIBUTE);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

BilliardBall::~BilliardBall()
{
}

void BilliardBall::GenerateSphere(float cx, float cy, float cz, float r, int p)
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

void BilliardBall::ApplyShaderProgram(GLuint shader_program_id)
{
	shader_program_id_ = shader_program_id;

	uniformMVP = glGetUniformLocation(shader_program_id_, "ModelViewProjectionMatrix");
	uniformModelMatrix = glGetUniformLocation(shader_program_id_, "ModelMatrix");
	uniformEyePosW = glGetUniformLocation(shader_program_id_, "EyePosW");

	// Light properties.
	uniformLightPosW = glGetUniformLocation(shader_program_id_, "LightPosW");
	uniformLightColor = glGetUniformLocation(shader_program_id_, "LightColor");

	// Global ambient.
	uniformAmbient = glGetUniformLocation(shader_program_id_, "Ambient");

	// Material properties.
	uniformMaterialEmissive = glGetUniformLocation(shader_program_id_, "MaterialEmissive");
	uniformMaterialDiffuse = glGetUniformLocation(shader_program_id_, "MaterialDiffuse");
	uniformMaterialSpecular = glGetUniformLocation(shader_program_id_, "MaterialSpecular");
	uniformMaterialShininess = glGetUniformLocation(shader_program_id_, "MaterialShininess");
}

void BilliardBall::Draw()
{
	glBindVertexArray(vertex_array_id_);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glUseProgram(shader_program_id_);

	const glm::vec4 white{ 1 };
	const glm::vec4 specular_light{ 1 };
	const glm::vec4 black{ 0 };
	const glm::vec4 ambient{ 0.5f, 0.5f, 0.5f, 1.0f };

	// Set the light position
	glUniform4fv(uniformLightPosW, 1, glm::value_ptr(Renderer::GetInstance().light_pos_));
	glUniform4fv(uniformLightColor, 1, glm::value_ptr(white));
	glUniform4fv(uniformAmbient, 1, glm::value_ptr(ambient));

	glm::mat4 model_matrix = glm::rotate(orientation_.x, glm::vec3(1, 0, 0)) *
							 glm::rotate(orientation_.y, glm::vec3(0, 1, 0)) *
							 glm::scale(glm::vec3(position_.z));

	auto camera_ptr = Renderer::GetInstance().camera_ptr_;
	glm::vec4 eyePosW = glm::vec4(camera_ptr->GetPosition(), 1);
	glm::mat4 mvp = camera_ptr->GetProjectionMatrix() * camera_ptr->GetViewMatrix() * model_matrix;

	glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniform4fv(uniformEyePosW, 1, glm::value_ptr(eyePosW));

	// Material properties.
	glUniform4fv(uniformMaterialEmissive, 1, glm::value_ptr(black));
	glUniform4fv(uniformMaterialDiffuse, 1, glm::value_ptr(white));
	glUniform4fv(uniformMaterialSpecular, 1, glm::value_ptr(specular_light));
	glUniform1f(uniformMaterialShininess, 50.0f);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}

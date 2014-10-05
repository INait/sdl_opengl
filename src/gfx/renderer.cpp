#include "gfx/renderer.hpp"
#include "gfx/camera.hpp"
#include "utils/resource_manager.hpp"

DECLARE_SINGLETON_VAR(Renderer)

Renderer::Renderer(int width, int height)
{
	camera_ptr_ = std::make_shared< Camera >();

	camera_ptr_->SetViewport(0, 0, width, height);
	camera_ptr_->SetProjectionRH(30.0f, width / (float)height, 0.1f, 200.0f);

	camera_ptr_->SetPosition(glm::vec3{ 0, 0, 100 });
	camera_ptr_->SetRotation(glm::quat{});
}

Renderer::~Renderer()
{
}

void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto & model : ResourceManager::GetInstance().models_)
		model.second->Draw();

	glFlush();
}
#include <fstream>

#include "engine/sdl_engine.hpp"
#include "utils/resource_manager.hpp"
#include "gfx/shader_program.hpp"
#include "gfx/renderer.hpp"

SdlEngine::SdlEngine()
{
}

SdlEngine::~SdlEngine()
{
}

void SdlEngine::Init( int width, int height, const std::string & res_location )
{
	width_ = width;
	height_ = height;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
		throw std::runtime_error(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	window.reset( SDL_CreateWindow("Sphere", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL) );

	if (window.get() == NULL)
		throw std::runtime_error("Couldn't create sdl window");

	SDL_GLContext glcontext = SDL_GL_CreateContext(window.get());

	if (glcontext == NULL)
		throw std::runtime_error("Couldn't create sdl GL context");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	resource_manager_ = std::make_shared< ResourceManager >();
	ResourceManager::SetInstance(*resource_manager_);
	resource_manager_->LoadXMLResources(res_location);

	renderer_ = std::make_shared< Renderer >(width_, height_);
	Renderer::SetInstance(*renderer_);
}

void SdlEngine::GameLoop()
{
	bool running = true;

	glm::vec3 orient{ 0.0, 0.0, 0.0 };
	glm::vec3 pos{ 0.0, 0.0, 2.0 };

	glm::vec4 lightPos{ 60, 0, 0, 1};

	while(running)
	{
		SDL_Event event;

		while ( SDL_PollEvent(&event) )
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
						case SDLK_w:
							pos.z -= 0.1f;
							break;
						case SDLK_s:
							pos.z += 0.1f;
							break;
						case SDLK_a:
							pos.x -= 0.1f;
							break;
						case SDLK_d:
							pos.x += 0.1f;
							break;
						case SDLK_UP:
							orient.x += 0.5f;
							break;
						case SDLK_DOWN:
							orient.x -= 0.5f;
							break;
						case SDLK_LEFT:
							orient.y -= 0.05f;
							break;
						case SDLK_RIGHT:
							orient.y += 0.05f;
							break;
						case SDLK_u:
							lightPos.x += 5;
							break;
						case SDLK_j:
							lightPos.x -= 5;
							break;
						case SDLK_i:
							lightPos.y += 5;
							break;
						case SDLK_k:
							lightPos.y -= 5;
							break;
						case SDLK_o:
							lightPos.z += 5;
							break;
						case SDLK_l:
							lightPos.z -= 5;
							break;
					}
					break;
			}
		}

		// Draw objects
		auto sphere_it = ResourceManager::GetInstance().models_.find("m2_sphere")->second;
		sphere_it->SetOrientation(orient);
		sphere_it->SetPosition(pos);

		Renderer::GetInstance().light_pos_ = lightPos;
		Renderer::GetInstance().Draw();

		SDL_GL_SwapWindow(window.get());
	}

	ResourceManager::GetInstance().FreeResources();
	SDL_Quit();
}


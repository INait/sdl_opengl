#include <fstream>

#include "engine/sdl_engine.hpp"
#include "utils/matrix_math.hpp"
#include "utils/resource_loader.hpp"
#include "gfx/shader_program.hpp"

SdlEngine::SdlEngine()
{
}

SdlEngine::~SdlEngine()
{
}

void SdlEngine::Init( int width, int height, const std::string & res_location )
{
	// Инициализация SDL
	width_ = width;
	height_ = height;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
		throw std::runtime_error(SDL_GetError());

	// Включаем двойной буфер, настраиваем цвета

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	// Создаем окно с заголовком "Cube", размером 640х480 и расположенным по центру экрана.

	window = SDL_CreateWindow("Sphere", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == NULL) // если не получилось создать окно, то выходим
		throw std::runtime_error("Couldn't create sdl window");

	SDL_GLContext glcontext = SDL_GL_CreateContext(window); // создаем контекст OpenGL

	if (glcontext == NULL) // если не получилось создать окно, то выходим
		throw std::runtime_error("Couldn't create sdl GL context");

	// Инициализация OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // устанавливаем фоновый цвет на черный
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST); // включаем тест глубины
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float) width / (float) height, 0.1f, 100.0f); // настраиваем трехмерную перспективу
	glMatrixMode(GL_MODELVIEW); // переходим в трехмерный режим

	glEnable(GL_CULL_FACE);  // NEW! - we're doing real 3D now...  Cull (don't render) the backsides of triangles
	glCullFace(GL_BACK);	// Other options?  GL_FRONT and GL_FRONT_AND_BACK
	glEnable(GL_DEPTH_TEST);// Make sure the depth buffer is on.  As you draw a pixel, update the screen only if it's closer than previus ones

	InitCamera();

	light.push_back({ 0.0f, 1.0f, 1.0f, 1.0f });

	resource_loader_ = std::make_shared< ResourceLoader >();
	resource_loader_->SetNewAssetFuction( std::bind( &SdlEngine::CreateAsset, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
	resource_loader_->LoadXMLResources( res_location );
}

void SdlEngine::CreateAsset(const char* obj_file, const char* tex_file, const char* vsh_file, const char* fsh_file)
{
	AssetPtr asset = std::make_shared< Asset >( obj_file, tex_file, vsh_file, fsh_file );
	asset->SetSdlEngine( this );
	assets.push_back( asset );
}

void SdlEngine::InitCamera()
{
	MatrixMath::makeIdentity(P);

	// Set up the (P)erspective matrix only once! Arguments are 1) the resulting matrix, 2) FoV, 3) aspect ratio, 4) near plane 5) far plane
	MatrixMath::makePerspectiveMatrix(P, 30.0f, 1.0f, 1.0f, 1000.0f);
}

void SdlEngine::GameLoop()
{
	bool running = true;

	Vec3 orient{ 0.0, 0.0, 0.0 };
	Vec3 pos{ 0.0, 0.0, -6.0 };

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
							pos.z -= 0.1;
							break;
						case SDLK_s:
							pos.z += 0.1;
							break;
						case SDLK_a:
							pos.x -= 0.1;
							break;
						case SDLK_d:
							pos.x += 0.1;
							break;
						case SDLK_UP:
							orient.x += 0.5;
							break;
						case SDLK_DOWN:
							orient.x -= 0.5;
							break;
						case SDLK_LEFT:
							orient.y -= 0.5;
							break;
						case SDLK_RIGHT:
							orient.y += 0.5;
							break;
					}
					break;
			}
		}

		// Draw objects
		for (auto & asset : assets)
		{
			asset->SetOrientation(orient);
			asset->SetPosition(pos);
			asset->Draw();
		}

		glFlush();
		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
}


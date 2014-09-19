#include <fstream>

#include "sdl_engine.hpp"
#include "resource_loader.hpp"

SdlEngine::SdlEngine()
{
}

SdlEngine::~SdlEngine()
{
}

int SdlEngine::Init( int width, int height, const std::string & res_location )
{
	// Инициализация SDL
	width_ = width;
	height_ = height;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Включаем двойной буфер, настраиваем цвета

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	// Создаем окно с заголовком "Cube", размером 640х480 и расположенным по центру экрана.

	window = SDL_CreateWindow("Sphere", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == NULL) // если не получилось создать окно, то выходим
		return -2;

	SDL_GLContext glcontext = SDL_GL_CreateContext(window); // создаем контекст OpenGL

	if (glcontext == NULL) // если не получилось создать окно, то выходим
		return -3;

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

	// shaders
	std::string vShaderSource = "in vec4 s_vPosition; \
								 void main() \
								 { \
									 gl_Position = s_vPosition; \
								 }";
	std::string fShaderSource = "out vec4 s_vColor; \
								 void main() \
								 { \
									 s_vColor = vec4( 1.0, 0.0, 0.0, 1.0 ); \
								 }";
	GLuint vShaderID = makeVertexShader( vShaderSource.c_str() );
	GLuint fShaderID = makeFragmentShader( fShaderSource.c_str() );
	shaderProgramID = makeShaderProgram( vShaderID, fShaderID );

	ResourceLoader res_loader;
	res_loader.LoadXMLResources( res_location, assets );

	for( std::vector< AssetPtr >::const_iterator asset_it = assets.begin(); asset_it != assets.end(); ++asset_it )
		(*asset_it)->SetShaderProgramID( shaderProgramID );

	return 0;
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

std::string SdlEngine::readShaderFile( const char* filename )
{
	std::ifstream t( filename, std::ios::in );
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve((size_t)t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign( ( std::istreambuf_iterator<char>( t ) ),
			std::istreambuf_iterator<char>() );

	return str;
}

GLuint SdlEngine::makeVertexShader( const char* shaderSource )
{
	GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShaderID, 1, ( const GLchar** ) & shaderSource, NULL );
	glCompileShader( vertexShaderID );
	return vertexShaderID;
}

GLuint SdlEngine::makeFragmentShader( const char* shaderSource )
{
	GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderID, 1, ( const GLchar** ) & shaderSource, NULL );
	glCompileShader( fragmentShaderID );
	return fragmentShaderID;
}

GLuint SdlEngine::makeShaderProgram( GLuint vertexShaderID, GLuint fragmentShaderID )
{
	GLuint shaderID = glCreateProgram();
	glAttachShader( shaderID, vertexShaderID );
	glAttachShader( shaderID, fragmentShaderID );
	glLinkProgram( shaderID );
	return shaderID;
}

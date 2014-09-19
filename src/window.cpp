#include <fstream>

#include "../include/window.hpp"
#include "../include/resource_loader.hpp"

SdlWindow::SdlWindow( int width, int height ) : width( width ), height( height )
{
}

SdlWindow::~SdlWindow()
{
}

void SdlWindow::Init( const std::string & res_location )
{
	// Инициализация SDL

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
		return;
	}

	// Включаем двойной буфер, настраиваем цвета

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	// Создаем окно с заголовком "Cube", размером 640х480 и расположенным по центру экрана.

	window = SDL_CreateWindow("Sphere", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if(window == NULL) // если не получилось создать окно, то выходим
		exit(1);

	SDL_GLContext glcontext = SDL_GL_CreateContext(window); // создаем контекст OpenGL

	if(glcontext == NULL) // если не получилось создать окно, то выходим
		exit(1);

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
}

void SdlWindow::GameLoop()
{
	bool running = true;

	float xrf = 0, yrf = 0, zrf = 0; // углы поворота

	while(running)
	{
		SDL_Event event; // события SDL

		while ( SDL_PollEvent(&event) ) // начинаем обработку событий
		{
			switch(event.type) // смотрим:
			{
				case SDL_QUIT: // если произошло событие закрытия окна, то завершаем работу программы
					running = false;
					break;

				case SDL_KEYDOWN: // если нажата клавиша
					switch(event.key.keysym.sym) // смотрим какая
					{
						case SDLK_ESCAPE: // клавиша ESC
							running = false; // завершаем работу программы
							break;
					}
					break;
			}
		}

		// пока программа запущена изменяем углы поворота, тем самым вращая куб

		xrf -= 0.5;
		yrf -= 0.5;
		zrf -= 0.5;

		for( std::vector< AssetPtr >::const_iterator asset_it = assets.begin(); asset_it != assets.end(); ++asset_it )
			(*asset_it)->Draw(xrf, yrf, zrf);

		// обновляем экран

		glFlush();
		SDL_GL_SwapWindow(window);
	}

	SDL_Quit(); // завершаем работу SDL и выходим
}

std::string SdlWindow::readShaderFile( const char* filename )
{
	std::ifstream t( filename, std::ios::in );
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign( ( std::istreambuf_iterator<char>( t ) ),
			std::istreambuf_iterator<char>() );

	return str;
}

GLuint SdlWindow::makeVertexShader( const char* shaderSource )
{
	GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShaderID, 1, ( const GLchar** ) & shaderSource, NULL );
	glCompileShader( vertexShaderID );
	return vertexShaderID;
}

GLuint SdlWindow::makeFragmentShader( const char* shaderSource )
{
	GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderID, 1, ( const GLchar** ) & shaderSource, NULL );
	glCompileShader( fragmentShaderID );
	return fragmentShaderID;
}

GLuint SdlWindow::makeShaderProgram( GLuint vertexShaderID, GLuint fragmentShaderID )
{
	GLuint shaderID = glCreateProgram();
	glAttachShader( shaderID, vertexShaderID );
	glAttachShader( shaderID, fragmentShaderID );
	glLinkProgram( shaderID );
	return shaderID;
}

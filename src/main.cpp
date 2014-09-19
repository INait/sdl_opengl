#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h> // Библиотека SDL 2

#include <GL/gl.h> // Библиотека OpenGL
#include <GL/glu.h> // Библиотека GLU
#include <GL/glext.h>
#include <SOIL/SOIL.h>

#include <string>
#include <streambuf>
#include <iostream> // стандартная библиотека ввода-вывода в C++
#include <fstream>

#include "../3rd/wavefront_loader/wavefront_loader.hpp"

using namespace std;

SDL_Window *window; // задаем окно для SDL

const int width = 640; // ширина окна
const int height = 480; // высота окна

GLuint shaderProgramID;

std::string readShaderFile( const char* filename )
{
	std::ifstream t( filename );
	std::string str;

	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign( ( std::istreambuf_iterator<char>( t ) ),
			    std::istreambuf_iterator<char>() );

	return str;
}

GLuint makeVertexShader( const char* shaderSource )
{
	GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertexShaderID, 1, ( const GLchar** ) & shaderSource, NULL );
	glCompileShader( vertexShaderID );
	return vertexShaderID;
}

GLuint makeFragmentShader( const char* shaderSource )
{
	GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragmentShaderID, 1, ( const GLchar** ) & shaderSource, NULL );
	glCompileShader( fragmentShaderID );
	return fragmentShaderID;
}

GLuint makeShaderProgram( GLuint vertexShaderID, GLuint fragmentShaderID )
{
	GLuint shaderID = glCreateProgram();
	glAttachShader( shaderID, vertexShaderID );
	glAttachShader( shaderID, fragmentShaderID );
	glLinkProgram( shaderID );
	return shaderID;
}

void init()
{
	// Инициализация SDL

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		cout << "Unable to init SDL, error: " << SDL_GetError() << endl;
		exit(1);
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
}

class Asset
{
private:
	std::string path_;
	std::vector< Vec3 > vertices;
	std::vector< Vec2 > uvs;
	std::vector< Vec3 > normals;

	// This will identify our vertex buffer
	GLuint vertexbuffer;

	GLuint texture_id;
public:

	Asset( const char * path ) : path_( path )
	{
		if( ! ObjWavefrontLoader( path_.c_str(), vertices, uvs, normals ) )
		{
			std::cout << "cannot load asset" << std::endl;
		}

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
			 "assets/textures/texture1.jpg",
			 SOIL_LOAD_AUTO,
			 SOIL_CREATE_NEW_ID,
			 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

		int img_width, img_height;
		unsigned char* img = SOIL_load_image("assets/textures/texture1.jpg", &img_width, &img_height, NULL, 0);

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

		glGenTextures(1, &texture_id);

		glBindTexture(GL_TEXTURE_2D, texture_id);
	}
	~Asset() { }

	void Draw(float xrf, float yrf, float zrf)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glTranslatef( 0.0f, 0.0f, -6.0f );
		glRotatef(xrf, 1.0f, 0.0f, 0.0f);   // Вращение куба по X, Y, Z
		glRotatef(yrf, 0.0f, 1.0f, 0.0f);   // Вращение куба по X, Y, Z
		glRotatef(zrf, 0.0f, 0.0f, 1.0f);   // Вращение куба по X, Y, Z

		GLuint positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
		glVertexAttribPointer( positionID, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		glUseProgram( shaderProgramID );
		glEnableVertexAttribArray( positionID );

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

int main(int argc, char *argv[])
{
	init(); // инициализация

	bool running = true;

	float xrf = 0, yrf = 0, zrf = 0; // углы поворота

	Asset sphere( "assets/textures/sphere.obj" );

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

		sphere.Draw(xrf, yrf, zrf);
		// обновляем экран

		glFlush();
		SDL_GL_SwapWindow(window);
	}

	SDL_Quit(); // завершаем работу SDL и выходим
	return 0;
}

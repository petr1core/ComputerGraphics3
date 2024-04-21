//glfw3.lib + glad + opengl32.lib
#define GLFW_INCLUDE_NONE
#include "../libs/glfw-3.4.bin.WIN64/include/GLFW/glfw3.h"
#include "../libs/glad/include/glad/glad.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> // ISO v.17+
using namespace std;

std::string readFile(std::filesystem::path path) {
	cout << "Started readind file: " << path << endl;
	std::ifstream inp(path, std::ios::binary);
	const auto size = std::filesystem::file_size(path);
	std::string out(size, '\0');
	
	inp.read(out.data(), size);
	cout << "Ended reading file: " << path << endl;
	cout << "ReadFile(" << path << ") ended, returned: " << endl << out << endl;
	return out;
}

unsigned int compileShader(unsigned int shader_type, std::string& data) {
	string type = "";
	if (GL_VERTEX_SHADER == shader_type)
		type = "GL_VERTEX_SHADER";
	else if (GL_FRAGMENT_SHADER == shader_type)
		type = "GL_FRAGMENT_SHADER";
	else {
		cout << "???Shader" << endl;
	}
	cout << "Started compiling " << type << endl;
	unsigned id = glCreateShader(shader_type);
	const char* char_str = data.c_str();
	glShaderSource(id, 1, &char_str, nullptr);
	glCompileShader(id);
	cout << "Compiling " << type << " ended, returned " << id << endl;
	return id;
}

unsigned int createShader(std::string& vertex, std::string& fragment) {
	cout << "Started creating shader\nvertex: " << vertex << "\nfragment: " << fragment << endl;
	unsigned vert = compileShader(GL_VERTEX_SHADER, vertex);
	unsigned frag = compileShader(GL_FRAGMENT_SHADER, fragment);
	unsigned program = glCreateProgram();
	cout << "compile vertex: " << vert << "\ncompile fragment: " << frag << "\n createProgram: " << program << endl;
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vert);
	glDeleteShader(frag);
	cout << "Ended createShader, returned program: " << program << endl;
	return program;
}

void render(GLFWwindow* window) {

	cout << "Render function started" << endl;

	cout << "Render function ended" << endl;
}

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	cout << "Window resized on" <<"\nWidth: " <<width << "\nHeight: " << height << endl;
}


int main(void) {
	int width = 720, height = 640;
	//glfwInit();
	if (!glfwInit()) {
		cout << "glfwInit done unsuccessfully" << endl;
		return -1; 
	}
	
	//4 AA
	glfwWindowHint(GLFW_SAMPLES, 4);
	//tells glfw to set opengl to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);    // создание окна с его размером и именем Null нам не интересны ))
	if (!window) {					// Проверка создалось ли окно, если нет, то самоуничтожаемся
		cout << "Can't create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  // Делаем window выбранным окном (то, с которым происходит интеракция и в которое будем рисовать)
	//gladLoadGL();						// напоминаем Glad, что мы создали контекстное окно (строчка выше)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to init Opengl context" << endl;
		return -1;
	}
	float verts[] = {
		-1.0f,-1.0f,		
		1.0f,-1.0f,		
		1.0f,1.0f,		
		// general diagonal split
		1.0f, 1.0f,	
		-1.0f,1.0f,    
		-1.0f,-1.0f,		
	};
	GLuint vertBuff;
	glGenBuffers(1, &vertBuff);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	string vertex = readFile("..\\shaders\\vertex.shdr");
	string fragment = readFile("..\\shaders\\fragment.shdr");

	unsigned int shader = createShader(vertex, fragment);
	//glUniform4f(glGetUniformLocation(shader, "vColor"), 0.0f, 255.0f, 0.0f, 1.0f);


	
	glClearColor(0.0f, 0.0f, 255.0f, 1.0f);			// делаем очищенный "холст" другим (дефолтно - чёрный)
	glUseProgram(shader);
	while (!glfwWindowShouldClose(window)) {	// Цикл обновления двух буфферов пока окно не закрылось 
		
		//render(window);
		// ^ рендерим здесь
		//glBindVertexArray(vertBuff);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);				// то самое обновление буффера
		glfwPollEvents();					// слушаем когда происходят какие-то события
		
		// resize окна 
		int newW = 0, newH = 0;
		glfwGetWindowSize(window, &newW, &newH);
		if (newW != width || newH != height)
			resize(window, newW, newH);
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}

//struct Program
//{
//    static GLuint Load(const char* shader, ...)
//    {
//        GLuint prog = glCreateProgram();
//        va_list args;
//        va_start(args, shader);
//        while (shader)
//        {
//            const GLenum type = va_arg(args, GLenum);
//            AttachShader(prog, type, shader);
//            shader = va_arg(args, const char*);
//        }
//        va_end(args);
//        glLinkProgram(prog);
//        CheckStatus(prog);
//        return prog;
//    }
//
//private:
//    static void CheckStatus(GLuint obj)
//    {
//        GLint status = GL_FALSE;
//        if (glIsShader(obj)) glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
//        if (glIsProgram(obj)) glGetProgramiv(obj, GL_LINK_STATUS, &status);
//        if (status == GL_TRUE) return;
//        GLchar log[1 << 15] = { 0 };
//        if (glIsShader(obj)) glGetShaderInfoLog(obj, sizeof(log), NULL, log);
//        if (glIsProgram(obj)) glGetProgramInfoLog(obj, sizeof(log), NULL, log);
//        std::cerr << log << std::endl;
//        exit(EXIT_FAILURE);
//    }
//
//    static void AttachShader(GLuint program, GLenum type, const char* src)
//    {
//        GLuint shader = glCreateShader(type);
//        glShaderSource(shader, 1, &src, NULL);
//        glCompileShader(shader);
//        CheckStatus(shader);
//        glAttachShader(program, shader);
//        glDeleteShader(shader);
//    }
//};
//
//#define GLSL(version, shader) "#version " #version "\n" #shader
//
//const char* vert = GLSL
//(
//    330 core,
//    layout(location = 0) in vec3 in_pos;
//void main()
//{
//    gl_Position.xyz = in_pos;
//    gl_Position.w = 1;
//}
//);
//
//const char* frag = GLSL
//(
//    330 core,
//    out vec3 color;
//void main()
//{
//    color = vec3(1, 0, 0);
//}
//);
//
//int main()
//{
//    if (glfwInit() == false)
//    {
//        //did not succeed
//        fprintf(stderr, "GLFW failed to initialise.");
//        return -1;
//    }
//
//    //4 AA
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    //tells glfw to set opengl to 3.3
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(640, 480, "DJ KELLER KEEMSTAR", NULL, NULL);
//    if (!window)
//    {
//        fprintf(stderr, "Window failed to create");
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//
//    glewExperimental = true;
//    if (glewInit() != GLEW_OK)
//    {
//        fprintf(stderr, "Glew failed to initialise");
//        glfwTerminate();
//        return -1;
//    }
//
//    //generate VAO
//    GLuint vaoID;
//    glGenVertexArrays(1, &vaoID);
//    glBindVertexArray(vaoID);
//
//    static const GLfloat verts[] =
//    {
//        //X, Y, Z
//        -1.0f, -1.0f, 0.0f,
//        1.0f, -1.0f, 0.0f,
//        0.0f, 1.0f, 0.0f
//    };
//
//    GLuint program = Program::Load
//    (
//        vert, GL_VERTEX_SHADER,
//        frag, GL_FRAGMENT_SHADER,
//        NULL
//    );
//
//    //generate VBO
//    GLuint vboID;
//    glGenBuffers(1, &vboID);
//    glBindBuffer(GL_ARRAY_BUFFER, vboID);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
//
//    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
//    do
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//        glEnableVertexAttribArray(0);
//
//        glBindBuffer(GL_ARRAY_BUFFER, vboID);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//
//        glUseProgram(program);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        glDisableVertexAttribArray(0);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//
//    } while (glfwWindowShouldClose(window) == false);
//
//    return 0;
//}
//glfw3.lib + glad + opengl32.lib
#define GLFW_INCLUDE_NONE
#include "../libs/glfw-3.4.bin.WIN64/include/GLFW/glfw3.h"
#include "../libs/glad/include/glad/glad.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> // ISO v.17+
using namespace std;
using namespace std::filesystem;

string readFile(std::filesystem::path path) {
	std::ifstream inp(path, std::ios::binary);
	const auto size = std::filesystem::file_size(path);
	string out(size, '\0');
	
	inp.read(out.data(), size);
	
	return out;
}

unsigned int compileShader(unsigned int shader_type, string& data) {
	unsigned id = glCreateShader(shader_type);
	const char* char_str = data.c_str();

	glShaderSource(id, 1, &char_str, nullptr);
	glCompileShader(id);
	return id;
}

unsigned int createShader(string& vertex, string& fragment) {
	unsigned vert = compileShader(GL_VERTEX_SHADER, vertex);
	unsigned frag = compileShader(GL_FRAGMENT_SHADER, fragment);
	unsigned program = glCreateProgram();
	
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vert);
	glDeleteShader(frag);
	return program;
}

void render(GLFWwindow* window) {

	
}

void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(void) {
	int width = 720, height = 640;
	//glfwInit();
	if (!glfwInit()) {
		return -1; 
	}
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);    // создание окна с его размером и именем Null нам не интересны ))
	if (!window) {					// Проверка создалось ли окно, если нет, то самоуничтожаемся
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  // Делаем window выбранным окном (то, с которым происходит интеракция и в которое будем рисовать)
	gladLoadGL();						// напоминаем Glad, что мы создали контекстное окно (строчка выше)

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

	glUseProgram(shader);

	glUniform4f(glGetUniformLocation(shader, "color1"), 0.2, 0.7, 0.3, 1.0);

	while (!glfwWindowShouldClose(window)) {	// Цикл обновления двух буфферов пока окно не закрылось 
		
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.0f, 255.0f, 255.0f, 1);			// делаем очищенный "холст" другим (дефолтно - чёрный)

		glDrawArrays(GL_TRIANGLES, 0, 6);
		//render(window);
		// ^ рендерим здесь

		glfwSwapBuffers(window);				// то самое обновление буффера

		
		glfwPollEvents();					// слушаем когда происходят какие-то события
		
		// resize окна 
		/*int newW = 0, newH = 0;
		glfwGetWindowSize(window, &newW, &newH);
		if (newW != width || newH != height)
			resize(window, newW, newH);*/
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}
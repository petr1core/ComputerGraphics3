//glfw3.lib + glad + opengl32.lib
#include "../libs/glfw-3.4.bin.WIN64/include/GLFW/glfw3.h"

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1; 
	}
	// создание окна с его размером и именем Null нам не интересны ))
	window = glfwCreateWindow(1920, 1080, "Window", NULL, NULL);
	if (!window) {    // Проверка создалось ли окно, если нет, то самоуничтожаемся
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);  // Делаем window выбранным окном (то, с которым происходит интеракция и в которое будем рисовать)
	while (!glfwWindowShouldClose(window)) {  // Цикл обновления двух буфферов пока окно не закрылось 
		
		// рендерим здесь

		glClear(GL_COLOR_BUFFER_BIT); 
		
		// то самое обновление буффера
		glfwSwapBuffers(window);

		// слушаем когда происходят какие-то события 
		glfwPollEvents();
	}

	glfwTerminate();
}
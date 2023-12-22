#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glad.h"
#include <glm/glm.hpp>
#include "Benchmarker.h"

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "Window", nullptr, nullptr);
	gladLoadGL();

	Benchmarker::Start("Test");
	uint64_t total = 0;
	for (int i = 0; i < 1000000000; i++) {
		total += i;
	}
	std::cout << total << std::endl;
	Benchmarker::End("Test");

	glfwDestroyWindow(window);
	glfwTerminate();
}
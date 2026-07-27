#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<array>

struct GLFWwindow;
typedef void (*GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);

class Renderer
{
public:
	Renderer() = default;
	Renderer(const std::pair<int, int>& version, int width, int height, std::string title = "default");
	~Renderer();

	bool initializeRenderer();
	bool initializeGLFW();
	bool initializeGLAD();
	bool initializeImGui();

	bool constructWindow();

	bool shouldClose();
	void setResizeCallback(GLFWframebuffersizefun func);
	void swapBuffers();
	void pollEvents();
	void clearColor(std::array<float, 4> color, GLbitfield mask);

	void styleImGui();

private:
	bool validateGLVersion(const std::pair<int, int>& version) const;

private:
	GLFWwindow* window;
	int width;
	int height;

	std::string title;
	std::pair<int, int> version;
};

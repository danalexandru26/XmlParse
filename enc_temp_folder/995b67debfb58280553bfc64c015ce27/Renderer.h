#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<vector>
#include<utility>

enum class RendererStatus
{
	GLFW_INITIALIZE_SUCCESS,
	GLFW_INITIALIZE_ERROR,

	GLAD_INITIALIZE_SUCCESS,
	GLAD_INITIALIZE_FAILURE,

	WINDOW_HANDLER_SUCCESS,
	WINDOW_HANDLER_FAILURE,

	OPENGL_VERSION_VALID,
	OPENGL_VERSION_INVALID,

	RENDERER_VALID,
	RENDERER_INVALID
};

class Renderer
{
public:
	Renderer() = default;
	Renderer(const std::pair<int, int>& version, int width, int height, std::string title = "default");
	~Renderer();

	RendererStatus initializeGLFW();
	RendererStatus initializeGLAD();

	RendererStatus constructWindow();

	bool shouldClose();

private:
	void terminateGLFW();
	RendererStatus validateGLVersion(const std::pair<int, int>& version) const;

private:
	GLFWwindow* window;
	int width;
	int height;

	std::string title;
	std::pair<int, int> glVersion;
	std::vector<float> clearColor;

	RendererStatus state;
};

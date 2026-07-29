#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<array>
#include"glad/glad.h"

inline const ImWchar ro_glyph_ranges[] = {
	0x0020, 0x00FF,
	0x0102, 0x0103,
	0x0106, 0x0107,
	0x0122, 0x0123,
	0x0130, 0x0131,
	0x015E, 0x015F,
	0x0162, 0x0163,
	0x0218, 0x021B,
	0,
};

struct GLFWwindow;
typedef void (*GLFWframebuffersizefun)(GLFWwindow* window, int width, int height);

class Renderer
{
public:
	Renderer() = default;
	Renderer(const std::pair<int, int>& version, int width, int height, const std::string& title = "default");
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

	void setupFont(ImGuiIO& io, const ImWchar gylphRange[]);

private:
	bool validateGLVersion(const std::pair<int, int>& version) const;

private:
	GLFWwindow* window;

	std::pair<int, int> version;
	int width;
	int height;
	std::string title;
};

#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<array>
#include"glad/glad.h"

inline const ImWchar ro_glyph_ranges[] = {
	0x0020, 0x00FF, // Basic Latin + Latin Supplement
	0x0102, 0x0103, // Ă, ă
	0x0106, 0x0107, // Ć, ć (optional extended)
	0x0122, 0x0123, // Ģ, ģ (optional extended)
	0x0130, 0x0131, // İ, ı
	0x015E, 0x015F, // Ş, ş (S-cedilla - legacy Romanian)
	0x0162, 0x0163, // Ţ, ţ (T-cedilla - legacy Romanian)
	0x0218, 0x021B, // Ș, ș, Ț, ț (S-comma, T-comma - standard Romanian)
	0,
};

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

	void setupFont(ImGuiIO& io, const ImWchar gylphRange[]);
	void styleLight();

private:
	bool validateGLVersion(const std::pair<int, int>& version) const;

private:
	GLFWwindow* window;
	int width;
	int height;

	std::string title;
	std::pair<int, int> version;
};

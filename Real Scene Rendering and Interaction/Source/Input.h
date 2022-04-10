#pragma once

#include "Camera.h"

class Input
{
	Camera cam;

public:
	static Input& getInstance()
	{
		static Input instance; 
		return instance;
	}

	Input();

	void SetCamera(Camera cam);

	void TriggerKey(GLFWwindow* window, int key, int action);

	void RegisterMouse(GLFWwindow* window, double xpos, double ypos);
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
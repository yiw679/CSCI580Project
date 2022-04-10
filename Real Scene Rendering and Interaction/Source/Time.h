#pragma once
#include "glad/glad.h"
#include <GLFW\glfw3.h>

class Time
{
public:
	void update();
	GLfloat deltaTime = 0.0f;

private:
	GLfloat lastFrame = 0.0f;
};


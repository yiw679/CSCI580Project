#pragma once
#include "Time.h"
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <vector>

using namespace std;

class Camera
{
public:
	Camera();
	Camera(Time* _time);
	glm::mat4 getPosition();

	void setLocation(glm::vec3 pos);
	void moveCamera(int direction);
	void updateCameraRotation(GLdouble mousePosX, GLdouble mousePosY);
private:
	Time* time;

	GLfloat speed = 15.0f;

	// cam positions
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat yaw = 0.0f;
	GLfloat pitch = 0.0f;
	GLfloat sensitivity = 0.05;

	// mouse position details
	GLboolean firstMouse = true;

	GLfloat lastX = 0.0f;
	GLfloat lastY = 0.0f;
};

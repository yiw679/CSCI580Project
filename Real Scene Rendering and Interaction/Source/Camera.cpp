#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(Time* _time)
	: time(_time)
{
}

glm::mat4 Camera::getPosition()
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::setLocation(glm::vec3 pos)
{
	position = pos;
}

void Camera::moveCamera(int direction)
{
	switch (direction) {
	case 1:
		position += forward * speed * time->deltaTime;
		break;
	case 2:
		position -= forward * speed * time->deltaTime;
		break;
	case 3:
		position -= glm::normalize(glm::cross(forward, up)) * speed * time->deltaTime;
		break;
	case 4:
		position += glm::normalize(glm::cross(forward, up)) * speed * time->deltaTime;
		break;
	default:
		break;
	}
}

void Camera::updateCameraRotation(GLdouble mousePosX, GLdouble mousePosY)
{
	if (firstMouse)
	{
		lastX = mousePosX;
		lastY = mousePosY;
		firstMouse = false;
	}

	GLfloat xoffset = mousePosX - lastX;
	GLfloat yoffset = lastY - mousePosY;
	lastX = mousePosX;
	lastY = mousePosY;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(front);
}
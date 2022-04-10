#include "Input.h"

Input::Input() {

}

void Input::SetCamera(Camera _cam) {
    cam = _cam;
}

void Input::TriggerKey(GLFWwindow* window, int key, int action) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
        cam.moveCamera(1);
    }
    if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
        cam.moveCamera(2);
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
        cam.moveCamera(3);
    }
    if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
        cam.moveCamera(4);
    }
}

void Input::RegisterMouse(GLFWwindow* window, double xpos, double ypos) {
    cam.updateCameraRotation(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Input::getInstance().TriggerKey(window, key, action);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Input::getInstance().RegisterMouse(window, xpos, ypos);
}
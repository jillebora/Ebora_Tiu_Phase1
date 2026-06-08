#include "inputHandler.h"
#include <glm/glm.hpp>

InputHandler* InputHandler::s_instance = nullptr;

InputHandler::InputHandler()
	: keyW(false), keyS(false), keyA(false), keyD(false)
		, isOrtho(true), spacePressed(false), windowHandle(nullptr)

{
}

void InputHandler::Register(GLFWwindow* window)
{
	windowHandle = window;
	s_instance = this;

	glfwSetKeyCallback(window, GLFWKeyCallback);

}

bool InputHandler::ProcessInput(float dt, float& yaw, float& pitch)
{
	if (keyA) yaw -= CameraSpeed * dt;
	if (keyD) yaw += CameraSpeed * dt;
	if (keyW) pitch += CameraSpeed * dt;
	if (keyS) pitch -= CameraSpeed * dt;

	if (pitch > 89.f)
		pitch = 89.f;

	if (pitch < -89.f)
		pitch = -89.f;

	bool spaceNow = (glfwGetKey(windowHandle, GLFW_KEY_SPACE) == GLFW_PRESS);
	bool toggleFired = spaceNow && !spacePressed;
	spacePressed = spaceNow;

	return toggleFired;	// toggle pause on this frame
}

void InputHandler::OnKey(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_W)
		keyW = (action != GLFW_RELEASE);
	if (key == GLFW_KEY_S)
		keyS = (action != GLFW_RELEASE);
	if (key == GLFW_KEY_A)
		keyA = (action != GLFW_RELEASE);
	if (key == GLFW_KEY_D)
		keyD = (action != GLFW_RELEASE);

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_1)
			isOrtho = true;			// orthographic
		if (key == GLFW_KEY_2)		// perspective
			isOrtho = false;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}


void InputHandler::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (s_instance)
		s_instance->OnKey(window, key, action);
}

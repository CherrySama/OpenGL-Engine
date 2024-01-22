#include "Camera.h"
#include <Windows.h>

Camera::Camera(glm::vec3 pos, glm::vec3 worldup, float pitch_ , float yaw_ )
{
	position = pos;
	worldUp = worldup;
	pitch = pitch_;
	yaw = yaw_;

	ps = PeoleStatus::NORMAL;
	mouseSpeed = SPEED;
	mouseSensitivity = SENSITIVITY;
	zoom = ZOOM;
	speedUp = 1.0f;
	firstMouse = true;
	eyeHeight = EYEHEIGHT;
	gravity = -GRAVITY;
	blinn = false;
	blinnKeyPressed = false;

	updateCameraPos();
}

Camera::~Camera()
{
	std::cout << "Camera has been deleted..." << std::endl;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::calculateLookAtMatrix(glm::vec3 pos, glm::vec3 target, glm::vec3 worldup)
{
	glm::vec3 z = glm::normalize(pos - target);
	glm::vec3 x = glm::normalize(glm::cross(glm::normalize(worldup), z));
	glm::vec3 y = glm::cross(z, x);
	
	// Rotation、Translation其实是相机在世界坐标系中的变换, 其实我们用它来看物体也就是它这个变换的逆变换
	glm::mat4 translation = glm::mat4(1.0f);
	translation[0][3] = -pos.x;
	translation[1][3] = -pos.y;
	translation[2][3] = -pos.z;
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = x.x, rotation[0][1] = y.x, rotation[0][2] = z.x;
	rotation[1][0] = x.y, rotation[1][1] = y.y, rotation[1][2] = z.y;
	rotation[2][0] = x.z, rotation[2][1] = y.z, rotation[2][2] = z.z;

	return rotation * translation;
}

void Camera::mouseMovement(GLboolean constrainPitch)
{
	double currentMouseX, currentMouseY;
	glfwGetCursorPos(this->window, &currentMouseX, &currentMouseY);

	if (this->firstMouse)
	{
		this->mouseX = currentMouseX;
		this->mouseY = currentMouseY;
		this->firstMouse = false;
	}
	this->yaw += (currentMouseX - this->mouseX) * this->mouseSensitivity;
	this->pitch += (this->mouseY - currentMouseY) * this->mouseSensitivity;

	this->mouseX = currentMouseX;
	this->mouseY = currentMouseY;
	// 限制范围
	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}
	updateCameraPos();
}

void Camera::walk(float deltaTime)
{
	float velocity = this->mouseSpeed * deltaTime;
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, true);
	if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		this->speedUp = 5.0f;
	else
		this->speedUp = 1.0f;
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->position += this->forward * velocity * this->speedUp;
		if (!this->ps == JUMPING)
			this->position.y = this->eyeHeight;
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->position -= this->forward * velocity * this->speedUp;
		if (!this->ps == JUMPING)
			this->position.y = this->eyeHeight;
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
		this->position -= this->right * velocity * this->speedUp;
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
		this->position += this->right * velocity * this->speedUp;

	// jump
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS && !this->ps==JUMPING)
	{
		this->jumpTime = JUMPTIME;
		this->ps = JUMPING;
	}
	if (this->jumpTime > 0) {
		this->gravity += (JUMPACCELER * (this->jumpTime / JUMPTIME)) * deltaTime;
		this->jumpTime -= deltaTime;
	}
	this->gravity -= GRAVITY * deltaTime;
	this->position.y += this->gravity * deltaTime * 10;
	if (this->position.y < this->eyeHeight) {
		this->position.y = this->eyeHeight;
		this->gravity = 0;
		this->ps = NORMAL;
	}

	checkCollision();
}

void Camera::fly(float deltaTime)
{
	float velocity = this->mouseSpeed * deltaTime;
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(this->window, true);
	if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		this->speedUp = 5.0f;
	else
		this->speedUp = 1.0f;
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
		this->position += this->forward * velocity * this->speedUp;
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
		this->position -= this->forward * velocity * this->speedUp;
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
		this->position -= this->right * velocity * this->speedUp;
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
		this->position += this->right * velocity * this->speedUp;
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->position += this->up * velocity * this->speedUp;
	if (glfwGetKey(this->window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
	{
		blinn = !blinn;
		blinnKeyPressed = true;
	}
	if (glfwGetKey(this->window, GLFW_KEY_B) == GLFW_RELEASE)
		blinnKeyPressed = false;

	

	checkCollision();
}

void Camera::checkCollision() {
	if (position.x > 90.0f)
		position.x = 90.0f;
	if (position.x < -90.0f)
		position.x = -90.0f;
	if (position.z > 75.0f)
		position.z = 75.0f;
	if (position.z < -35.0f)
		position.z = -35.0f;
}

void Camera::updateCameraPos()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(this->pitch)) * glm::cos(glm::radians(this->yaw));
	front.y = glm::sin(glm::radians(this->pitch));
	front.z = glm::cos(glm::radians(this->pitch)) * glm::sin(glm::radians(this->yaw));
	
	this->forward = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->forward, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->forward));
}


 #pragma once
 
 #include <glad/glad.h>
 #include <GLFW/glfw3.h>
 #include <glm/glm.hpp>
 #include <glm/gtc/matrix_transform.hpp>
 #include <vector>
 #include <iostream>
 #include "Shader.h"
 
 enum CameraMovement
 {
 	FORWARD,
 	BACKWARD,
 	LEFT,
 	RIGHT,
 	JUMP
 };
 
 enum PeoleStatus
 {
 	NORMAL,
 	JUMPING
 };
 
 // default camera settings
 const float YAW			= -90.0f;
 const float PITCH		= 0.0f;
 const float SPEED		= 15.0f;
 const float SENSITIVITY = 0.05f;
 const float ZOOM		= 45.0f;
 const float JUMPTIME    = 0.1f;
 const float GRAVITY		= 4.9f;
 const float JUMPACCELER = 30.0f;
 const float EYEHEIGHT   = 5.f;
 
 class Camera
 {
 public:
 	GLFWwindow* window;
 	PeoleStatus ps;
 	// camera attribute
 	glm::vec3 position;
 	glm::vec3 forward;
 	glm::vec3 right;
 	glm::vec3 up;
 	glm::vec3 worldUp;
 	// euler angles
 	float yaw;
 	float pitch;
 	// camera option
 	float mouseSpeed;
 	float mouseSensitivity;
 	float zoom;
 	float speedUp; // 加速
 	double mouseX;
 	double mouseY;
 	bool firstMouse;
 	// 跳跃
 	float eyeHeight;
 	float gravity;
 	float jumpTime;
 	// blinn
 	bool blinn;
 	bool blinnKeyPressed;
 	// hdr
 	bool hdr;
 	bool hdrKeyPressed;
 
 	// constructor
 	Camera(glm::vec3 pos, glm::vec3 worldup, float pitch_ = PITCH, float yaw_ = YAW);
 	~Camera();
 
 	// return the lookat matrix
 	glm::mat4 getViewMatrix();
 	// calculate the lookat matrix(view matrix)
 	glm::mat4 calculateLookAtMatrix(glm::vec3 pos, glm::vec3 target, glm::vec3 worldup);
 	
 	// 鼠标移动
 	void mouseMovement(GLboolean constrainPitch = true);
 	// 键盘交互
 	void walk(float deltaTime);
 	void fly(float deltaTime);
 	// 碰撞检查(使人物在一个盒子里活动)
 	void checkCollision();
 
 private:
 	void updateCameraPos();
 };
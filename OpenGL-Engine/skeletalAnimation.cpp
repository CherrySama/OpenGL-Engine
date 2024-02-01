#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/animator.h>
#include <learnopengl/model_animation.h>
#include <iostream>
#include "Shader.h"
#include "Camera.h"

// settings
GLFWwindow* window;
glm::vec2 windowSize;
const bool isFullScreen = false;
// camera
std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(-5.f, 5.f, 10.f), glm::vec3(0, 1.0f, 0));
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void openglInit()
{
	const char* TITLE = "OpenGLEngine";
	const unsigned int WIDTH = 1600;
	const unsigned int HEIGHT = 900;
	// glfw: initialize and configure
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW..." << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // the size of window will be unresizable

	GLFWmonitor* monitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;
	window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, monitor, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window..." << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标光标

	// glad: load all OpenGL function pointers
	// 因此如果没有gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)这一步，
	// OpenGL那些内置的函数（gl开头的）是找不到的。访问的时候可能提示0x000000000错误。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD..." << std::endl;
		return;
	}
	windowSize = glm::vec2(WIDTH, HEIGHT);
	camera->window = window;
}

int main()
{
	openglInit();
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	// shaders
	Shader ourShader("animateVS.glsl", "animateFS.glsl");

	// load models
	Model_Animate ourModel("resources/models/Breakdance/Breakdance 1990.fbx");
	Animation danceAnimation("resources/models/Breakdance/Breakdance 1990.fbx", &ourModel);
	Animator animator(&danceAnimation);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		camera->fly(deltaTime);
		camera->mouseMovement();
		animator.UpdateAnimation(deltaTime);

		// render
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			ourShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}


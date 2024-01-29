#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include "Camera.h"
#include "Shader.h"
#include "Model.h"

// settings
GLFWwindow* window;
glm::vec2 windowSize;
const bool isFullScreen = false;
// camera
std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0, 1.0f, 0));
// hdr exposure
float exposure = 1.0f;
// VAO
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
unsigned int quadVAO = 0;
unsigned int quadVBO = 0;


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

// render a 1x1 3D cube in NDC.
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// front face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, -1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, -1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, -1.0f, -1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, -1.0f, // top-left
			// back face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			 // bottom face
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 // top face
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

int main()
{
	openglInit();
	glEnable(GL_DEPTH_TEST);

	// shader
	Shader lightShader("lightingVS.glsl", "lightingFS.glsl");
	Shader hdrShader("hdrVS.glsl", "hdrFS.glsl");
	Shader bloomScene("bloomVS.glsl", "bloomFS.glsl");
	Shader bloomLight("bloomVS.glsl", "bloomBrightColorFS.glsl");
	Shader bloomBlur("bloomBlurVS.glsl", "bloomBlurFS.glsl");
	Shader bloomBlend("bloomBlendVS.glsl", "bloomBlendFS.glsl");

	// texture
	unsigned int woodTexture = loadTexture("resources/wood.png");

	//// float point framebuffer configure
	//unsigned int hdrFBO;
	//glGenFramebuffers(1, &hdrFBO);
	//// create float point color buffer
	//unsigned int hdrColorBuffer;
	//glGenTextures(1, &hdrColorBuffer);
	//glBindTexture(GL_TEXTURE_2D, hdrColorBuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowSize.x, windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//// create float point depth buffer
	//unsigned int hdrDepth;
	//glGenRenderbuffers(1, &hdrDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, hdrDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y);
	//// attach buffers
	//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrColorBuffer, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrDepth);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "Framebuffer not complete!" << std::endl;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	   // configure (floating point) framebuffers
	// ---------------------------------------
	unsigned int hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
	unsigned int colorBuffers[2];
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowSize.x, windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowSize.x, windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}

	// lighting info
	// positions
	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
	lightPositions.push_back(glm::vec3(-1.4f, 0.9f, 9.0f));
	//lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	//lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
	lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f)); // red
	//lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.5f)); // green
	//lightColors.push_back(glm::vec3(0.0f, 0.2f, 0.0f)); // blue

	// shaders configure
	//lightShader.use();
	//lightShader.setInt("diffuseTexture", 0);
	//hdrShader.use();
	//hdrShader.setInt("hdrBuffer", 0);
	bloomScene.use();
	bloomScene.setInt("diffuseTexture", 0);
	bloomBlur.use();
	bloomBlur.setInt("image", 0);
	bloomBlend.use();
	bloomBlend.setInt("scene", 0);
	bloomBlend.setInt("bloomBlur", 1);

	// 适应电脑不同渲染的速度以达到camera移动速度较平衡
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera->mouseMovement();
		camera->fly(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1.render scene into float point framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (GLfloat)windowSize.x / (GLfloat)windowSize.y, 0.1f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();
		bloomScene.use();
		bloomScene.setMat4("projection", projection);
		bloomScene.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		// set lighting uniforms
		for (unsigned int i = 0; i < lightPositions.size(); i++)
		{
			bloomScene.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
			bloomScene.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
		}
		bloomScene.setVec3("viewPos", camera->position);
		// render tunnel
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
		bloomScene.setMat4("model", model);
		bloomScene.setInt("inverse_normals", true);
		renderCube();

		// 2.show all the light sources as bright cubes
		bloomLight.use();
		bloomLight.setMat4("projection", projection);
		bloomLight.setMat4("view", view);

		for (unsigned int i = 0; i < lightPositions.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(lightPositions[i]));
			model = glm::scale(model, glm::vec3(0.25f));
			bloomLight.setMat4("model", model);
			bloomLight.setVec3("lightColor", lightColors[i]);
			if (i == 0) // 省略第一个大的背景光
				NULL;// do nothing
			else
			{
				renderCube();
			}
				
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 3.blur bright fragments with 2-steps Guassian blur
		bool horizontal = true, firstIter = true;
		unsigned int amount = 10;
		bloomBlur.use();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			bloomBlur.setInt("horizontal", horizontal);
			// bind texture of other framebuffer (or scene if first iteration)
			glBindTexture(GL_TEXTURE_2D, firstIter ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
			renderQuad();
			horizontal = !horizontal;
			if (firstIter)
				firstIter = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 4. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		bloomBlend.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
		bloomBlend.setInt("bloom", camera->hdr);// 按h键进行bloom的开关
		bloomBlend.setFloat("exposure", exposure);
		renderQuad();

		std::cout << "bloom: " << (camera->hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;

		
		//// 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//hdrShader.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, hdrColorBuffer);
		//hdrShader.setInt("hdr", camera->hdr);
		//hdrShader.setFloat("exposure", exposure);
		//renderQuad();

		//std::cout << "hdr: " << (camera->hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
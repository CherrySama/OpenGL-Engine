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
// quad stuff
unsigned int quadVAO = 0;
unsigned int quadVBO;
// Light position
glm::vec3 lightPos(0.5f, 0.0f, 0.3f);
// parallax map configure
float heightScale = 0.1f;

// light cube data
float vertices[] = {
	//position             //normal             //TexCoord 
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,	0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,	0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,	0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,	0.0f, 0.0f,

};

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

// RenderQuad() Renders a 1x1 quad in NDC
void RenderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0, 1.0, 0.0);
        glm::vec3 pos2(-1.0, -1.0, 0.0);
        glm::vec3 pos3(1.0, -1.0, 0.0);
        glm::vec3 pos4(1.0, 1.0, 0.0);
        // texture coordinates
        glm::vec2 uv1(0.0, 1.0);
        glm::vec2 uv2(0.0, 0.0);
        glm::vec2 uv3(1.0, 0.0);
        glm::vec2 uv4(1.0, 1.0);
        // normal vector
        glm::vec3 nm(0.0, 0.0, 1.0);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // - triangle 1
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent1 = glm::normalize(bitangent1);

        // - triangle 2
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent2 = glm::normalize(tangent2);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent2 = glm::normalize(bitangent2);


        GLfloat quadVertices[] = {
            // Positions            // normal         // TexCoords  // Tangent                          // Bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

int main()
{
    openglInit();
    glEnable(GL_DEPTH_TEST);


	// light cube
	unsigned int lightVAO, lightVBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    // load shader
    Shader normalMapWall("NormalMapVS.glsl", "NormalMapFS.glsl");
    Shader parallaxMapWall("parallaxMapVS.glsl", "parallaxMapFS.glsl");
    Shader lightCube("lightcubeVS.glsl", "lightcubeFS.glsl");

    // load texture
    unsigned int diffuseNormalMap = loadTexture("resources/brickwall.jpg");
    unsigned int normalNormalMap = loadTexture("resources/brickwall_normal.jpg");
    unsigned int diffuseParallaxMap = loadTexture("resources/bricks2.jpg");
    unsigned int normalParallaxMap = loadTexture("resources/bricks2_normal.jpg");
    unsigned int heightParallaxMap = loadTexture("resources/bricks2_disp.jpg");

    normalMapWall.use();
    normalMapWall.setInt("diffuseMap", 0);
    normalMapWall.setInt("normalMap", 1);

    parallaxMapWall.use();
    parallaxMapWall.setInt("diffuseMap", 0);
	parallaxMapWall.setInt("normalMap", 1);
	parallaxMapWall.setInt("depthMap", 2);

    // 适应电脑不同渲染的速度以达到camera移动速度较平衡
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        camera->mouseMovement();
        camera->fly(deltaTime);

        // change the light's position
		lightPos.x = static_cast<float>(std::sin(glfwGetTime() * 0.5));
		lightPos.y = static_cast<float>(std::sin(glfwGetTime() * 0.5));

        // clear the buff
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        
		// light
		lightCube.use();
		lightCube.setMat4("projection", projection);
		lightCube.setMat4("view", view);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.05f));
		lightCube.setMat4("model", model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        // normal mapping
        normalMapWall.use();
        normalMapWall.setMat4("view", view);
        normalMapWall.setMat4("projection", projection);
        // render normal-mapped quad
        // Rotates the quad to show normal mapping works in all directions
        model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
        normalMapWall.setMat4("model", model);
        normalMapWall.setVec3("viewPos", camera->position);
        normalMapWall.setVec3("lightPos", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseNormalMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalNormalMap);
        RenderQuad();
       
        // parallax mapping
        parallaxMapWall.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		parallaxMapWall.setMat4("model", model);
		parallaxMapWall.setMat4("view", view);
		parallaxMapWall.setMat4("projection", projection);
		parallaxMapWall.setVec3("viewPos", camera->position);
		parallaxMapWall.setVec3("lightPos", lightPos);
		parallaxMapWall.setFloat("heightScale", heightScale);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseParallaxMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalParallaxMap);
		glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, heightParallaxMap);
        RenderQuad();

        glfwSwapBuffers(window);
		glfwPollEvents();
    }
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);

    glfwTerminate();
    return 0;
}
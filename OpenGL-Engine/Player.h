#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"

class Player
{
public:
	Player(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera);
	~Player();

	std::shared_ptr<Shader> shader;
	std::shared_ptr<Camera> camera;

	void lightOn(glm::vec3 lightpos);
};


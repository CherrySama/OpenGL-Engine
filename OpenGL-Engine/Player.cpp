#include "Player.h"

Player::Player(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera)
{
	this->shader = shader;
	this->camera = camera;
}

Player::~Player()
{
	std::cout << "player has been deleted..." << std::endl;
}

void Player::lightOn(glm::vec3 lightpos)
{
	shader->use();
	shader->setInt("material.diffuse", 0);
	shader->setInt("material.specular", 1);
	shader->setVec3("viewPos", camera->position);
	shader->setFloat("material.shininess", 64.0f);

	// sun light
	shader->setVec3("sunLight.direction", -0.2f, -1.0f, -0.3f);
	shader->setVec3("sunLight.ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("sunLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader->setVec3("sunLight.specular", 0.5f, 0.5f, 0.5f);
	//point light
	shader->setVec3("pointLight.position", lightpos);
	shader->setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
	shader->setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
	shader->setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("pointLight.Kc", 1.0f);
	shader->setFloat("pointLight.Kl", 0.09f);
	shader->setFloat("pointLight.Kq", 0.032f);
	// spot light
	shader->setVec3("spotLight.position", camera->position);
	shader->setVec3("spotLight.direction", camera->forward);
	shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	shader->setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
	shader->setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
	shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader->setFloat("spotLight.Kc", 1.0f);
	shader->setFloat("spotLight.Kl", 0.045f);
	shader->setFloat("spotLight.Kq", 0.0075f);
}
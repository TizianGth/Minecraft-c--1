#pragma once
#include "Texture.h"
#include "Renderer.h"
#include "Model.h"

class Skybox {
public:
	Skybox();
	~Skybox();
	void Render(glm::mat4& mvp);
public:
	Model m_Model;
	Shader m_Shader;
private:
	Textures m_skyboxTextures[6] = { SkyboxFront, SkyboxBack, SkyboxTop, SkyboxBottom, SkyboxLeft, SkyboxRight};
	CubeMap m_SkyBox;
};
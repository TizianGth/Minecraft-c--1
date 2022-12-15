#include "Skybox.h"


Skybox::Skybox() : m_Shader("res/shaders/basic.shader")
{
	m_SkyBox.Load(m_skyboxTextures, 0);
	std::vector<float> vertices = {
		-400, -400,  400, -1, -1,  1, 0, // 0
		 400, -400,  400,  1, -1,  1, 0, // 1
		 400,  400,  400,  1,  1,  1, 0, // 2
		-400,  400,  400, -1,  1,  1, 0, // 3
		-400, -400, -400, -1, -1, -1, 0, // 4
		 400, -400, -400,  1, -1, -1, 0, // 5
		 400,  400, -400,  1,  1, -1, 0, // 6
		-400,  400, -400, -1,  1, -1, 0  // 7

	};
	std::vector<int> indices = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		1,5,6,
		6,2,1,

		0,4,7,
		7,3,0,

		2,3,7,
		7,2,6,

		1,5,4,
		4,1,0 
	};
	Mesh mesh;
	mesh.vertices = vertices;
	mesh.indices = indices;
	m_Model.Set(mesh);
	m_Model.addVB();
	m_Model.addIB();
	m_Model.addVA();
}

Skybox::~Skybox()
{
}

void Skybox::Render(glm::mat4& mvp)
{
	m_SkyBox.Bind(3);
	m_Shader.Bind();
	m_Shader.SetUniform1i("u_Texture", 3);
	m_Shader.SetUniformMat4f("u_MVP", mvp);
}

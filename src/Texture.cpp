#include "Texture.h"

#include "stb_image/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path) : m_RenderID(0), m_FilePath(path), m_localBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);


	glGenTextures(1, &m_RenderID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_localBuffer) {
		stbi_image_free(m_localBuffer);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RenderID);
}

void Texture::Bind(unsigned int slot) const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	// TODO: FIND OUT MAX TEXUTRE SLOTS ON MACHINE 
	glActiveTexture(GL_TEXTURE0 + slot);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

// CUBEMAP
std::string textLoc = "res/textures/";

CubeMap::CubeMap()
	: m_RenderID(0), m_Width(0), m_Height(0), m_BPP(0), texturePaths({ {GrassSide,  textLoc + "side.png"}, {Grass,  textLoc + "Grass_Top.png"},
		{Dirt, textLoc + "dirt.png"}, {GrassMask,  textLoc + "Grass_Mask.png"}, { Null, ""} , 
		{ SkyboxTop,  textLoc + "/skybox/top.png"} , { SkyboxBottom,  textLoc + "/skybox/bottom.png"} , { SkyboxRight,  textLoc + "/skybox/right.png"},
		{ SkyboxLeft,  textLoc + "/skybox/left.png"} , { SkyboxFront,  textLoc + "/skybox/front.png"} , { SkyboxBack,  textLoc + "/skybox/back.png"} })
{

}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &m_RenderID);
}

void CubeMap::Bind(unsigned int slot) const
{
	// TODO: FIND OUT MAX TEXUTRE SLOTS ON MACHINE 
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderID);
}

void CubeMap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::Load(Textures textures[6], int offset)
{
	glGenTextures(1, &m_RenderID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderID);


	for (int i = 0; i < 6; i++) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

		stbi_set_flip_vertically_on_load(0);
		m_localBuffer[i] = stbi_load(texturePaths[textures[i]].c_str(), &m_Width, &m_Height, &m_BPP, 4);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_Width, m_Height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer[i]);

		stbi_image_free(m_localBuffer[i]);
	}
}



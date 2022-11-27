#include "Texture.h"

#include "stb_image/stb_image.h"

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

CubeMap::CubeMap(Textures textures[6], int offset)
	: m_RenderID(0), m_Width(0), m_Height(0), m_BPP(0), texturePaths({ {GrassSide, "res/textures/side.png"}, {Grass, "res/textures/Grass_Top.png"}, {Dirt, "res/textures/dirt.png"}, {GrassMask, "res/textures/Grass_Mask.png"}, { Null, ""} })
{
	


	glGenTextures(1, &m_RenderID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++) {
		if (textures[i] != Null) {
			stbi_set_flip_vertically_on_load(0);
			m_localBuffer[i] = stbi_load(texturePaths[textures[i]].c_str(), &m_Width, &m_Height, &m_BPP, 4);
		}
		else {
			m_localBuffer[i] = 0;
		}
	}

	stbi_set_flip_vertically_on_load(0);
	m_localBuffer[0] = stbi_load(texturePaths[textures[0]].c_str(), &m_Width, &m_Height, &m_BPP, 4);
	stbi_set_flip_vertically_on_load(0);
	m_localBuffer[1] = stbi_load(texturePaths[textures[1]].c_str(), &m_Width, &m_Height, &m_BPP, 4);
	stbi_set_flip_vertically_on_load(0);
	m_localBuffer[2] = stbi_load(texturePaths[textures[2]].c_str(), &m_Width, &m_Height, &m_BPP, 4); // TOP
	stbi_set_flip_vertically_on_load(0);
	m_localBuffer[3] = stbi_load(texturePaths[textures[3]].c_str(), &m_Width, &m_Height, &m_BPP, 4); // BOTTOM
	stbi_set_flip_vertically_on_load(0);
	m_localBuffer[4] = stbi_load(texturePaths[textures[4]].c_str(), &m_Width, &m_Height, &m_BPP, 4);
	stbi_set_flip_vertically_on_load(0);
	m_localBuffer[5] = stbi_load(texturePaths[textures[5]].c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, m_Width, m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, m_Width, m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, m_Width, m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, m_Width, m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, m_Width, m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, m_Width, m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer[5]); 

	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	stbi_image_free(m_localBuffer[0]);
	stbi_image_free(m_localBuffer[1]);
	stbi_image_free(m_localBuffer[2]);
	stbi_image_free(m_localBuffer[3]);
	stbi_image_free(m_localBuffer[4]);
	stbi_image_free(m_localBuffer[5]);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);


	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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



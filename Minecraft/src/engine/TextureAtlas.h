#pragma once

#include <stb_image/stb_image.h>
#include <string>

class TextureAtlas
{
public:
	TextureAtlas();
	~TextureAtlas() = default; // TODO: GL_DELETIMAGE

	void Init();
	void AddTexture(std::string path, unsigned int location);

	void Bind(unsigned int slot);
private:
	unsigned int m_RenderID;
	unsigned int m_FixedTexturePx = 16;
};


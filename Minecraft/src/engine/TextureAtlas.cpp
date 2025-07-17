#include "TextureAtlas.h"
#include "Renderer.h"

TextureAtlas::TextureAtlas()
{

}

void TextureAtlas::Init()
{
	int textAmount = 5;

	glGenTextures(1, &m_RenderID);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, m_FixedTexturePx, m_FixedTexturePx, textAmount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, m_RenderID);

	AddTexture("res\\textures\\dirt\\0.png", 0);
	AddTexture("res\\textures\\grass\\1.png", 1);
	AddTexture("res\\textures\\grass\\2.png", 2);
	AddTexture("res\\textures\\water\\0.png", 3);
	AddTexture("res\\textures\\sand\\0.png", 4);
}

void TextureAtlas::AddTexture(std::string path, unsigned int location)
{

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* buffer = stbi_load(path.c_str(), &width, &height, &channels, 4);

	if (!buffer) {
		std::cout << "Error loading texture: " << '"' << path << '"' << "!" << std::endl;
		return;
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, location, m_FixedTexturePx, m_FixedTexturePx, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	stbi_image_free(buffer);
}

void TextureAtlas::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_RenderID);
}

#pragma once

#include <string>
#include <vector>
#include <stb_image/stb_image.h>

class Texture {
private:
	unsigned int m_RenderID;
	std::string m_FilePath;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_Channels;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

class CubeMap {
private:
	unsigned int m_RenderID;
	unsigned char* m_localBuffer[6];
	int m_Width, m_Height, m_BPP;

public:
	CubeMap(std::string paths[6]);
	~CubeMap();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;


	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};

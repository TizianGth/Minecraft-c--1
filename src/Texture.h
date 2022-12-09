#pragma once

#include "Renderer.h"
#include <unordered_map>

class Texture {
private:
	unsigned int m_RenderID;
	std::string m_FilePath;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

enum Textures {
	Null = 0, GrassSide = 1, Grass = 2, Dirt = 3, GrassMask = 4,
	SkyboxTop = 5, SkyboxRight = 6, SkyboxBottom = 7, SkyboxLeft = 8, SkyboxFront = 9, SkyboxBack = 10
};




class CubeMap {
private:
	unsigned int m_RenderID;
	unsigned char* m_localBuffer[6];
	int m_Width, m_Height, m_BPP;
public:
	std::unordered_map<int, std::string> texturePaths;

	CubeMap();
	~CubeMap();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	void Load(Textures textures[6], int offset);

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};

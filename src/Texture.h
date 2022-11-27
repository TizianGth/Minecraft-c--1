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
	GrassSide = 0, Grass = 1, Dirt = 2, GrassMask = 3, Null = 4
};




class CubeMap {
private:
	unsigned int m_RenderID;
	unsigned char* m_localBuffer[6];
	int m_Width, m_Height, m_BPP;
public:
	std::unordered_map<int, std::string> texturePaths;

	CubeMap(Textures textures[6], int offset);
	~CubeMap();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};

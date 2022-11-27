#pragma once
#include <glad/gl.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <string>
#include "vector.h"
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string&source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& game, int value);
	void SetUniform1f(const std::string& game, float value);
	void SetUniform4f(const std::string& game, vector4 value);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);
};
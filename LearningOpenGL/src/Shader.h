#pragma once
#include <string>
#include <unordered_map>

#include "glm\glm.hpp"

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
private:
	unsigned int m_RendererID;

	std::string m_filePath;

	// caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);

	~Shader();

	void Bind() const;

	void Unbind() const;

	// Set Uniforms
	void SetUniform1i(const std::string& name, int value);

	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	int GetUniformLocation(const std::string& name);

	ShaderProgramSource ParseShader();

	unsigned int CompileShader(unsigned int type, const std::string& source);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
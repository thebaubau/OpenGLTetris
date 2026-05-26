#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class Shader {
private:
	unsigned int ID;
	std::string m_FilePath;
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name) const;
	void CheckCompileErrors(GLuint shader, std::string type);

public:

	Shader() = default;
	Shader(const std::string vertexPath, const std::string fragmentPath);

	~Shader();

	void Bind() const;

	void Unbind() const;

	void SetBool(const std::string& name, bool value) const;

	void SetInt(const std::string& name, int value) const;

	void SetFloat(const std::string& name, float value) const;

	void SetVec2(const std::string& name, const glm::vec2& value) const;

	void SetVec2(const std::string& name, float x, float y) const;

	void SetVec3(const std::string& name, const glm::vec3& value) const;

	void SetVec3(const std::string& name, float x, float y, float z) const;

	void SetVec4(const std::string& name, const glm::vec4& value) const;

	void SetVec4(const std::string& name, float x, float y, float z, float w) const;

	void SetMat2(const std::string& name, const glm::mat2& mat) const;

	void SetMat3(const std::string& name, const glm::mat3& mat) const;

	void SetMat4(const std::string& name, const glm::mat4& mat) const;

	void SetIntArray(const std::string& name, int count, const int* arrayValue) const;

	void SetFloatArray(const std::string& name, int count, const float* arrayValue) const;

	void SetVec2Array(const std::string& name, int count, const glm::vec2* arrayValue) const;
};

#pragma once
#include <string>
#include "glad.h"

namespace Utils {
	std::string readFileToString(const std::string& shaderName);
	void checkCompileErrors(GLuint shader, std::string type);
}
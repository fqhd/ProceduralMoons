#include "Utils.h"
#include <fstream>
#include <iostream>

namespace Utils {
    std::string readFileToString(const std::string& shaderName) {
        std::string shaderCode = "";
        std::string line;
        std::ifstream is;
        is.open(shaderName);
        if (is.fail()) {
            std::cout << "Failed to find file: " << shaderName << std::endl;
            is.close();
            return shaderCode;
        }

        while (std::getline(is, line)) {
            shaderCode += line + "\n";
        }

        is.close();
        return shaderCode;
    }

    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
}
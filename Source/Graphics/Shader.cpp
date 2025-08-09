#include <Graphics/Shader.hpp>
#include <Utils/File.hpp>

using namespace TM::Graphics;
using namespace TM::Utils;

// Function to check shader compile errors
void checkShaderCompile(unsigned int shader, std::string type) {
	int success;
	char infoLog[512];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: "
				<< type << "\n" << infoLog << "\n";
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n"
				<< infoLog << "\n";
		}
	}
}

void Shader::Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	// Load shader source code from files
	std::string vertexShaderSource = File::Read(vertexShaderPath);
	std::string fragmentShaderSource = File::Read(fragmentShaderPath);
	const char* vertexShaderCode = vertexShaderSource.c_str();
	const char* fragmentShaderCode = fragmentShaderSource.c_str();

	// Create and compile vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShader);
	checkShaderCompile(vertexShader, "VERTEX");

	// Create and compile fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);
	checkShaderCompile(fragmentShader, "FRAGMENT");

	// Link shaders into a program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkShaderCompile(shaderProgram, "PROGRAM");

	// Clean up shaders as they're no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Store the shader program ID for later use
	this->_programId = shaderProgram;
}

void Shader::Use()
{
	glUseProgram(_programId);
}

void Shader::StopUse()
{
	glUseProgram(0);
}

void Shader::Destroy()
{
	if (_programId != 0) {
		glDeleteProgram(_programId);
		_programId = 0;
	}
}

void Shader::BindUniform1i(const std::string& name, int value)
{
	int location = glGetUniformLocation(_programId, name.c_str());
	if (location == -1) {
		std::cerr << "Warning: uniform '" << name << "' not found in shader program." << std::endl;
		return;
	}
	glUniform1i(location, value);
}

void Shader::BindUniformMatrix4fv(const std::string& name, const float* value)
{
	int location = glGetUniformLocation(_programId, name.c_str());
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}
}

#pragma once

#include <string>
#include <Graphics/Core.hpp>

namespace TM
{
	namespace Graphics
	{
		class Shader
		{
		private:
			unsigned int _programId = -1;

		public:
			Shader() = default;
			~Shader() = default;

			void Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
			bool IsLoaded() { return _programId != -1; }

			void Use();
			void StopUse();
			void Destroy();

			void BindUniform1i(const std::string& name, int value);
			void BindUniformMatrix4fv(const std::string& name, const float* value);
			void BindUniform4f(const std::string& name, float x, float y, float z, float w);
			void BindUniform2f(const std::string& name, float x, float y);
			void BindUniform1f(const std::string& name, float value);
		};
	}
}
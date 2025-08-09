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
			unsigned int _programId;

		public:
			Shader() = default;
			~Shader() = default;
			void Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

			void Use();
			void StopUse();
			void Destroy();

			void BindUniform1i(const std::string& name, int value);
			void BindUniformMatrix4fv(const std::string& name, const float* value);
		};
	}
}
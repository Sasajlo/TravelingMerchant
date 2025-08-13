#include <Graphics/Texture.hpp>

#include <iostream>

// Include stb_image for texture loading
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../../Extern/stb_image.h"
#endif

namespace TM
{
	namespace Graphics
	{
		std::unordered_map<std::string, unsigned int> Texture::_textureCache;
		unsigned int Texture::_nextTextureId = 1;

		unsigned int Texture::Load(const std::string& path)
		{
			// Check if texture is already loaded
			auto it = _textureCache.find(path);
			if (it != _textureCache.end())
			{
				return it->second;
			}

			// Load new texture
			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
			
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

				// Texture parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				stbi_image_free(data);
				
				// Store in cache
				_textureCache[path] = textureID;
				
				return textureID;
			}
			else
			{
				std::cerr << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
				return 0; // Return 0 for failed loads
			}
		}

		void Texture::LoadTexture(const std::string& path)
		{
			_path = path;
			_textureId = Load(path);
		}

		void Texture::Bind() const
		{
			glBindTexture(GL_TEXTURE_2D, _textureId);
		}

		void Texture::ClearCache()
		{
			// Delete all textures from OpenGL
			for (const auto& pair : _textureCache)
			{
				glDeleteTextures(1, &pair.second);
			}
			
			_textureCache.clear();
		}

		size_t Texture::GetCacheSize()
		{
			return _textureCache.size();
		}
	}
}
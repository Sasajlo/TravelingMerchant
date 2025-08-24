#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

namespace TM
{
	namespace Graphics
	{
		enum class TextureFilter
		{
			LINEAR,   // Smooth
			NEAREST   // Pixel-perfect
		};

		class Texture
		{
		private:
			static std::unordered_map<std::string, unsigned int> _textureCache;
			static unsigned int _nextTextureId;

			unsigned int _textureId;
			std::string _path;
			TextureFilter _filter;

		public:
			Texture() = default;
			~Texture() = default;

			// Static method to load texture and return texture ID
			// Returns the texture ID that can be used with OpenGL
			static unsigned int Load(const std::string& path, TextureFilter filter = TextureFilter::NEAREST);

			// Load texture and store it in this instance
			void LoadTexture(const std::string& path, TextureFilter filter = TextureFilter::NEAREST);

			// Get the current filtering mode
			TextureFilter GetFilter() const { return _filter; }

			// Bind the texture for rendering
			void Bind() const;

			// Get the texture ID
			unsigned int GetTextureId() const { return _textureId; }

			// Get the texture path
			const std::string& GetPath() const { return _path; }

			// Clear the texture cache (useful for cleanup)
			static void ClearCache();

			// Get cache statistics
			static size_t GetCacheSize();
		};
	}
}
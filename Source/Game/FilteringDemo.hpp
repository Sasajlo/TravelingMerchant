#pragma once

#include <Core/Component.hpp>
#include <Core/Input.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/Texture.hpp>

namespace TM::Game
{
	class FilteringDemo : public Component
	{
	private:
		Sprite* _sprite;
		bool _isPixelPerfect = true;

	public:
		FilteringDemo(GameObject& gameObject) : Component(gameObject) {}
		~FilteringDemo() = default;

		void Start() override
		{
			_sprite = _gameObject.GetComponent<Sprite>();
			if (_sprite)
			{
				_sprite->SetPixelPerfect(_isPixelPerfect);
			}
		}

		void Update(float deltaTime) override
		{
			// Toggle filtering mode with 'F' key
			if (Input::GetKeyDown(Key::F))
			{
				_isPixelPerfect = !_isPixelPerfect;
				if (_sprite)
				{
					_sprite->SetPixelPerfect(_isPixelPerfect);
					std::cout << "Filtering mode: " << (_isPixelPerfect ? "Pixel Perfect" : "Smooth") << std::endl;
				}
			}
		}
	};
}

#pragma once

#include <iostream>
#include <Core/Component.hpp>
#include <Utils/Color.hpp>

using namespace TM::Utils;
using namespace TM::Core;

namespace TM
{
	namespace Core
	{
		class GameObject; // Forward declaration
	}

	namespace Graphics
	{
		class Camera : public Core::Component
		{
		private:
			static Camera* _mainCamera; // Static pointer to the main camera instance
			Color _backgroundColor = Color::RED; // RGBA clear color

		public:
			Camera(GameObject& gameObject) : Component(gameObject) { if (!_mainCamera) _mainCamera = this; }
			static Camera* GetMain() { return _mainCamera; }

			// Override component methods
			void Awake() override;
			void Start() override;
			void Update(float deltaTime) override;

			void SetBackgroundColor(Color color);
			Color GetBackgroundColor();
			void SetMainCamera();
		};
	}
}

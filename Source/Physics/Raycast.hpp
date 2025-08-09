#pragma once

#include <Utils/Vector3.hpp>
#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Graphics/Camera.hpp>

#include <Graphics/Core.hpp>

using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Utils;

namespace TM
{
	namespace Physics
	{
		class Raycast
		{
		public:
			struct Ray
			{
				Vector3 origin;
				Vector3 direction;
			};

			struct HitResult
			{
				GameObject* hitObject = nullptr;
				float distance = -1.0f;
				Vector3 hitPoint;
				bool hit = false;
			};

			// Convert screen coordinates to world ray
			static Ray ScreenPointToRay(float screenX, float screenY, int screenWidth, int screenHeight);

			// Check if ray intersects with a sprite (simplified as a quad)
			static HitResult RaycastSprite(const Ray& ray, GameObject* gameObject);

			// Perform raycast against all objects in scene
			static HitResult RaycastAll(const Ray& ray);

			// Get mouse ray and perform raycast
			static HitResult MouseRaycast();
		};
	}
}
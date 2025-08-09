#include "Raycast.hpp"
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <algorithm>

using namespace TM::Utils;
using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Physics;

Raycast::Ray Raycast::ScreenPointToRay(float screenX, float screenY, int screenWidth, int screenHeight)
{
	Camera* camera = Camera::GetMain();
	if (!camera) {
		return { Vector3::Zero, Vector3::Zero };
	}

	// Convert screen coordinates to normalized device coordinates (-1 to 1)
	float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * screenY) / screenHeight;

	// Create ray in clip space
	glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

	// Transform to eye space
	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::vec4 rayEye = glm::inverse(projection) * rayClip;
	rayEye.z = -1.0f;
	rayEye.w = 0.0f;

	// Transform to world space
	glm::mat4 view = camera->GetViewMatrix();
	glm::vec4 rayWorld = glm::inverse(view) * rayEye;
	glm::vec3 rayDir = glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));

	// Get camera position
	const Transform& cameraTransform = camera->GetGameObject().GetTransform();
	Vector3 cameraPos = cameraTransform.GetPosition();

	return { cameraPos, Vector3(rayDir.x, rayDir.y, rayDir.z) };
}

Raycast::HitResult Raycast::RaycastSprite(const Ray& rayWorld, GameObject* go)
{
	HitResult result;

	auto* sprite = go->GetComponent<Sprite>();
	if (!sprite) return result;

	// Model matrix
	const glm::mat4 M = go->_transform.GetModelMatrix();

	// Basis (columns) and scales along local X/Y in world space
	const glm::vec3 colX = glm::vec3(M[0]);  // scaled right
	const glm::vec3 colY = glm::vec3(M[1]);  // scaled up
	const glm::vec3 posW = glm::vec3(M * glm::vec4(0, 0, 0, 1)); // origin in world

	const float sx = glm::length(colX);
	const float sy = glm::length(colY);
	if (sx < 1e-6f || sy < 1e-6f) return result;

	const glm::vec3 rightDir = colX / sx;
	const glm::vec3 upDir = colY / sy;
	const glm::vec3 normal = glm::normalize(glm::cross(rightDir, upDir));

	// Ray-plane intersection
	const glm::vec3 ro = { rayWorld.origin.x, rayWorld.origin.y, rayWorld.origin.z };
	const glm::vec3 rd = glm::normalize(glm::vec3(rayWorld.direction.x, rayWorld.direction.y, rayWorld.direction.z));

	const float denom = glm::dot(rd, normal);
	if (std::abs(denom) < 1e-6f) return result; // parallel

	const float t = glm::dot(posW - ro, normal) / denom;
	if (t < 0.0f) return result; // behind camera

	const glm::vec3 hitW = ro + rd * t;

	// Project hit into local (u,v) in units of the unscaled quad
	const glm::vec3 diff = hitW - posW;
	const float u = glm::dot(diff, rightDir) / sx; // local x
	const float v = glm::dot(diff, upDir) / sy; // local y

	// Local bounds from pivot (matches Sprite::UpdateVertexData)
	const glm::vec2 pivot = sprite->GetPivot();
	const float left = -pivot.x;
	const float right = 1.0f - pivot.x;
	const float bottom = -pivot.y;
	const float top = 1.0f - pivot.y;

	if (u < left || u > right || v < bottom || v > top) return result;

	result.hit = true;
	result.hitObject = go;
	result.hitPoint = { hitW.x, hitW.y, hitW.z };
	result.distance = glm::length(hitW - ro);
	return result;
}

Raycast::HitResult Raycast::RaycastAll(const Ray& ray)
{
	HitResult closestHit;
	closestHit.distance = std::numeric_limits<float>::max();

	Scene* activeScene = SceneManager::GetActiveScene();
	if (!activeScene) return closestHit;

	// Get all active objects
	auto activeObjects = activeScene->GetActiveGameObjects();
	std::reverse(activeObjects.begin(), activeObjects.end());

	for (GameObject* gameObject : activeObjects)
	{
		// Skip objects without SpriteRenderer
		if (!gameObject->GetComponent<SpriteRenderer>() || gameObject->HasTag("Player")) continue;

		HitResult hit = RaycastSprite(ray, gameObject);
		if (hit.hit && hit.distance < closestHit.distance)
		{
			closestHit = hit;
		}
	}

	return closestHit;
}

Raycast::HitResult Raycast::MouseRaycast()
{
	Vector3 mousePos = Input::GetMousePosition();

	// Get window size (you might need to add this to Window class)
	int width = Engine::GetWindowSize().width; 
	int height = Engine::GetWindowSize().height;

	Ray ray = ScreenPointToRay(mousePos.x, mousePos.y, width, height);
	return RaycastAll(ray);
}
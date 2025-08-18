#include "Raycast.hpp"
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Graphics/ImageRenderer.hpp>
#include <Graphics/TextRenderer.hpp>
#include <algorithm>

#include <Game/Interactable.hpp>

using namespace TM::Utils;
using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Physics;
using namespace TM::Game;

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
	const glm::mat4 M = go->transform.GetModelMatrix();

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

Raycast::HitResult Raycast::RaycastUI(Vector3 mousePos, GameObject* gameObject)
{
	// Check children first (uncomment this if you want to check child UI elements)
	auto children = gameObject->GetChildren();
	std::reverse(children.begin(), children.end());
	for (auto& child : children)
	{
		auto result = RaycastUI(mousePos, child);
		if (result.hit) return result;
	}

	// Check if this is a UI element
	if (!gameObject->HasTag("UI") || !gameObject->IsActive()) {
		HitResult hit;
		return hit;
	}

	// Get the UI element's screen position and size
	Vector3 screenPos = gameObject->transform.GetWorldPosition(); // Use local position for UI
	Size size(0, 0);

	// Check for ImageRenderer
	if (auto* imageRenderer = gameObject->GetComponent<ImageRenderer>()) {
		size = imageRenderer->GetSize();
	}
	// Check for TextRenderer
	//else if (auto* textRenderer = gameObject->GetComponent<TextRenderer>()) {
	//	// Get text bounds (you might need to implement this in TextRenderer)
	//	size = textRenderer->GetSize(); // Assuming this exists
	//}
	else {
		HitResult hit;
		return hit;
	}

	// Account for pivot point
	Vector3 pivot = Vector3(0.5f, 0.5f, 0.0f); // Default pivot
	if (auto* imageRenderer = gameObject->GetComponent<ImageRenderer>()) {
		pivot = imageRenderer->GetPivot();
	}
	//else if (auto* textRenderer = gameObject->GetComponent<TextRenderer>()) {
	//	pivot = textRenderer->GetPivot();
	//}

	// Calculate actual bounds considering pivot
	float left = screenPos.x - (size.width * pivot.x);
	float right = screenPos.x + (size.width * (1.0f - pivot.x));
	float bottom = screenPos.y - (size.height * pivot.y);
	float top = screenPos.y + (size.height * (1.0f - pivot.y));

	// Check if mouse is within bounds
	if (mousePos.x >= left && mousePos.x <= right &&
		mousePos.y >= bottom && mousePos.y <= top)
	{
		HitResult uiHit;
		uiHit.ui = true;
		uiHit.hitPoint = { mousePos.x, mousePos.y, 0.0f };
		uiHit.hitObject = gameObject;
		uiHit.hit = true;
		return uiHit;
	}

	HitResult hit;
	return hit;
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
		if (!gameObject->IsActive() || gameObject->HasTag("UI")) continue;

		// Skip objects without SpriteRenderer
		if (!gameObject->GetComponent<Interactable>() || gameObject->HasTag("Player")) continue;

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

	// Get window size
	int width = Engine::GetWindowSize().width;
	int height = Engine::GetWindowSize().height;

	if (width == 0 || height == 0)
	{
		HitResult result;
		result.hit = false;
		return result;
	}

	// Check if mouse is over any UI element first
	Scene* activeScene = SceneManager::GetActiveScene();
	if (activeScene)
	{
		auto activeObjects = activeScene->GetActiveGameObjects();
		std::reverse(activeObjects.begin(), activeObjects.end());

		for (auto gameObject : activeObjects)
		{
			if (!gameObject->IsActive()) continue;

			auto result = RaycastUI(mousePos, gameObject);
			if (result.hit) return result;
		}
	}

	// If no UI element is hit, do normal world raycast
	Ray ray = ScreenPointToRay(mousePos.x, mousePos.y, width, height);
	return RaycastAll(ray);
}
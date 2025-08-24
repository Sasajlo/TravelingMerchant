#pragma once

#include <iostream>
#include <Core/GameObject.hpp>
#include <Core/SceneManager.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Utils/Math.hpp>
#include <Game/Billboard.hpp>
#include <Game/Inventory/Inventory.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Game
    {
        class Player;

        class Interactable : public Component
        {
        protected:
            bool _hovered = false;
            SpriteRenderer* _spriteRenderer;
            bool _valid = true;

        public:
            Interactable(GameObject& gameObject) : Component(gameObject) {}

            void SetHovered(bool h) { _hovered = h; if (_spriteRenderer) _spriteRenderer->SetHovered(h); }
            bool IsHovered() const { return _hovered; }

            virtual void StartInteraction(Player* player);
            virtual void Interact(Player* player);
            virtual bool IsValid() const { return _valid; }

            void SpawnLogs();
            void SpawnRocks();
            
            void Awake() override
            {
                _spriteRenderer = _gameObject.GetComponent<SpriteRenderer>();
            }

            virtual std::string GetInteractionText()
            {
                if (_gameObject.HasTag("Tree"))
                {
                    return "Chop Tree";
                }
                else if (_gameObject.HasTag("Log"))
                {
                    return "Collect Log";
                }
                else if (_gameObject.HasTag("Stone"))
                {
                    return "Mine Stone";
                }
                else if (_gameObject.HasTag("Rocks"))
                {
                    return "Collect Rocks";
                }
                else if (_gameObject.HasTag("Berry Bush"))
                {
                    return "Harvest Berries";
                }
                else if (_gameObject.HasTag("Plant"))
                {
                    return "Collect Herb";
                }
                else if (_gameObject.HasTag("Slime"))
                {
                    return "Attack Slime";
                }

                return "Interact with " + _gameObject.GetName();
            }

            virtual float GetDistance()
            {
                if (_gameObject.HasTag("Tree"))
                {
                    return 1.0f;
                }
                else if (_gameObject.HasTag("Log"))
                {
                    return 0.3f;
                }
                else if (_gameObject.HasTag("Stone"))
                {
                    return 1.0f;
                }
                else if (_gameObject.HasTag("Rocks"))
                {
                    return 0.3f;
                }
                else if (_gameObject.HasTag("Berry Bush"))
                {
                    return 0.6f;
                }
                else if (_gameObject.HasTag("Plant"))
                {
                    return 0.3f;
                }
                else if (_gameObject.HasTag("Slime"))
                {
                    return 1.0f;
                }

                return 1.0f;
            }
        };
    }
}
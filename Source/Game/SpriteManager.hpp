#pragma once

#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <Core/GameObject.hpp>
#include <Graphics/Graphics.hpp>
#include <Utils/Utils.hpp>

namespace TM
{
    namespace Game
    {
        struct AnimationState
        {
            std::string name;
            std::string spritesheetPath;
            int columns;
            int rows;
            float frameRate;
            bool loop;

            // Add default constructor
            AnimationState() : name(""), spritesheetPath(""), columns(1), rows(1), frameRate(12.0f), loop(false) {}

            // Parameterized constructor
            AnimationState(const std::string& animName, const std::string& path, int cols, int rws, float fps, bool loop=false)
                : name(animName), spritesheetPath(path), columns(cols), rows(rws), frameRate(fps), loop(loop) {
            }
        };

        class SpriteManager : public Component
        {
        private:
            Sprite* _sprite = nullptr;
            Camera* _camera = nullptr;
            float _animationLifeTime = 0.0f;
            bool _isFinished = false;

            const int DOWN = 0;
            const int UP = 1;
            const int RIGHT = 2;
            const int LEFT = 3;

            Vector3 _lastDirection = { 0.0f, 0.0f, 1.0f };
            int _direction = DOWN;

            // Animation states storage
            std::map<std::string, AnimationState> _animationStates;
            std::string _currentAnimation = "";
            std::string _defaultAnimation = "";

            // Animation transitions
            std::map<std::string, std::vector<std::pair<std::string, std::string>>> _transitions;
            std::map<std::string, bool> _triggers;
            std::map<std::string, bool> _booleans;

            void UpdateDirection()
            {
                Vector3 cameraPosition = _camera->GetGameObject()->transform.position;
                Vector3 toPlayer = _gameObject.transform.GetWorldPosition() - cameraPosition;
                toPlayer.y = 0;
                toPlayer = toPlayer.Normalized();

                // Get angle in radians
                float angleRad = acos(std::clamp(toPlayer * _lastDirection, -1.0f, 1.0f));

                // Convert to degrees if needed
                float angleDeg = glm::degrees(angleRad);
                //if (_gameObject.GetName() == "Slime 0") std::cout << angleDeg << std::endl;

                int newDirection = -1;

                if (angleDeg < 45) newDirection = UP;
                else if (angleDeg > 135) newDirection = DOWN;
                else 
                {
                    float sign = _lastDirection.Cross(toPlayer).y;
                    if (sign < 0) newDirection = LEFT;
                    else newDirection = RIGHT;
                }

                if (newDirection != -1 && newDirection != _direction) 
                {
                    _sprite->SetAnimationOffset(newDirection);
                    _direction = newDirection;
                }
            }

        public:
            SpriteManager(GameObject& gameObject) : Component(gameObject) {}

            void Awake() override
            {
                _sprite = _gameObject.GetComponent<Sprite>();
                _camera = Camera::GetMain();
            }

            void Start() override
            {
                PlayDefaultAnimation();
            }

            void Update(float deltaTime) override
            {
                _isFinished = _sprite->IsFinished();
                UpdateDirection();
                CheckTransitions();

            }

            // Add a new animation state
            void AddAnimationState(const std::string& name, const std::string& spritesheetPath,
                int columns, int rows, float frameRate, bool loop=false)
            {
                if (_animationStates.size() == 0) _defaultAnimation = name;
                _animationStates[name] = AnimationState(name, spritesheetPath, columns, rows, frameRate, loop);
            }

            void PlayDefaultAnimation()
            {
                if (_defaultAnimation == "") return;
                PlayAnimation(_defaultAnimation);
            }

            // Play animation by name
            void PlayAnimation(const std::string& name)
            {
                auto it = _animationStates.find(name);
                if (it != _animationStates.end())
                {
                    const AnimationState& state = it->second;
                    _sprite->SetSpriteSheet(state.spritesheetPath, state.columns, state.rows);
                    _sprite->SetFrameRate(state.frameRate);
                    _sprite->SetLooping(state.loop);
                    //_sprite->Play();
                    _currentAnimation = name;
                }
                else
                {
                    std::cout << "Warning: Animation '" << name << "' not found!" << std::endl;
                }
            }

            void SetDirection(Vector3 direction)
            {
                _lastDirection = direction;
            }

            // Get current animation name
            std::string GetCurrentAnimation() const
            {
                return _currentAnimation;
            }

            // Check if animation exists
            bool HasAnimation(const std::string& name) const
            {
                return _animationStates.find(name) != _animationStates.end();
            }

            void AddTransition(const std::string& startState, const std::string& endState, const std::string& trigger = "")
            {
                _transitions[startState].push_back({ endState, trigger });
            }

            void CheckTransitions()
            {
                if (_currentAnimation == "") return;

                auto transitions = _transitions[_currentAnimation];
                for (auto& transition : transitions)
                {
                    bool inverted = transition.second[0] == '!';
                    std::string condition = inverted ? transition.second.substr(1) : transition.second;
                   
                    if (condition == "") // No condition, check if animation is finished
                    {
                        if (_sprite->IsFinished())
                        {
                            PlayAnimation(transition.first);
                            CheckTransitions();
                            break;
                        }
                        continue;
                    }

                    if (_booleans[condition] ^ inverted || _triggers[condition])
                    {
                        PlayAnimation(transition.first);
                        CheckTransitions();
                        break;
                    }
                }

                ResetTrigers();
            }

            void SetTrigger(const std::string& name)
            {
                _triggers[name] = true;
            }

            void ResetTrigers()
            {
                for (auto& trigger : _triggers)
                {
                    trigger.second = false;
                }
            }

            void SetBool(const std::string& name, bool value)
            {
                _booleans[name] = value;
            }

            float GetAnimationLifeTime() { return _sprite->GetAnimationLifeTime(); }

            bool IsFinished() const { return _isFinished; }
        };
    }
}
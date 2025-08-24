#pragma once

#include <iostream>
#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Graphics/Camera.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Game
    {
		const int MAX_LEVEL = 10;
		const int LEVELS[] = { 0, 100, 300, 600, 1000, 1500, 2100, 2800, 3600, 4500 };
		const int HEALHT[] = { 0, 100, 300, 600, 1000, 1500, 2100, 2800, 3600, 4500 };
		const int MANA[] = { 0, 150, 450, 900, 1500, 2250, 3150, 4200, 5400, 6750 };
		const int HUNGER[] = { 0, 100, 300, 600, 1000, 1500, 2100, 2800, 3600, 4500 };

        class PlayerStats : public Component
        {
        private:
			int _level = 1;
			int _experience = 0;

			int _health = 100;
			int _mana = 100;
			int _hunger = 100;

        public:
            PlayerStats(GameObject& gameObject) : Component(gameObject) {}

			void AddHealth(int amount) { _health = std::min(_health + amount, GetMaxHealth()); }
			void RemoveHealth(int amount) { _health = std::max(_health - amount, 0); }
			int GetHealth() const { return _health; }
			int GetMaxHealth() const { return HEALHT[_level - 1]; }

			void AddMana(int amount) { _mana = std::min(_mana + amount, GetMaxMana()); }
			void RemoveMana(int amount) { _mana = std::max(_mana - amount, 0); }
			int GetMana() const { return _mana; }
			int GetMaxMana() const { return MANA[_level - 1];; }

			void AddHunger(int amount) { _hunger = std::min(_hunger + amount, GetMaxHunger()); }
			void RemoveHunger(int amount) { _hunger = std::max(_hunger - amount, 0); }
			int GetHunger() const { return _hunger; }
			int GetMaxHunger() const { return HUNGER[_level - 1]; }

			void AddExperience(int amount) { 
				_experience += amount;
				if (_experience >= LEVELS[_level - 1])
					LevelUp();
			}

			void LevelUp()
			{
				_experience = _experience - LEVELS[_level - 1];
				++_level;
			}
        };
    }
}
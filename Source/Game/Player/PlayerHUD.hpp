#pragma once

#include <iostream>
#include <Core/GameObject.hpp>
#include <Core/Engine.hpp>
#include <Core/Input.hpp>
#include <Graphics/ImageRenderer.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

const int BUTTONS_SIZE = 5;
const std::string BUTTONS[] = {
    "Assets/Textures/UI/Icons/Buttons/character_icon.png",
    "Assets/Textures/UI/Icons/Buttons/craft_icon.png",
    "Assets/Textures/UI/Icons/Buttons/spellbook_icon.png",
    "Assets/Textures/UI/Icons/Buttons/skills_icon.png",
    "Assets/Textures/UI/Icons/Buttons/quest_log_icon.png"
};

namespace TM
{
    namespace Game
    {
        class PlayerHUD : public Component
        {
        private:
            ImageRenderer* _healthBar = nullptr;
            ImageRenderer* _manaBar = nullptr;
			TextRenderer* _levelText = nullptr;

            void CreatePlayerStatsUI()
            {
                GameObject* playerStatsObject = GameObject::Create("Player Stats");
                playerStatsObject->transform.SetPosition(0.0f, 0.0f, 0.0f);

                // Create health and mana background
                {
                    GameObject* playerIconFrame = playerStatsObject->CreateChild("Player Health and Mana");
                    playerIconFrame->transform.SetPosition(90, 50.0f, 0.1f);
                    auto* iconImage = playerIconFrame->AddComponent<ImageRenderer>();
                    iconImage->SetImage("Assets/Textures/UI/Frames/health_and_mana_bar_1.png");
                    iconImage->SetSize(256, 73);
                    iconImage->SetPivot(0.0f, 0.0f);

                    // Create health bar
                    {
                        GameObject* healthBarObject = playerIconFrame->CreateChild("Health Bar");
                        healthBarObject->transform.SetPosition(37.0f, 17.0f, 0.0f);
                        _healthBar = healthBarObject->AddComponent<ImageRenderer>();
                        _healthBar->SetImage("Assets/Textures/UI/Foregrounds/health_bar_1.png");
                        _healthBar->SetSize(210, 16);
                        _healthBar->SetPivot(0.0f, 0.0f);
					}

                    // Create mana bar
                    {
                        GameObject* manaBarObject = playerIconFrame->CreateChild("Mana Bar");
                        manaBarObject->transform.SetPosition(26.0f, 36.0f, 0.0f);
                        _manaBar = manaBarObject->AddComponent<ImageRenderer>();
                        _manaBar->SetImage("Assets/Textures/UI/Foregrounds/mana_bar_1.png");
                        _manaBar->SetSize(203, 14);
                        _manaBar->SetPivot(0.0f, 0.0f);
                    }
                }

                // Create player icon background
                {
                    GameObject* iconBackground = playerStatsObject->CreateChild("Player Icon Background");
                    iconBackground->transform.SetPosition(20.0f, 20.0f, 0.0f);
                    auto* iconHolderImage = iconBackground->AddComponent<ImageRenderer>();
                    iconHolderImage->SetImage("Assets/Textures/UI/Backgrounds/round_background_1.png");
                    iconHolderImage->SetSize(100, 100);
                    iconHolderImage->SetPivot(0.0f, 0.0f);
                }

                // Create player icon
                {
                    GameObject* playerIcon = playerStatsObject->CreateChild("Player Icon");
                    playerIcon->transform.SetPosition(20.0f, 20.0f, 0.0f);
                    auto* iconImage = playerIcon->AddComponent<ImageRenderer>();
                    iconImage->SetImage("Assets/Textures/UI/Foregrounds/player_icon.png");
                    iconImage->SetSize(100, 100);
                    iconImage->SetPivot(0.0f, 0.0f);
                }

                // Create player icon frame
                {
                    GameObject* playerIconFrame = playerStatsObject->CreateChild("Player Icon Frame");
                    playerIconFrame->transform.SetPosition(20.0f, 20.0f, 0.0f);
                    auto* iconImage = playerIconFrame->AddComponent<ImageRenderer>();
                    iconImage->SetImage("Assets/Textures/UI/Frames/round_frame_1.png");
                    iconImage->SetSize(100, 100);
                    iconImage->SetPivot(0.0f, 0.0f);
                }

                // Create level frame
                {
                    GameObject* levelFrameObject = playerStatsObject->CreateChild("Level Frame");
                    levelFrameObject->transform.SetPosition(95.0f, 105.0f, 0.0f);
                    auto* levelFrameImage = levelFrameObject->AddComponent<ImageRenderer>();
                    levelFrameImage->SetImage("Assets/Textures/UI/Frames/round_frame_3.png");
                    levelFrameImage->SetSize(30, 30);
                    levelFrameImage->SetPivot(0.0f, 0.0f);

                    // Create level text
                    {
                        GameObject* levelTextObject = levelFrameObject->CreateChild("Level Text");
                        levelTextObject->transform.SetPosition(15.0f, 15.0f, 0.0f);
                        _levelText = levelTextObject->AddComponent<TextRenderer>();
                        _levelText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 10.0f);
                        _levelText->SetPivot(0.5f, 0.5f);
                        _levelText->SetText("1");
                    }
				}
            }

            void CreateMinimapUI()
            {
                GameObject* minimapObject = GameObject::Create("Minimap");
                minimapObject->transform.SetPosition(Engine::GetWindowSize().width - 20.0f, 20.0f, 0.0f);
                auto* minimapImage = minimapObject->AddComponent<ImageRenderer>();
                minimapImage->SetImage("Assets/Textures/UI/Backgrounds/round_background_1.png");
                minimapImage->SetSize(180, 180);
                minimapImage->SetPivot(1.0f, 0.0f);

                // Create minimap frame
                {
                    GameObject* minimapFrame = minimapObject->CreateChild("Minimap Frame");
                    minimapFrame->transform.SetPosition(-90.0f, 90.0f, 0.0f);
                    auto* frameImage = minimapFrame->AddComponent<ImageRenderer>();
                    frameImage->SetImage("Assets/Textures/UI/Frames/round_frame_2.png");
                    frameImage->SetSize(200, 200);
                    frameImage->SetPivot(0.5f, 0.5f);
                }

                // Create map button
                {
                    GameObject* minimapFrame = minimapObject->CreateChild("Map Button Background");
                    minimapFrame->transform.SetPosition(-50.0f, 150.0f, 0.0f);
                    auto* frameImage = minimapFrame->AddComponent<ImageRenderer>();
                    frameImage->SetImage("Assets/Textures/UI/Frames/round_frame_3.png");
                    frameImage->SetSize(50, 50);
                    frameImage->SetPivot(0.0, 0.0f);

                    // Create map icon
                    {
                        GameObject* minimapIcon = minimapFrame->CreateChild("Map Button Icon");
                        minimapIcon->transform.SetPosition(5.0f, 5.0f, 0.0f);
                        auto* iconImage = minimapIcon->AddComponent<ImageRenderer>();
                        iconImage->SetImage("Assets/Textures/UI/Icons/Buttons/map_icon.png");
                        iconImage->SetSize(40, 40);
                        iconImage->SetPivot(0.0, 0.0f);
                    }
                }
			}

            void CreateActionBarUI()
            {
                GameObject* actionBarObject = GameObject::Create("Action Bar");
                actionBarObject->transform.SetPosition(Engine::GetWindowSize().width * 0.5f, Engine::GetWindowSize().height + 15.0f, 0.0f);

				// Create action bars
                {
                    // Create spell action bar
                    {
                        GameObject* actionBar = actionBarObject->CreateChild("Spell Action Bar");
                        actionBar->transform.SetPosition(-10.0f, -15.0f, 0.0f);
                        auto* backgroundImage = actionBar->AddComponent<ImageRenderer>();
                        backgroundImage->SetImage("Assets/Textures/UI/Backgrounds/action_bar_background_1.png");
                        backgroundImage->SetSize(520.0f, 70.0f);
                        backgroundImage->SetPivot(1.0f, 1.0f);

                        // Create slots
                        for (int i = 0; i < 10; i++)
                        {
                            // Create slot background
                            {
                                GameObject* slotBackground = actionBar->CreateChild("Slot " + std::to_string(i));
                                slotBackground->transform.SetPosition(-i*50.0f - 10.0f, -10.0f, 0.0f);
                                auto* slotBackgroundImage = slotBackground->AddComponent<ImageRenderer>();
                                slotBackgroundImage->SetImage("Assets/Textures/UI/Backgrounds/slot_background_2.png");
                                slotBackgroundImage->SetSize(50.0f, 50.0f);
                                slotBackgroundImage->SetPivot(1.0f, 1.0f);
                            }

                            // Create slot frame
                            {
                                GameObject* slotBackground = actionBar->CreateChild("Slot Frame" + std::to_string(i));
                                slotBackground->transform.SetPosition(-i * 50.0f - 10.0f, -10.0f, 0.0f);
                                auto* slotBackgroundImage = slotBackground->AddComponent<ImageRenderer>();
                                slotBackgroundImage->SetImage("Assets/Textures/UI/Frames/slot_frame_1.png");
                                slotBackgroundImage->SetSize(50.0f, 50.0f);
                                slotBackgroundImage->SetPivot(1.0f, 1.0f);
                            }
                        }
                    }

                    // Create buttons action bar
                    {
                        GameObject* actionBar = actionBarObject->CreateChild("Buttons Action Bar");
                        actionBar->transform.SetPosition(-10.0f, -15.0f, 0.0f);
                        auto* backgroundImage = actionBar->AddComponent<ImageRenderer>();
                        backgroundImage->SetImage("Assets/Textures/UI/Backgrounds/action_bar_background_1.png");
                        backgroundImage->SetSize(270.0f, 70.0f);
                        backgroundImage->SetPivot(0.0f, 1.0f);

                        // Create buttons
                        for (int i = 0; i < BUTTONS_SIZE; i++)
                        {
                            // Create button icon
                            {
                                GameObject* buttonIcon = actionBar->CreateChild("Slot Frame" + std::to_string(i));
                                buttonIcon->transform.SetPosition(i * 50.0f + 15.0f, -15.0f, 0.0f);
                                auto* buttonIconImage = buttonIcon->AddComponent<ImageRenderer>();
                                buttonIconImage->SetImage(BUTTONS[i]);
                                buttonIconImage->SetSize(40.0f, 40.0f);
                                buttonIconImage->SetPivot(0.0f, 1.0f);
                            }
                        }
                    }

                    // Create bags action bar
                    {
                        GameObject* actionBar = actionBarObject->CreateChild("Bags Action Bar");
                        actionBar->transform.SetPosition(260.0f, -15.0f, 0.0f);
                        auto* backgroundImage = actionBar->AddComponent<ImageRenderer>();
                        backgroundImage->SetImage("Assets/Textures/UI/Backgrounds/action_bar_background_1.png");
                        backgroundImage->SetSize(270.0f, 70.0f);
                        backgroundImage->SetPivot(0.0f, 1.0f);

                        // Create slots
                        for (int i = 0; i < 5; i++)
                        {
                            // Create slot background
                            {
                                GameObject* slotBackground = actionBar->CreateChild("Bag Slot " + std::to_string(i));
                                slotBackground->transform.SetPosition(i * 50.0f + 10.0f, -10.0f, 0.0f);
                                auto* slotBackgroundImage = slotBackground->AddComponent<ImageRenderer>();
                                slotBackgroundImage->SetImage("Assets/Textures/UI/Backgrounds/slot_background_3.png");
                                slotBackgroundImage->SetSize(50.0f, 50.0f);
                                slotBackgroundImage->SetPivot(0.0f, 1.0f);
                            }

                            if (i == 4)
                            {
                                GameObject* bagIcon = actionBar->CreateChild("Bag Slot Icon" + std::to_string(i));
                                bagIcon->transform.SetPosition(i * 50.0f + 15.0f, -15.0f, 0.0f);
                                auto* bagIconImage = bagIcon->AddComponent<ImageRenderer>();
                                bagIconImage->SetImage("Assets/Textures/UI/Icons/Buttons/bag_icon_2.png");
                                bagIconImage->SetSize(40.0f, 40.0f);
                                bagIconImage->SetPivot(0.0f, 1.0f);
                            }

                            // Create slot frame
                            {
                                GameObject* slotBackground = actionBar->CreateChild("Bag Slot Frame" + std::to_string(i));
                                slotBackground->transform.SetPosition(i * 50.0f + 10.0f, -10.0f, 0.0f);
                                auto* slotBackgroundImage = slotBackground->AddComponent<ImageRenderer>();
                                slotBackgroundImage->SetImage("Assets/Textures/UI/Frames/slot_frame_1.png");
                                slotBackgroundImage->SetSize(50.0f, 50.0f);
                                slotBackgroundImage->SetPivot(0.0f, 1.0f);
                            }
                        }
                    }
                }

                // Create XP bar
                {
                    // Create XP bar background
                    {
                        GameObject* xpBarBackground = actionBarObject->CreateChild("XP Bar Background");
                        xpBarBackground->transform.SetPosition(0.0f, -84.0f, 0.0f);
                        auto* backgroundImage2 = xpBarBackground->AddComponent<ImageRenderer>();
                        backgroundImage2->SetImage("Assets/Textures/UI/Backgrounds/xp_background.png");
                        backgroundImage2->SetSize(1062.0f, 20.0f);
                        backgroundImage2->SetPivot(0.5f, 1.0f);
					}

                    // Create XP bar
                    {
                        GameObject* xpBarBackground = actionBarObject->CreateChild("XP Bar");
                        xpBarBackground->transform.SetPosition(-531.0f, -84.0f, 0.0f);
                        auto* backgroundImage2 = xpBarBackground->AddComponent<ImageRenderer>();
                        backgroundImage2->SetImage("Assets/Textures/UI/Foregrounds/xp_bar.png");
                        backgroundImage2->SetSize(130.0f, 20.0f);
                        backgroundImage2->SetPivot(0.0f, 1.0f);
                    }

                    // Create XP bar frame
                    {
                        GameObject* xpBarFrame = actionBarObject->CreateChild("XP Bar Frame");
                        xpBarFrame->transform.SetPosition(0.0f, -84.0f, 0.0f);
                        auto* backgroundImage2 = xpBarFrame->AddComponent<ImageRenderer>();
                        backgroundImage2->SetImage("Assets/Textures/UI/Frames/xp_frame.png");
                        backgroundImage2->SetSize(1062.0f, 20.0f);
                        backgroundImage2->SetPivot(0.5f, 1.0f);
                    }
                }

				// Create decorations
                {
                    // Create action bar left decoration
                    {
                        GameObject* actionBarDecoration = actionBarObject->CreateChild("Action Bar Left Decoration");
                        actionBarDecoration->transform.SetPosition(-530.0f, -15.0f, 0.0f);
                        auto* decorationImage = actionBarDecoration->AddComponent<ImageRenderer>();
                        decorationImage->SetImage("Assets/Textures/UI/Decorations/pattern.png");
                        decorationImage->SetSize(-90.0f, 90.0f);
                        decorationImage->SetPivot(0.0f, 1.0f);
				    }

                    // Create action bar right decoration
                    {
                        GameObject* actionBarDecoration = actionBarObject->CreateChild("Action Bar Left Decoration");
                        actionBarDecoration->transform.SetPosition(530.0f, -15.0f, 0.0f);
                        auto* decorationImage = actionBarDecoration->AddComponent<ImageRenderer>();
                        decorationImage->SetImage("Assets/Textures/UI/Decorations/pattern.png");
                        decorationImage->SetSize(90.0f, 90.0f);
                        decorationImage->SetPivot(0.0f, 1.0f);
                    }
                }
			}

        public:
            PlayerHUD(GameObject& gameObject) : Component(gameObject) {}
            
            void Awake() override
            {
				CreatePlayerStatsUI();
				CreateMinimapUI();
                CreateActionBarUI();
			}
        };
    }
}
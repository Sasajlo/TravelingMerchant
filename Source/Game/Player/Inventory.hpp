#pragma once

#include <iostream>
#include <vector>

#include <Core/Core.hpp>
#include <Graphics/Graphics.hpp>
#include <Physics/Physics.hpp>

#define SLOT_SEGMENT 90
#define SLOT_SIZE 70
#define SLOT_PADDING 30

#define ICON_SIZE 60
#define ICON_PADDING 35

#define TEXT_PADDING 35

namespace TM
{
    namespace Game
    {
        struct Item 
        {
            std::string name = "";
            std::string iconPath = "";
            int amount = 0;
        };

        class Slot 
        {
        private:
            Item _item;
            ImageRenderer* _itemIcon = nullptr;
            TextRenderer* _amountText = nullptr;

        public:
            Slot(ImageRenderer* icon, TextRenderer* amountText) : _itemIcon(icon), _amountText(amountText) {}

            void SetItem(Item item) 
            { 
                _item = item; 
                _itemIcon->SetImage(item.iconPath);
                _itemIcon->SetSize(ICON_SIZE, ICON_SIZE);
                _amountText->SetText(std::to_string(_item.amount));
            }

            Item GetItem()
            {
                return _item;
            }

            void Show() 
            {
                _itemIcon->SetActive(true);
                _amountText->SetActive(true);
            }

            void Hide()
            {
                _itemIcon->SetActive(false);
                _amountText->SetActive(false);
            }

            bool IsHovered()
            {
                return _itemIcon->IsMouseOver();
            }
        };

        class Inventory : public Component
        {
        public:
            Inventory(GameObject& gameObject) : Component(gameObject) {}

            // References
            GameObject* _inventory = nullptr;
            GameObject* _equipment = nullptr;
            GameObject* _bag = nullptr;

            TextRenderer* _cursorText = nullptr;

            bool _opened = false;

            // Slots
            int _inventorySize = 18;
            std::vector<Slot> _slots;

            void Awake() override
            {
                _inventory = GameObject::Find("Inventory");
                _equipment = GameObject::Find("Equipment");
                _bag = GameObject::Find("Bag");

                _cursorText = GameObject::Find("Cursor Text")->GetComponent<TextRenderer>();
            }

            void Start() override
            {
                _inventory->SetActive(_opened);

                CreateSlots();
            }

            void CreateSlots()
            {
                for (int y = 0; y < 3; y++)
                {
                    for (int x = 0; x < 6; x++)
                    {
                        // Convert 2D index to 1D
                        int index = y * 6 + x + 1;

                        GameObject* slotObject = _bag->CreateChild("Slot " + index);
                        slotObject->transform.SetPosition(x * SLOT_SEGMENT + SLOT_PADDING + 10, y * SLOT_SEGMENT + SLOT_PADDING - 155, 0.0f);
                        auto* slotImage = slotObject->AddComponent<ImageRenderer>();
                        slotImage->SetImage("Assets/Textures/bag_slot.png");
                        slotImage->SetSize(SLOT_SIZE, SLOT_SIZE);
                        slotImage->SetPivot(0.0f, 0.0f);

                        GameObject* iconObject = _bag->CreateChild("Slot Icon " + index);
                        iconObject->transform.SetPosition(x * SLOT_SEGMENT + ICON_PADDING + 10, y * SLOT_SEGMENT + ICON_PADDING - 155, 0.0f);
                        auto* iconImage = iconObject->AddComponent<ImageRenderer>();
                        iconImage->SetSize(ICON_SIZE, ICON_SIZE);
                        iconImage->SetPivot(0.0f, 0.0f);
                        iconImage->SetActive(false);

                        GameObject* textObject = _bag->CreateChild("Item Count Text " + index);
                        textObject->transform.SetPosition(x * SLOT_SEGMENT + TEXT_PADDING + 10, y * SLOT_SEGMENT + TEXT_PADDING - 155, 0.0f);
                        auto* amountText = textObject->AddComponent<TextRenderer>();
                        amountText->SetFont("Assets/Fonts/LibertinusSerif-Regular.ttf", 26.0f);
                        amountText->SetText("0");
                        amountText->SetPivot(0.0f, 0.0f);
                        amountText->SetActive(false);

                        Slot newSlot(iconImage, amountText);
                        _slots.push_back(newSlot);
                    }
                }
            }

            void Update(float deltaTime) override
            {
                if (Input::IsKeyPressed(GLFW_KEY_B)) {
                    _opened = !_opened;
                    _inventory->SetActive(_opened);
                }
                else if (Input::IsKeyPressed(GLFW_KEY_ESCAPE) && _opened) {
                    _opened = false;
                    _inventory->SetActive(false);
                }

                if (_opened) 
                {
                    for (auto slot : _slots) {
                        if (!slot.IsHovered()) continue;
                        auto item = slot.GetItem();
                        _cursorText->SetText(item.name + " x" + std::to_string(item.amount));
                        _cursorText->SetActive(true);
                    }
                }
            }

            int AddItem(Item newItem) 
            {
                newItem.amount = newItem.amount == 0 ? 1 : newItem.amount; // Ensure that amount is minimum 1

                for (auto& slot : _slots)
                {
                    Item slotItem = slot.GetItem();
                    if (slotItem.name == newItem.name)
                    {
                        slotItem.amount += newItem.amount;
                        slot.SetItem(slotItem);
                        return newItem.amount;
                    }

                    if (slot.GetItem().amount != 0) continue;
                    slot.SetItem(newItem);
                    slot.Show();
                    return newItem.amount;
                }

                return 0;
            }
        };
    }
}
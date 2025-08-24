# Sprite and Image Filtering System

This document explains how to use the new filtering system that allows you to choose between pixel-perfect and smooth rendering for both sprites and images.

## Overview

The filtering system provides two rendering modes:
- **Pixel Perfect (NEAREST)**: Maintains sharp, crisp edges for pixel art and retro-style graphics
- **Smooth (LINEAR)**: Provides anti-aliased, smooth edges for modern graphics

## Usage

### Setting Filter Mode on Sprites

```cpp
// Get a sprite component
Sprite* sprite = gameObject->GetComponent<Sprite>();

// Set pixel-perfect rendering (default)
sprite->SetPixelPerfect(true);
// or
sprite->SetFilterMode(TextureFilter::NEAREST);

// Set smooth rendering
sprite->SetPixelPerfect(false);
// or
sprite->SetFilterMode(TextureFilter::LINEAR);

// Check current mode
bool isPixelPerfect = sprite->IsPixelPerfect();
TextureFilter currentFilter = sprite->GetFilterMode();
```

### Setting Filter Mode on Images (UI)

```cpp
// Get an image renderer component
ImageRenderer* image = gameObject->GetComponent<ImageRenderer>();

// Set pixel-perfect rendering (default)
image->SetPixelPerfect(true);
// or
image->SetFilterMode(TextureFilter::NEAREST);

// Set smooth rendering
image->SetPixelPerfect(false);
// or
image->SetFilterMode(TextureFilter::LINEAR);

// Check current mode
bool isPixelPerfect = image->IsPixelPerfect();
TextureFilter currentFilter = image->GetFilterMode();
```

### Setting Filter Mode on SpriteRenderers

```cpp
// Get a sprite renderer component
SpriteRenderer* renderer = gameObject->GetComponent<SpriteRenderer>();

// Set pixel-perfect rendering
renderer->SetPixelPerfect(true);
// or
renderer->SetFilterMode(TextureFilter::NEAREST);

// Set smooth rendering
renderer->SetPixelPerfect(false);
// or
renderer->SetFilterMode(TextureFilter::LINEAR);

// Check current mode
bool isPixelPerfect = renderer->IsPixelPerfect();
TextureFilter currentFilter = renderer->GetFilterMode();
```

### Loading Textures with Specific Filtering

```cpp
// Load texture with pixel-perfect filtering
unsigned int textureId = Texture::Load("path/to/texture.png", TextureFilter::NEAREST);

// Load texture with smooth filtering
unsigned int textureId = Texture::Load("path/to/texture.png", TextureFilter::LINEAR);

// Create texture instance with specific filtering
Texture texture;
texture.LoadTexture("path/to/texture.png", TextureFilter::LINEAR);
```

### Setting Texture Filtering at Runtime

```cpp
// Change filtering mode for an existing texture
Texture texture;
texture.LoadTexture("path/to/texture.png");
texture.SetFilter(TextureFilter::LINEAR); // Change to smooth rendering
```

## Examples

### Example 1: Pixel Art Character (Sprite)
```cpp
GameObject* player = GameObject::Create("Player");
Sprite* playerSprite = player->AddComponent<Sprite>();
playerSprite->SetTexture("Assets/Textures/player.png");
playerSprite->SetPixelPerfect(true); // Pixel-perfect for pixel art
```

### Example 2: Smooth UI Element (Image)
```cpp
GameObject* uiElement = GameObject::Create("UI Element");
ImageRenderer* uiImage = uiElement->AddComponent<ImageRenderer>();
uiImage->SetImage("Assets/Textures/ui_button.png");
uiImage->SetPixelPerfect(false); // Smooth for modern UI
```

### Example 3: Mixed Filtering in UI
```cpp
// Main UI container with smooth rendering
GameObject* container = GameObject::Create("UI Container");
ImageRenderer* containerImage = container->AddComponent<ImageRenderer>();
containerImage->SetImage("Assets/Textures/container_bg.png");
containerImage->SetPixelPerfect(false); // Smooth background

// UI frame with pixel-perfect rendering
GameObject* frame = container->CreateChild("Frame");
ImageRenderer* frameImage = frame->AddComponent<ImageRenderer>();
frameImage->SetImage("Assets/Textures/frame.png");
frameImage->SetPixelPerfect(true); // Pixel-perfect frame
```

### Example 4: Runtime Toggle for Sprites
```cpp
// In your game loop or input handler
if (Input::GetKeyDown(Key::F))
{
    Sprite* sprite = player->GetComponent<Sprite>();
    bool currentMode = sprite->IsPixelPerfect();
    sprite->SetPixelPerfect(!currentMode);
    std::cout << "Switched to " << (sprite->IsPixelPerfect() ? "pixel-perfect" : "smooth") << " mode" << std::endl;
}
```

### Example 5: Runtime Toggle for Images
```cpp
// In your game loop or input handler
if (Input::GetKeyDown(Key::G))
{
    ImageRenderer* image = uiElement->GetComponent<ImageRenderer>();
    bool currentMode = image->IsPixelPerfect();
    image->SetPixelPerfect(!currentMode);
    std::cout << "UI switched to " << (image->IsPixelPerfect() ? "pixel-perfect" : "smooth") << " mode" << std::endl;
}
```

## Technical Details

### Texture Caching
The system properly caches textures with different filter modes. Each texture path + filter combination is cached separately, so you can have the same texture loaded with both NEAREST and LINEAR filtering.

### Performance
- Pixel-perfect (NEAREST) filtering is generally faster
- Smooth (LINEAR) filtering may have a slight performance cost but provides better visual quality
- The filtering mode is set at the OpenGL texture level, so it affects all sprites/images using that texture

### Default Behavior
- All sprites and images default to pixel-perfect (NEAREST) filtering for backward compatibility
- This maintains the existing behavior of your game

## Best Practices

1. **Use pixel-perfect for pixel art**: Maintains the intended sharp, blocky appearance
2. **Use smooth for modern graphics**: Provides better visual quality for high-resolution textures
3. **Be consistent**: Use the same filtering mode for related elements (e.g., all UI elements)
4. **Consider performance**: Use pixel-perfect for frequently rendered elements if performance is critical
5. **Test both modes**: Some textures may look better with the opposite of what you'd expect
6. **UI considerations**: Modern UI elements often benefit from smooth filtering, while retro-style UI elements work better with pixel-perfect

## Migration from Existing Code

Existing code will continue to work without changes, as the default behavior is pixel-perfect filtering. To take advantage of the new system, simply add filtering mode calls where appropriate:

```cpp
// Before (existing code)
sprite->SetTexture("texture.png");
image->SetImage("texture.png");

// After (with explicit filtering)
sprite->SetTexture("texture.png", TextureFilter::NEAREST); // or LINEAR
image->SetImage("texture.png", TextureFilter::LINEAR); // or NEAREST

// or
sprite->SetTexture("texture.png");
sprite->SetPixelPerfect(true); // or false

image->SetImage("texture.png");
image->SetPixelPerfect(false); // or true
```

## Component Comparison

| Component | Use Case | Default Filter | Best For |
|-----------|----------|----------------|----------|
| **Sprite** | Game world objects | Pixel-perfect | Characters, items, world objects |
| **ImageRenderer** | UI elements | Pixel-perfect | Buttons, panels, HUD elements |
| **SpriteRenderer** | Rendering sprites | Pixel-perfect | Batch rendering, performance optimization |

All components support the same filtering interface for consistency across your codebase.

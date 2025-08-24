#pragma once

#include <iostream>
#include <Core/Component.hpp>
#include <Utils/Vector3.hpp>
#include <string>
#include <memory>
#include <fmod.hpp>
#include <fmod_errors.h>

namespace TM
{
    namespace Core
    {
        class GameObject; // Forward declaration
    }

    namespace Audio
    {
        class AudioSource : public Core::Component
        {
        public:
            enum class AudioType
            {
                TwoDimensional,  // UI sounds, music
                ThreeDimensional // Game world sounds
            };

        private:
            // Audio properties
            AudioType _audioType = AudioType::TwoDimensional;
            std::string _audioFilePath;
            float _volume = 1.0f;
            float _pitch = 1.0f;
            bool _isLooping = false;
            bool _isPlaying = false;
            bool _isPaused = false;

            // 3D audio properties
            float _minDistance = 1.0f;
            float _maxDistance = 100.0f;
            float _rolloffFactor = 1.0f;
            float _referenceDistance = 1.0f;

            // FMOD objects
            static FMOD::System* _system;
            static bool _isInitialized;

            FMOD::Sound* _sound = nullptr;
            FMOD::Channel* _channel = nullptr;

        public:
            AudioSource(Core::GameObject& gameObject);
            ~AudioSource();

            // Lifecycle methods
            void Awake() override;
            void Start() override;
            void Update(float deltaTime) override;
            void Destroy() override;

            // Audio control methods
            void Play();
            void Pause();
            void Stop();
            void Resume();

            // Audio setup methods
            void SetAudioFile(const std::string& filePath);
            void SetAudioType(AudioType type);
            void SetVolume(float volume);
            void SetPitch(float pitch);
            void SetLooping(bool looping);

            // 3D audio setup methods
            void SetMinDistance(float distance);
            void SetMaxDistance(float distance);
            void SetRolloffFactor(float factor);
            void SetReferenceDistance(float distance);

            // Getters
            bool IsPlaying() const { return _isPlaying; }
            bool IsPaused() const { return _isPaused; }
            AudioType GetAudioType() const { return _audioType; }
            float GetVolume() const { return _volume; }
            float GetPitch() const { return _pitch; }
            bool IsLooping() const { return _isLooping; }

            // Static initialization and cleanup
            static bool InitializeAudioSystem();
            static void CleanupAudioSystem();
            static bool IsAudioSystemInitialized() { return _isInitialized; }

        private:
            // Helper methods
            bool LoadAudioFile(const std::string& filePath);
            void Update3DPosition();
            void UpdateAudioProperties();
            void CleanupFMODObjects();

            static void LogFMOD(FMOD_RESULT r, const char* ctx)
            {
                if (r != FMOD_OK)
                    std::cerr << "[FMOD] " << ctx << " failed: " << FMOD_ErrorString(r) << " (" << r << ")" << std::endl;
            }
        };
    }
}
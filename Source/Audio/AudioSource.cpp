#include "AudioSource.hpp"
#include <Core/GameObject.hpp>
#include <Graphics/Camera.hpp>
#include <Utils/Vector3.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace TM
{
    namespace Audio
    {
        FMOD::System* AudioSource::_system = nullptr;
        bool AudioSource::_isInitialized = false;

        static FMOD_VECTOR ToFMODVec(const TM::Utils::Vector3& v)
        {
            FMOD_VECTOR fv;
            fv.x = v.x;
            fv.y = v.y;
            fv.z = v.z;
            return fv;
        }

        AudioSource::AudioSource(Core::GameObject& gameObject)
            : Core::Component(gameObject)
        {
        }

        AudioSource::~AudioSource()
        {
            CleanupFMODObjects();
        }

        void AudioSource::Awake()
        {
            if (!_isInitialized)
            {
                if (!InitializeAudioSystem())
                {
                    std::cerr << "Failed to initialize FMOD for AudioSource: " << _gameObject.GetName() << std::endl;
                    return;
                }
            }

            if (!_audioFilePath.empty())
            {
                LoadAudioFile(_audioFilePath);
            }
        }

        void AudioSource::Start()
        {
        }

        void AudioSource::Update(float deltaTime)
        {
            if (!_isInitialized || !_system) return;

            if (_audioType == AudioType::ThreeDimensional)
            {
                Update3DPosition();
            }

            _system->update();

            if (_channel && _isPlaying)
            {
                bool isPlaying = false;
                if (_channel->isPlaying(&isPlaying) == FMOD_OK && !isPlaying)
                {
                    _isPlaying = false;
                }
            }
        }

        void AudioSource::Destroy()
        {
            Stop();
            CleanupFMODObjects();
        }

        void AudioSource::Play()
        {
            if (!_system)
            {
                std::cerr << "Cannot play: FMOD system not available" << std::endl;
                return;
            }
            if (!_sound)
            {
                if (_audioFilePath.empty())
                {
                    std::cerr << "Cannot play: No audio file set" << std::endl;
                    return;
                }
                if (!LoadAudioFile(_audioFilePath))
                {
                    std::cerr << "Cannot play: Failed to load sound" << std::endl;
                    return;
                }
            }

            if (_channel)
            {
                _channel->stop();
                _channel = nullptr;
            }

            FMOD_RESULT r = _system->playSound(_sound, nullptr, false, &_channel);
            LogFMOD(r, "playSound");
            if (r != FMOD_OK) return;

            UpdateAudioProperties();
            if (_audioType == AudioType::ThreeDimensional)
            {
                Update3DPosition();
            }

            _isPlaying = true;
            _isPaused = false;
        }

        void AudioSource::Pause()
        {
            if (_channel)
            {
                _channel->setPaused(true);
                _isPaused = true;
                _isPlaying = false;
            }
        }

        void AudioSource::Stop()
        {
            if (_channel)
            {
                _channel->stop();
                _channel = nullptr;
            }
            _isPlaying = false;
            _isPaused = false;
        }

        void AudioSource::Resume()
        {
            if (_channel)
            {
                _channel->setPaused(false);
                _isPlaying = true;
                _isPaused = false;
            }
        }

        void AudioSource::SetAudioFile(const std::string& filePath)
        {
            _audioFilePath = filePath;
            if (_system)
            {
                LoadAudioFile(filePath);
            }
        }

        void AudioSource::SetAudioType(AudioType type)
        {
            _audioType = type;
            UpdateAudioProperties();
        }

        void AudioSource::SetVolume(float volume)
        {
            _volume = std::max(0.0f, std::min(1.0f, volume));
            UpdateAudioProperties();
        }

        void AudioSource::SetPitch(float pitch)
        {
            _pitch = std::max(0.1f, std::min(10.0f, pitch));
            UpdateAudioProperties();
        }

        void AudioSource::SetLooping(bool looping)
        {
            _isLooping = looping;
            UpdateAudioProperties();
        }

        void AudioSource::SetMinDistance(float distance)
        {
            _minDistance = std::max(0.0f, distance);
            UpdateAudioProperties();
        }

        void AudioSource::SetMaxDistance(float distance)
        {
            _maxDistance = std::max(_minDistance, distance);
            UpdateAudioProperties();
        }

        void AudioSource::SetRolloffFactor(float factor)
        {
            _rolloffFactor = std::max(0.0f, factor);
            if (_system)
            {
                _system->set3DSettings(1.0f, 1.0f, _rolloffFactor);
            }
        }

        void AudioSource::SetReferenceDistance(float distance)
        {
            _referenceDistance = std::max(0.0f, distance);
            SetMinDistance(_referenceDistance);
        }

        bool AudioSource::InitializeAudioSystem()
        {
            if (_isInitialized) return true;

            FMOD_RESULT r = FMOD::System_Create(&_system);
            LogFMOD(r, "System_Create");
            if (r != FMOD_OK) return false;

            r = _system->init(512, FMOD_INIT_NORMAL, nullptr);
            LogFMOD(r, "System::init");
            if (r != FMOD_OK) return false;

            _system->set3DSettings(1.0f, 1.0f, 1.0f);

            _isInitialized = true;
            std::cout << "FMOD audio system initialized" << std::endl;
            return true;
        }

        void AudioSource::CleanupAudioSystem()
        {
            if (!_isInitialized) return;

            if (_system)
            {
                _system->close();
                _system->release();
                _system = nullptr;
            }
            _isInitialized = false;
        }

        static inline bool ends_with_ci(const std::string& s, const char* suf)
        {
            size_t n = s.size(), m = std::char_traits<char>::length(suf);
            if (m > n) return false;
            for (size_t i = 0; i < m; ++i)
            {
                char a = (char)std::tolower((unsigned char)s[n - m + i]);
                char b = (char)std::tolower((unsigned char)suf[i]);
                if (a != b) return false;
            }
            return true;
        }

        bool AudioSource::LoadAudioFile(const std::string& filePath)
        {
            CleanupFMODObjects();

            if (!_system) return false;

            FMOD_MODE mode = FMOD_DEFAULT;
            mode |= (_audioType == AudioType::ThreeDimensional) ? FMOD_3D : FMOD_2D;
            mode |= _isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

            if (ends_with_ci(filePath, ".mp3") || ends_with_ci(filePath, ".ogg") || ends_with_ci(filePath, ".flac"))
            {
                mode |= FMOD_CREATESTREAM;
            }

            FMOD_RESULT r = _system->createSound(filePath.c_str(), mode, nullptr, &_sound);
            LogFMOD(r, "createSound");
            if (r != FMOD_OK)
            {
                _sound = nullptr;
                return false;
            }

            if (_audioType == AudioType::ThreeDimensional && _sound)
            {
                _sound->set3DMinMaxDistance(_minDistance, _maxDistance);
            }

            return true;
        }

        void AudioSource::Update3DPosition()
        {
            if (!_channel || !_system) return;

            auto sourcePos = _gameObject.transform.GetPosition();
            FMOD_VECTOR pos = ToFMODVec(sourcePos);
            FMOD_VECTOR vel{ 0.0f, 0.0f, 0.0f };

            _channel->set3DAttributes(&pos, &vel);

            auto* mainCamera = TM::Graphics::Camera::GetMain();
            if (!mainCamera) return;

            auto listenerPosV = mainCamera->GetGameObject()->transform.GetPosition();
            FMOD_VECTOR listenerPos = ToFMODVec(listenerPosV);
            FMOD_VECTOR listenerVel{ 0.0f, 0.0f, 0.0f };

            FMOD_VECTOR forward{ 0.0f, 0.0f, -1.0f };
            FMOD_VECTOR up{ 0.0f, 1.0f, 0.0f };

            _system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &forward, &up);
        }

        void AudioSource::UpdateAudioProperties()
        {
            if (_sound)
            {
                _sound->setMode(_isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

                if (_audioType == AudioType::ThreeDimensional)
                {
                    _sound->set3DMinMaxDistance(_minDistance, _maxDistance);
                }
            }

            if (_channel)
            {
                _channel->setVolume(_volume);
                _channel->setPitch(_pitch);
            }
        }

        void AudioSource::CleanupFMODObjects()
        {
            if (_channel)
            {
                _channel->stop();
                _channel = nullptr;
            }
            if (_sound)
            {
                _sound->release();
                _sound = nullptr;
            }
        }
    }
}


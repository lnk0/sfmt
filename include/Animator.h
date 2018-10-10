#ifndef SFMT_ANIMATOR_H
#define SFMT_ANIMATOR_H

#include <string>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

namespace sfmt {
    class Animator {
    public:
        struct Animation {
            std::string                 _name;
            std::string                 _textureName;
            std::vector<sf::IntRect>    _frames;
            sf::Time                    _duration;
            bool                        _isLoop;

            Animation(std::string const &name, std::string const &textureName, sf::Time const &duration, bool isLoop) :
                    _name(name), _textureName(textureName), _duration(duration), _isLoop(isLoop) {}

            void AddFrames(sf::Vector2i const &startFrom, sf::Vector2i const &frameSize, unsigned int frames) {
                sf::Vector2i current = startFrom;

                for (unsigned int frame = 0; frame < frames; ++frame) {
                    _frames.emplace_back(current.x, current.y, frameSize.x, frameSize.y);
                    current.x += frameSize.x;
                }
            }

        };

        explicit Animator(sf::Sprite &sprite) :
                _sprite(sprite), _currentTime(), _currentAnimation(nullptr) {}

        Animation &CreateAnimation(std::string const &name, std::string const &textureName, sf::Time const &duration, bool isLoop = false) {
            _animations.emplace_back(Animation(name, textureName, duration, isLoop));

            if (_currentAnimation == nullptr)
                SwitchAnimation(&_animations.back());

            return _animations.back();
        }

        void Update(sf::Time const& deltaTime) {
            if (_currentAnimation == nullptr)
                return;

            _currentTime += deltaTime;

            float scaledTime = _currentTime.asSeconds() / _currentAnimation->_duration.asSeconds();
            int frameNumber = _currentAnimation->_frames.size();
            int currentFrame = static_cast<int>(scaledTime * frameNumber);

            if (_currentAnimation->_isLoop)
                currentFrame %= frameNumber;
            else if (currentFrame >= frameNumber)
                currentFrame = frameNumber - 1;

            _sprite.setTextureRect(_currentAnimation->_frames[currentFrame]);
        }

        bool SwitchAnimation(std::string const& name) {
            auto animation = findAnimation(name);

            if (animation != nullptr) {
                SwitchAnimation(animation);
                return true;
            }

            return false;
        }

        std::string GetAnimationName() const {
            if (_currentAnimation != nullptr)
                return _currentAnimation->_name;

            return "";
        }

    private:
        sf::Sprite              &_sprite;
        sf::Time                _currentTime;
        std::list<Animation>    _animations;
        Animation               *_currentAnimation;

        Animation *findAnimation(std::string const &name) {

            for (auto &_animation : _animations) {
                if (_animation._name == name)
                    return &_animation;
            }

            return nullptr;
        }

        void SwitchAnimation(Animation *animation) {
            if (animation != nullptr)
                _sprite.setTexture(AssetManager::GetTexture(animation->_textureName));

            _currentAnimation = animation;
            _currentTime = sf::Time::Zero;
        }
    };
}

#endif //SFMT_ANIMATOR_H

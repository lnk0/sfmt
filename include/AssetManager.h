#ifndef SFMT_ASSETMANAGER_H
#define SFMT_ASSETMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <assert.h>
#include <map>

namespace sfmt {
    class AssetManager {
    public:
        AssetManager() {
            assert(_instance == nullptr);
            _instance = this;
        };

        static sf::Texture &GetTexture(std::string const &filename) {
            auto &textureMap = _instance->_textures;

            auto isTextureLoaded = textureMap.find(filename);
            if ( isTextureLoaded != textureMap.end())
                return isTextureLoaded->second;

            auto &texture = textureMap[filename];
            texture.loadFromFile(filename);
            return texture;
        }

        static sf::SoundBuffer &GetSoundBuffer(std::string const &filename) {
            auto &soundBufferMap = _instance->_soundBuffers;

            auto isSoundBufferLoaded = soundBufferMap.find(filename);
            if (isSoundBufferLoaded != soundBufferMap.end())
                return isSoundBufferLoaded->second;

            auto &soundBuffer = soundBufferMap[filename];
            soundBuffer.loadFromFile(filename);
            return soundBuffer;
        }

        static  sf::Font &GetFont(std::string const &filename) {
            auto &fontMap = _instance->_fonts;

            auto isFontLoaded = fontMap.find(filename);
            if (isFontLoaded != fontMap.end())
                return isFontLoaded->second;

            auto &font = fontMap[filename];
            font.loadFromFile(filename);
            return font;
        }

    private:
        std::map<std::string, sf::Texture>      _textures;
        std::map<std::string, sf::SoundBuffer>  _soundBuffers;
        std::map<std::string, sf::Font>         _fonts;

        static AssetManager                     *_instance;
    };

    AssetManager *AssetManager::_instance = nullptr;
}

#endif //SFMT_ASSETMANAGER_H

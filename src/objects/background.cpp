#include "background.hpp"

#include "game.hpp"
#include "primitive_sprite.hpp"
#include "asset_manager.hpp"
#include "settings.hpp"
#include "game.hpp"
#include "simple_object.hpp"
#include "background_object.hpp"
#include "randomizer.hpp"

Background::Background(size_t islands, size_t clouds, float additional_distance)
    : GameObject("background")
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("Background_0");
    sf::Vector2u tSize = texture.getSize();
    const sf::Vector2f &cSize = Game::getObjectManager().getCamera().getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture, sf::IntRect(0, 0, cSize.x, tSize.y));
    this->setScale(1, cSize.y / tSize.y);
    this->setOrigin(cSize.x / 2, tSize.y / 2);

    sf::Vector2f sunPos = {500 * cSize.x / 1280, -300 * cSize.y / 720};
    children.push_back(std::make_shared<SimpleObject>("sun", std::make_shared<PrimitiveSprite>(assetManager.getTexture("Sun")), sunPos));
    size_t firstClouds = randomizer::getRandInt(0, clouds);
    for (size_t i = 0; i < firstClouds; ++i)
    {
        std::string name = "cloud" + std::to_string(i);
        std::string textureName = "Cloud_" + std::to_string(randomizer::getRandInt(0, 40));
        sf::Vector2f startPos = {randomizer::getRandFloat(-cSize.x, cSize.x), randomizer::getRandFloat(-0.45 * cSize.y, 0 * cSize.y)};
        float distance = clouds / 10 + 1 + additional_distance;
        distance = randomizer::getRandFloat(distance - 0.1 * (i + 1), distance - 0.1 * i);
        float speed = randomizer::getRandFloat(0.01, 2);
        children.push_back(std::make_shared<BackgroundObject>(name, std::make_shared<PrimitiveSprite>(assetManager.getTexture(textureName)), startPos, speed, distance));
    }
    for (size_t i = 0; i < islands; ++i)
    {
        std::string name = "island" + std::to_string(i);
        std::string textureName = "bg_island_" + std::to_string(randomizer::getRandInt(1, 8));
        sf::Vector2f startPos = {randomizer::getRandFloat(-cSize.x, cSize.x), randomizer::getRandFloat(-0.45 * cSize.y, 0.45 * cSize.y)};
        float distance = islands / 10 + 2;
        distance = randomizer::getRandFloat(distance - 0.1 * (i + 1), distance - 0.1 * i);
        children.push_back(std::make_shared<BackgroundObject>(name, std::make_shared<PrimitiveSprite>(assetManager.getTexture(textureName)), startPos, 0.0f, distance));
    }
    for (size_t i = firstClouds; i < clouds; ++i)
    {
        std::string name = "cloud" + std::to_string(i);
        std::string textureName = "Cloud_" + std::to_string(randomizer::getRandInt(0, 40));
        sf::Vector2f startPos = {randomizer::getRandFloat(-cSize.x, cSize.x), randomizer::getRandFloat(-0.45 * cSize.y, 0 * cSize.y)};
        float distance = clouds / 10 + 1 + additional_distance;
        distance = randomizer::getRandFloat(distance - 0.1 * (i + 1), distance - 0.1 * i);
        float speed = randomizer::getRandFloat(0.01, 2);
        children.push_back(std::make_shared<BackgroundObject>(name, std::make_shared<PrimitiveSprite>(assetManager.getTexture(textureName)), startPos, speed, distance));
    }
}

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
    auto &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager->getTexture("background");
    sf::Vector2u tSize = texture.getSize();
    const sf::Vector2f &cSize = Game::getObjectManager()->getView().getSize();
    float cSize_x = cSize.x;
    float cSize_y = cSize.y;
    drawable = std::make_shared<PrimitiveSprite>(texture, sf::IntRect(0, 0, cSize.x, tSize.y));
    this->setScale(1, cSize.y / tSize.y);
    this->setOrigin(cSize.x / 2, tSize.y / 2);

    // adding sun
    sf::Vector2f sunPos = {500 * cSize.x / 1280, -300 * cSize.y / 720};
    auto sun = std::make_shared<SimpleObject>("aaa_sun", "sun");
    sun->setPosition(sunPos);
    children.push_back(sun);

    size_t firstClouds = randomizer::getRandInt(0, clouds); // наверное будет лучше если часть облаков будет за островами, а часть перед

    // adding some clouds
    for (size_t i = 0; i < firstClouds; ++i)
    {
        std::string textureName = "cloud_" + std::to_string(randomizer::getRandInt(0, 21));
        sf::Vector2f startPos = {randomizer::getRandFloat(-cSize_x, cSize_x), randomizer::getRandFloat(-0.45 * cSize_y, 0 * cSize_y)};
        float distance = clouds / 10 + 1;
        distance = randomizer::getRandFloat(distance - 0.1 * (i + 1), distance - 0.1 * i);
        float speed = randomizer::getRandFloat(0.01, 2);
        auto ptr = std::make_shared<BackgroundObject>("cloud" + std::to_string(i), textureName, speed, distance);
        ptr->setPosition(startPos);
        children.push_back(ptr);
    }

    // adding islands
    for (size_t i = 0; i < islands; ++i)
    {
        std::string textureName = "bg_island_" + std::to_string(randomizer::getRandInt(0, 7));
        sf::Vector2f startPos = {randomizer::getRandFloat(-cSize_x, cSize_x), randomizer::getRandFloat(-0.45 * cSize_y, 0.45 * cSize_y)};
        float distance = islands / 10 + additional_distance;
        distance = randomizer::getRandFloat(distance - 0.1 * (i + 1), distance - 0.1 * i);
        auto ptr = std::make_shared<BackgroundObject>("island" + std::to_string(i), textureName, 0.0f, distance);
        ptr->setPosition(startPos);
        children.push_back(ptr);
    }

    // adding remaining clouds
    for (size_t i = firstClouds; i < clouds; ++i)
    {
        std::string textureName = "cloud_" + std::to_string(randomizer::getRandInt(0, 21));
        sf::Vector2f startPos = {randomizer::getRandFloat(-cSize_x, cSize_x), randomizer::getRandFloat(-0.45 * cSize_y, 0 * cSize_y)};
        float distance = clouds / 10 + 1;
        distance = randomizer::getRandFloat(distance - 0.1 * (i + 1), distance - 0.1 * i);
        float speed = randomizer::getRandFloat(0.01, 2);
        auto ptr = std::make_shared<BackgroundObject>("cloud" + std::to_string(i), textureName, speed, distance);
        ptr->setPosition(startPos);
        children.push_back(ptr);
    }
}

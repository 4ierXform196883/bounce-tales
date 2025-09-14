#include "rock.hpp"

#include "game.hpp"
#include "primitive_sprite.hpp"
#include "randomizer.hpp"

Rock::Rock()
    : GameObject("rock")
{
    const sf::Texture &texture = Game::getAssetManager()->getTexture("rocks");
    const sf::IntRect& rect = Game::getAssetManager()->getSpriteBounds("rocks", std::to_string(randomizer::getRandInt(1, 5)));
    this->setOrigin(sf::Vector2f(rect.width / 2.0f, rect.height / 2.0f));
    drawable = std::make_shared<PrimitiveSprite>(texture, rect);
    CircleHitbox hitbox{.radius = rect.width / 2.0f, .center = {0, 0}};
    collidable = std::make_shared<Collidable>(hitbox);
    physical = std::make_shared<Physical>(rect.width * rect.height / 90, 0.1);
}
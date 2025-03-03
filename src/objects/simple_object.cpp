#include "simple_object.hpp"

#include "game.hpp"
#include "primitive_sprite.hpp"
#include "curved_shape.hpp"

SimpleObject::SimpleObject(const std::string &tag, const std::string &textureName)
    :GameObject(tag)
{
    const sf::Texture &texture = Game::getAssetManager().getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();
    this->setOrigin(sf::Vector2f(tSize.x / 2.0f, tSize.y / 2.0f));
    drawable = std::make_shared<PrimitiveSprite>(texture);
}

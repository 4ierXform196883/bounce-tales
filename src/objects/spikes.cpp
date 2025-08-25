#include "spikes.hpp"

#include "player.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"

Spikes::Spikes(size_t count)
    : GameObject("spikes")
{
    auto &am = Game::getAssetManager();
    const auto &texture = am->getTexture("spikes");
    const auto tSize = texture.getSize();
    const sf::Vector2f size = {(float)tSize.x * count, (float)tSize.y};
    this->drawable = std::make_shared<PrimitiveSprite>(texture, sf::IntRect(0, 0, size.x, size.y));
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {size.x, 0}, {size.x, size.y}, {0, size.y}}}, 0, true);
    this->setOrigin(size.x / 2.f, size.y / 2.f);
}

void Spikes::onCollision(std::shared_ptr<GameObject> other)
{
    if (auto ptr = std::dynamic_pointer_cast<Player>(other))
        ptr->onDeath();
}

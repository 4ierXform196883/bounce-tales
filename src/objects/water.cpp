#include "water.hpp"

#include "game.hpp"
#include "randomizer.hpp"
#include "particle.hpp"
#include "primitive_sprite.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))

Water::Water(const sf::Vector2f &size)
    : GameObject("a_water"), size(size)
{
    const auto &am = Game::getAssetManager();
    const sf::Texture &texture = am->getTexture("water");
    const sf::Vector2f tSize = sf::Vector2f(texture.getSize().x, texture.getSize().y);
    this->drawable = std::make_shared<PrimitiveSprite>(am->getTexture("water"));
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {tSize.x, 0}, tSize, {0, tSize.y}}}, 0, true);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
    this->setScale(size.x / tSize.x, size.y / tSize.y);
    this->particleTimer = Timer::create(0.33, [this]()
                                        {
        const auto& am = Game::getAssetManager();
        size_t amount = randomizer::getRandInt(2, 5);
        Particle::spawnScatter("particles", "bubble", this->getPosition(), 0.1f, 0.4f * this->size, amount);
        this->particleTimer->duration = randomizer::getRandFloat(0.1f, 0.9f); });
}

void Water::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherPhys = std::dynamic_pointer_cast<IPhysical>(other);
    if (!other->isPhysical() || otherPhys->getMass() == -1)
        return;
    if (other->getPosition().y < this->getPosition().y - this->getOrigin().y)
        return;
    otherPhys->addForce(sf::Vector2f(0.f, -0.3f));
}

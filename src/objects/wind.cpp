#include "wind.hpp"

#include "game.hpp"
#include "randomizer.hpp"
#include "particle.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))

Wind::Wind(const sf::Vector2f &size, const sf::Vector2f &direction)
    : GameObject("wind"), size(size), direction(direction)
{
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {size.x, 0}, size, {0, size.y}}}, 0, true);
    this->setOrigin(size.x / 2.f, size.y / 2.f);
    this->particleTimer = Timer::create(0.33, [this]()
                                        {
        const AssetManager& am = Game::getAssetManager();
        std::string particleNames[] = {"wind_hor_0", "wind_hor_1", "wind_hor_2", "wind_ver_0", "wind_ver_1", "wind_ver_2"};
        std::string pName;
        if (abs(this->direction.x) > abs(this->direction.y))
            pName = particleNames[randomizer::getRandInt(0, 3)];
        else
            pName = particleNames[randomizer::getRandInt(3, 6)];
        size_t amount = randomizer::getRandInt(2, 5);
        Particle::spawnScatterDirected("particles", pName, this->getPosition(), 0.5f, 0.45f * this->size, this->direction, amount);
        this->particleTimer->duration = randomizer::getRandFloat(0.1f, 0.9f); });
}

void Wind::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherPhys = std::dynamic_pointer_cast<IPhysical>(other);
    if (!other->isPhysical() || otherPhys->getMass() == -1)
        return;
    otherPhys->addForce(this->direction);
}

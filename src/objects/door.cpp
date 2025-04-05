#include "door.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define normalized(vec) (vec.x == 0 && vec.y == 0 ? sf::Vector2f(0, 0) : (1 / norm(vec)) * vec)

Door::Door(const std::string &tag, const sf::Vector2f &startPos, float elevation)
    : GameObject(tag), startPos(startPos), elevation(elevation)
{
    auto &am = Game::getAssetManager();
    const auto &texture = am.getTexture("door");
    const auto tSize = texture.getSize();
    this->drawable = std::make_shared<PrimitiveSprite>(texture);
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{4, 1}, {30, 1}, {33, 135}, {1, 135}}}, 0);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
    this->setPosition(startPos);
    auto ptr = std::make_shared<SimpleObject>("rope", "rope");
    ptr->setOrigin(0, 2);
    children.emplace(ptr->getTag(), ptr);
    ptr = std::make_shared<SimpleObject>("zzz_another_door_to_hide_the_rope", "door");
    ptr->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
    children.emplace(ptr->getTag(), ptr);
}

void Door::setScale(float factorX, float factorY)
{
    GameObject::setScale(factorX, factorY);
    for (auto [tag, child] : children)
        child->setScale(factorX, factorY);
}

void Door::setScale(const sf::Vector2f &factors)
{
    GameObject::setScale(factors);
    for (auto [tag, child] : children)
        child->setScale(factors);
}

void Door::scale(float factorX, float factorY)
{
    GameObject::scale(factorX, factorY);
    for (auto [tag, child] : children)
        child->scale(factorX, factorY);
}

void Door::scale(const sf::Vector2f &factors)
{
    GameObject::scale(factors);
    for (auto [tag, child] : children)
        child->scale(factors);
}

void Door::update()
{
    double phi = M_PI * (this->getRotation() + 90) / 180;
    sf::Vector2f endPos = startPos - sf::Vector2f(elevation * std::cos(phi), elevation * sin(phi));
    sf::Vector2f endDiff = endPos - this->getPosition();
    this->children.at("rope")->setScale(1, norm(endDiff) / 2.f);
    sf::Vector2f startDiff = startPos - this->getPosition();
    if (isOpened && norm(endDiff) > 2)
        this->move(normalized(endDiff));
    else if (!isOpened && norm(startDiff) > 2)
        this->move(normalized(startDiff));
}

#include "game_object.hpp"

#include <queue>

GameObject::GameObject(
    const std::string &tag,
    std::shared_ptr<sf::Drawable> drawable,
    std::shared_ptr<CollidableComponent> collidable,
    std::shared_ptr<PhysicalComponent> physical,
    std::shared_ptr<SoundComponent> sound)
    : tag(tag), drawable(drawable), collidable(collidable), physical(physical), sound(sound) {}

const std::string &GameObject::getTag() const
{
    return this->tag;
}

void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Transform initTransform = states.transform;
    std::queue<std::shared_ptr<const GameObject>> objs;
    std::queue<sf::Transform> transq;
    objs.push(shared_from_this());
    transq.push(this->getTransformV());
    while (objs.size() != 0)
    {
        std::shared_ptr<const GameObject> cur = objs.front();
        states.transform = initTransform * transq.front();
        if (cur->drawable)
            target.draw(*cur->drawable, states);
        for (auto child : cur->children)
        {
            objs.push(child);
            transq.push(transq.front().translate(cur->getOriginV()) * child->getTransformV());
        }
        objs.pop();
        transq.pop();
    }
}

void GameObject::update()
{
    if (physical)
        physical->update();
    if (sound)
        sound->update();
}

void GameObject::addChild(std::shared_ptr<GameObject> obj)
{
    if (obj)
        this->children.push_back(obj);
}

std::shared_ptr<GameObject> GameObject::findChild(const std::string &tag)
{
    for (auto child : this->children)
    {
        if (child->tag == tag)
            return child;
        if (auto subchild = child->findChild(tag))
            return subchild;
    }
    return nullptr;
}

void GameObject::setPositionV(float x, float y)
{
    VirtualTransformable::setPositionV(x, y);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::setPositionV(const sf::Vector2f &position)
{
    VirtualTransformable::setPositionV(position);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::setRotationV(float angle)
{
    VirtualTransformable::setRotationV(angle);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::setScaleV(float factorX, float factorY)
{
    VirtualTransformable::setScaleV(factorX, factorY);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::setScaleV(const sf::Vector2f &factors)
{
    VirtualTransformable::setScaleV(factors);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::setOriginV(float x, float y)
{
    VirtualTransformable::setOriginV(x, y);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::setOriginV(const sf::Vector2f &origin)
{
    VirtualTransformable::setOriginV(origin);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::moveV(float offsetX, float offsetY)
{
    VirtualTransformable::moveV(offsetX, offsetY);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::moveV(const sf::Vector2f &offset)
{
    VirtualTransformable::moveV(offset);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::rotateV(float angle)
{
    VirtualTransformable::rotateV(angle);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::scaleV(float factorX, float factorY)
{
    VirtualTransformable::scaleV(factorX, factorY);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
void GameObject::scaleV(const sf::Vector2f &factor)
{
    VirtualTransformable::scaleV(factor);
    if (collidable)
        collidable->update();
    if (sound)
        sound->update();
}
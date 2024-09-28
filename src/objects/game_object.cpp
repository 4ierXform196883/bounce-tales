#include "game_object.hpp"

#include <queue>

GameObject::GameObject(
    const std::string &tag,
    std::shared_ptr<sf::Drawable> drawable,
    std::shared_ptr<Collidable> collidable,
    std::shared_ptr<Physical> physical,
    std::shared_ptr<SoundPlayer> soundPlayer)
    : trans(std::make_shared<sf::Transformable>()), tag(tag), drawable(drawable), collidable(collidable), physical(physical), soundPlayer(soundPlayer) {}

void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Transform initTransform = states.transform;
    std::queue<std::shared_ptr<const GameObject>> objs;
    std::queue<sf::Transform> transq;
    objs.push(shared_from_this());
    transq.push(this->getTransform());
    while (objs.size() != 0)
    {
        std::shared_ptr<const GameObject> cur = objs.front();
        states.transform = initTransform * transq.front();
        if (cur->drawable)
            target.draw(*cur->drawable, states);
        for (auto child : cur->children)
        {
            objs.push(child);
            const sf::Vector2f &curScale = cur->getScale();
            transq.push(transq.front().translate(cur->getOrigin()).scale(1 / curScale.x, 1 / curScale.y) * child->getTransform());
        }
        objs.pop();
        transq.pop();
    }
}

void GameObject::fullUpdate()
{
    this->update();
    if (physical)
    {
        this->move(physical->updateSpeed());
    }
}

std::shared_ptr<IGameObject> GameObject::findChild(const std::string &tag)
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

void GameObject::setPosition(float x, float y)
{
    trans->setPosition(x, y);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::setPosition(const sf::Vector2f &position)
{
    trans->setPosition(position);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::setRotation(float angle)
{
    trans->setRotation(angle);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::setScale(float factorX, float factorY)
{
    trans->setScale(factorX, factorY);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::setScale(const sf::Vector2f &factors)
{
    trans->setScale(factors);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::setOrigin(float x, float y)
{
    trans->setOrigin(x, y);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::setOrigin(const sf::Vector2f &origin)
{
    trans->setOrigin(origin);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::move(float offsetX, float offsetY)
{
    trans->move(offsetX, offsetY);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::move(const sf::Vector2f &offset)
{
    trans->move(offset);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::rotate(float angle)
{
    trans->rotate(angle);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::scale(float factorX, float factorY)
{
    trans->scale(factorX, factorY);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

void GameObject::scale(const sf::Vector2f &factor)
{
    trans->scale(factor);
    if (soundPlayer)
        soundPlayer->posUpdate(trans->getPosition());
    if (collidable)
        collidable->transUpdate(trans->getTransform(), trans->getScale());
}

#include "game_object.hpp"

#include <queue>
#include <iostream>

GameObject::GameObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable)
    : tag(tag), drawable(drawable) {}

void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Transform initTransform = states.transform;
    std::queue<std::shared_ptr<const GameObject>> objs;
    std::queue<sf::Vector2f> positions;
    std::queue<float> rotations;
    objs.push(shared_from_this());
    positions.push(sf::Vector2f(0, 0));
    rotations.push(0.0f);
    while (objs.size() != 0)
    {
        std::shared_ptr<const GameObject> cur = objs.front();
        states.transform = initTransform.translate(positions.front()).rotate(rotations.front()) * cur->getTransform();
        if (cur->drawable)
            target.draw(*cur->drawable, states);
        // std::cout << cur->tag << " " << states.transform.transformPoint(sf::Vector2f(0, 0)).x << " " << states.transform.transformPoint(sf::Vector2f(0, 0)).y << std::endl;
        for (auto child : cur->children)
        {
            objs.push(child);
            positions.push(positions.front() + cur->getPosition());
            rotations.push(rotations.front() + cur->getRotation());
        }
        objs.pop();
        positions.pop();
        rotations.pop();
    }
}

void GameObject::fullUpdate()
{
    std::queue<std::shared_ptr<GameObject>> objs;
    objs.push(shared_from_this());
    while (objs.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objs.front();
        cur->update();
        for (auto child : cur->children)
            objs.push(child);
        objs.pop();
    }
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
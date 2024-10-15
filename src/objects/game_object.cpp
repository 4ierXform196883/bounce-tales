#include "game_object.hpp"

#include <queue>
#include <iostream>

GameObject::GameObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable)
    : tag(tag), drawable(drawable), transformable(std::make_shared<Transformable>()) {}

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
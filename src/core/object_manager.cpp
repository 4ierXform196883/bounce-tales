#include "object_manager.hpp"

#include "game.hpp"
#include <queue>

void ObjectManager::load(const std::string &path)
{
    background = std::make_shared<Background>();
}

void ObjectManager::save(const std::string &path)
{
}

void ObjectManager::updateAll()
{
    recursiveUpdate(background);
    // updates

    // checkCollisions

    // this->move(physical->speedUpdate());
}

void ObjectManager::drawBackground(sf::RenderTarget &target)
{
    recursiveDraw(background, target);
}

void ObjectManager::drawObjects(sf::RenderTarget &target)
{
}

void ObjectManager::drawUI(sf::RenderTarget &target)
{
}

void ObjectManager::recursiveDraw(std::shared_ptr<GameObject> object, sf::RenderTarget &target)
{
    sf::Transform identity = sf::Transform::Identity;
    std::queue<std::shared_ptr<GameObject>> objects;
    std::queue<sf::Vector2f> positions;
    std::queue<float> rotations;
    objects.push(object);
    positions.push(sf::Vector2f(0, 0));
    rotations.push(0.0f);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        if (cur->getDrawable())
            target.draw(*cur->getDrawable(), identity.translate(positions.front()).rotate(rotations.front()) * cur->getTransform());
        // std::cout << cur->tag << " " << states.transform.transformPoint(sf::Vector2f(0, 0)).x << " " << states.transform.transformPoint(sf::Vector2f(0, 0)).y << std::endl;
        for (auto child : cur->getChildren())
        {
            objects.push(child);
            positions.push(positions.front() + cur->getPosition());
            rotations.push(rotations.front() + cur->getRotation());
        }
        objects.pop();
        positions.pop();
        rotations.pop();
    }
}

void ObjectManager::recursiveUpdate(std::shared_ptr<GameObject> object)
{
    std::queue<std::shared_ptr<GameObject>> objects;
    objects.push(object);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        cur->update();
        for (auto child : cur->getChildren())
            objects.push(child);
        objects.pop();
    }
}

void ObjectManager::recursiveCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second)
{
    // std::queue<std::shared_ptr<GameObject>> firstObjects;
    // firstObjects.push(first);
    // while (firstObjects.size() != 0)
    // {
    //     std::shared_ptr<GameObject> firstCur = firstObjects.front();

    //     // Another BFS
    //     std::queue<std::shared_ptr<GameObject>> secondObjects;
    //     secondObjects.push(second);
    //     while (secondObjects.size() != 0)
    //     {
    //         std::shared_ptr<GameObject> secondCur = secondObjects.front();
    //         if (firstCur->getCollidable() && secondCur->getCollidable() && !(firstCur->getCollidable()->isTrigger() && secondCur->getCollidable()->isTrigger()))
    //             calculateCollision(firstCur, secondCur);
    //         for (auto secondChild : secondCur->getChildren())
    //             secondObjects.push(secondChild);
    //         secondObjects.pop();
    //     }
    //     // ==========================

    //     for (auto firstChild : firstCur->getChildren())
    //         firstObjects.push(firstChild);
    //     firstObjects.pop();
    // }
}

void ObjectManager::calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second)
{
    if (first->getCollidable()->hitbox.index() != 0)
        return;
    switch (second->getCollidable()->hitbox.index())
    {
    case 0:
        calculateCircleCollision(first, second);
        break;

    case 1:
        calculateRectangleCollision(first, second);
        break;

    case 2:
        calculateConcaveCollision(first, second);
        break;

    default:
        break;
    }
}

void ObjectManager::calculateCircleCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second)
{
    CircleHitbox &firstHitbox = std::get<CircleHitbox>(first->getCollidable()->hitbox);
    CircleHitbox &secondHitbox = std::get<CircleHitbox>(second->getCollidable()->hitbox);
    std::shared_ptr<Transformable> firstTrans = std::dynamic_pointer_cast<Transformable>(first);
    std::shared_ptr<Transformable> secondTrans = std::dynamic_pointer_cast<Transformable>(second);
    std::shared_ptr<Physical> firstPhys = std::dynamic_pointer_cast<Physical>(first);
    std::shared_ptr<Physical> secondPhys = std::dynamic_pointer_cast<Physical>(second);
    if (!firstTrans || !secondTrans || !firstPhys)
        return;
    // firstTrans->getPosition(); firstTrans->move(10, 10); ...
    // firstPhys->speed; firstPhys->addForce({1.0, 2.0}, 1000.0f); ...

    // checking collision...

    // if(noCollision) return;

    if (second->getCollidable()->isTrigger())
    {
        // No movement because collision is with a trigger
    }
    else
    {
        // Move objects appropriately
    }
    first->onCollision(second);
    second->onCollision(first);
}

void ObjectManager::calculateRectangleCollision(std::shared_ptr<GameObject> circle, std::shared_ptr<GameObject> rectangle)
{
}

void ObjectManager::calculateConcaveCollision(std::shared_ptr<GameObject> circle, std::shared_ptr<GameObject> concave)
{
}

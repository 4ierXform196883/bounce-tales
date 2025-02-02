#include "object_manager.hpp"

#include "game.hpp"
#include <queue>
#include "collision_calculator.hpp"
#include "curved_shape.hpp"
#include <iostream>

void ObjectManager::load(const std::string &path)
{
    background = std::make_shared<Background>();
    player = std::make_shared<Player>();
    player->setPosition(0, -500);
    test = std::make_shared<Player>();
    test->removeLongForce("gravity");
    std::vector<sf::Vector2f> verts = {{97, 217}, {156, 236}, {195, 275}, {176, 334}, {136, 373}, {97, 451}, {136, 549}, {215, 608}, {352, 627}, {410, 627}, {449, 569}, {700, 510}, {430, 471}, {391, 432}, {332, 393}, {332, 354}, {352, 314}, {410, 295}, {449, 256}, {469, 178}, {449, 138}, {410, 80}, {352, 60}, {234, 60}, {117, 80}, {97, 99}, {78, 158}};
    testGround = std::make_shared<Ground>(verts);
    // sf::Re
}

void ObjectManager::save(const std::string &path)
{
}

void ObjectManager::updateAll()
{
    recursiveUpdate(background);
    recursiveUpdate(player);
    recursiveUpdate(test);
    // updates

    // checkCollisions

    // this->move(physical->speedUpdate());
}

void ObjectManager::collideAll()
{
    calculateCollision(player, test);
    calculateCollision(player, testGround);
}

void ObjectManager::moveAll()
{
    recursiveMove(player);
    recursiveMove(test);
}

void ObjectManager::drawBackground(sf::RenderTarget &target)
{
    recursiveDraw(background, target);
    auto backroungInterface = std::dynamic_pointer_cast<IGameObject>(background);
}

void ObjectManager::drawObjects(sf::RenderTarget &target)
{
    recursiveDraw(player, target);
    recursiveDraw(test, target);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        player->setPosition(Game::getMousePos() - sf::Vector2f(15, 15));
        player->getPhysical()->speed = {0, 0};
    }
    recursiveDraw(testGround, target);
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

void ObjectManager::recursiveMove(std::shared_ptr<GameObject> object)
{
    std::queue<std::shared_ptr<GameObject>> objects;
    objects.push(object);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        if (cur->getPhysical())
            cur->move(cur->getPhysical()->speedUpdate());
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
    const Hitbox &firstHitbox = first->getCollidable()->getHitbox();
    const Hitbox &secondHitbox = second->getCollidable()->getHitbox();
    
    switch (secondHitbox.index())
    {
    case 0: // circle

    case 1: // triangle
    {
        auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, secondHitbox);
        if (simplex.size() == 0)
            return;
        sf::Vector2f penetration = collision_calculator::getPenetrationVector(firstHitbox, secondHitbox, simplex);
        if (first->getPhysical())
        {
            first->getPhysical()->speed -= penetration;
        }
        break;
    }

    case 2: // concave
    {
        sf::Vector2f penetration = {0, 0};
        auto triangles = std::get<ConcaveHitbox>(secondHitbox).triangles;
        for (auto triangle : triangles)
        {
            auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, triangle);
            if (simplex.size() == 0)
                continue;;
            penetration += collision_calculator::getPenetrationVector(firstHitbox, triangle, simplex);
        }
        if (auto physical = first->getPhysical())
        {
            physical->speed -= penetration;
        }
        break;
    }

    default:
        break;
    }
}

// void ObjectManager::calculateCircleCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second)
// {
//     CircleHitbox &firstHitbox = std::get<CircleHitbox>(first->getCollidable()->hitbox);
//     CircleHitbox &secondHitbox = std::get<CircleHitbox>(second->getCollidable()->hitbox);
//     std::shared_ptr<Transformable> firstTrans = std::dynamic_pointer_cast<Transformable>(first);
//     std::shared_ptr<Transformable> secondTrans = std::dynamic_pointer_cast<Transformable>(second);
//     std::shared_ptr<Physical> firstPhys = std::dynamic_pointer_cast<Physical>(first);
//     std::shared_ptr<Physical> secondPhys = std::dynamic_pointer_cast<Physical>(second);
//     if (!firstTrans || !secondTrans || !firstPhys)
//         return;
//     // firstTrans->getPosition(); firstTrans->move(10, 10); ...
//     // firstPhys->speed; firstPhys->addForce({1.0, 2.0}, 1000.0f); ...

//     // checking collision...

//     // if(noCollision) return;

//     if (second->getCollidable()->isTrigger())
//     {
//         // No movement because collision is with a trigger
//     }
//     else
//     {
//         // Move objects appropriately
//     }
//     first->onCollision(second);
//     second->onCollision(first);
// }

// void ObjectManager::calculateRectangleCollision(std::shared_ptr<GameObject> circle, std::shared_ptr<GameObject> rectangle)
// {
// }

// void ObjectManager::calculateConcaveCollision(std::shared_ptr<GameObject> circle, std::shared_ptr<GameObject> concave)
// {
// }

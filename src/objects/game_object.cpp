#include "game_object.hpp"

#include <queue>
#include <iostream>
#include "collision_calculator.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define normalized(vec) (vec.x == 0 && vec.y == 0 ? sf::Vector2f(0, 0) : (1 / norm(vec)) * vec)

std::map<std::string, size_t> GameObject::namesCount;

void GameObject::calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second, bool notify)
{
    if (!first->collidable || !second->collidable || first.get() == second.get())
        return;
    const Hitbox &firstHitbox = first->collidable->getHitbox();
    const Hitbox &secondHitbox = second->collidable->getHitbox();
    sf::Vector2f penetration;

    switch (secondHitbox.index())
    {
    case 0: // circle

    case 1: // convex
    {
        auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, secondHitbox);
        if (simplex.size() == 0)
            return;
        if (notify)
        {
            first->collidable->colliding.emplace(second->tag, second);
            second->collidable->colliding.emplace(first->tag, first);
            first->onCollision(second);
            second->onCollision(first);
            notify = false;
            if (!first->physical || second->collidable->isTrigger())
                return;
        }
        penetration = -collision_calculator::getPenetrationVector(firstHitbox, secondHitbox, simplex);
        break;
    }

    case 2: // concave
    {
        auto &triangles = std::get<ConcaveHitbox>(secondHitbox).triangles;
        size_t count = 0;
        for (auto triangle : triangles)
        {
            auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, triangle);
            if (simplex.size() == 0)
                continue;
            if (notify)
            {
                first->collidable->colliding.emplace(second->tag, second);
                second->collidable->colliding.emplace(first->tag, first);
                first->onCollision(second);
                second->onCollision(first);
                notify = false;
                if (!first->physical || second->collidable->isTrigger())
                    return;
            }
            penetration -= collision_calculator::getPenetrationVector(firstHitbox, triangle, simplex);
            count++;
        }
        if (count == 0)
            return;
        penetration = {penetration.x / count, penetration.y / count};
        break;
    }

    default:
        return;
    }
    // на всякий случай
    if (norm(penetration) > 20)
        return;
    float firstMass = first->physical ? first->physical->mass : -1;
    float secondMass = second->physical ? second->physical->mass : -1;
    if (firstMass != -1 && secondMass != -1)
    {
        float coef = firstMass / (firstMass + secondMass);
        first->move((1 - coef) * penetration);
        first->physical->speed += (1 - coef) * penetration;
        second->move(-coef * penetration);
        second->physical->speed -= coef * penetration;
        if (first->collidable->prevColliding.find(second->tag) == first->collidable->prevColliding.end())
        {
            sf::Vector2f newVel = coef * first->physical->speed + (1 - coef) * second->physical->speed;
            first->physical->speed = newVel;
            second->physical->speed = newVel;
        }
    }
    else if (firstMass != -1)
    {
        first->move(penetration);
        first->physical->speed += penetration;
        // friction
        sf::Vector2f perpPen = sf::Vector2f(-penetration.y, penetration.x);
        float vt = first->physical->speed.x * perpPen.x + first->physical->speed.y * perpPen.y;
        sf::Vector2f fricDir = -1.f * sign(vt) * normalized(perpPen);
        first->physical->speed += fricDir * first->physical->mass * 0.02f * second->collidable->fricCoef;
    }
    else
    {
        second->move(-penetration);
        second->physical->speed -= penetration;
        // friction
        sf::Vector2f perpPen = sf::Vector2f(-penetration.y, penetration.x);
        float vt = second->physical->speed.x * perpPen.x + second->physical->speed.y * perpPen.y;
        sf::Vector2f fricDir = -1.f * sign(vt) * normalized(perpPen);
        second->physical->speed += fricDir * second->physical->mass * 0.02f * first->collidable->fricCoef;
    }
}

void GameObject::update(std::shared_ptr<GameObject> obj)
{
    std::queue<std::shared_ptr<GameObject>> objects;
    objects.push(obj);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        cur->update();
        if (cur->physical)
        {
            cur->physical->update();
            cur->move(cur->physical->speed);
        }
        if (cur->collidable)
        {
            cur->collidable->prevColliding = cur->collidable->colliding;
            cur->collidable->colliding.clear();
        }

        for (auto child : cur->children)
            objects.push(child);
        objects.pop();
    }
}

void GameObject::draw(std::shared_ptr<GameObject> obj, sf::RenderTarget &target)
{
    sf::Transform identity = sf::Transform::Identity;
    std::queue<std::shared_ptr<GameObject>> objects;
    std::queue<sf::Vector2f> positions;
    std::queue<float> rotations;
    objects.push(obj);
    positions.push(sf::Vector2f(0, 0));
    rotations.push(0.0f);
    while (objects.size() != 0)
    {
        std::shared_ptr<GameObject> cur = objects.front();
        if (cur->drawable)
            target.draw(*cur->drawable, identity.translate(positions.front()).rotate(rotations.front()) * cur->getTransform());
        for (auto child : cur->children)
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

GameObject::GameObject(const std::string &tag)
    : tag(tag), transformable(std::make_shared<sf::Transformable>())
{
    if (namesCount.find(tag) != namesCount.end())
        this->tag = tag + std::to_string(namesCount.at(tag));
    namesCount[tag] += 1;
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

void GameObject::setPosition(float x, float y)
{
    transformable->setPosition(x, y);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::setPosition(const sf::Vector2f &position)
{
    transformable->setPosition(position);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::setRotation(float angle)
{
    transformable->setRotation(angle);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::setScale(float factorX, float factorY)
{
    transformable->setScale(factorX, factorY);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::setScale(const sf::Vector2f &factors)
{
    transformable->setScale(factors);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::setOrigin(float x, float y)
{
    transformable->setOrigin(x, y);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::setOrigin(const sf::Vector2f &origin)
{
    transformable->setOrigin(origin);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::move(float offsetX, float offsetY)
{
    transformable->move(offsetX, offsetY);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::move(const sf::Vector2f &offset)
{
    transformable->move(offset);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::rotate(float angle)
{
    transformable->rotate(angle);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::scale(float factorX, float factorY)
{
    transformable->scale(factorX, factorY);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

void GameObject::scale(const sf::Vector2f &factor)
{
    transformable->scale(factor);
    if (soundPlayer)
        soundPlayer->update(getPosition());
    if (collidable)
        collidable->update(getTransform(), getScale());
}

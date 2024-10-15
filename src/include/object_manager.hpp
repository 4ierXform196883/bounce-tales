#pragma once

#include <string>
#include <unordered_map>
#include "background.hpp"
#include "collidable.hpp"

class ObjectManager
{
public:
    void load(const std::string &path);
    void save(const std::string &path);

    void updateAll();
    void drawBackground(sf::RenderTarget &target);
    void drawObjects(sf::RenderTarget &target);
    void drawUI(sf::RenderTarget &target);
    

private:
    void recursiveDraw(std::shared_ptr<GameObject> object, sf::RenderTarget &target);
    void recursiveUpdate(std::shared_ptr<GameObject> object);
    void recursiveCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);

    void calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);
    void calculateCircleCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);
    void calculateRectangleCollision(std::shared_ptr<GameObject> circle, std::shared_ptr<GameObject> rectangle);
    void calculateConcaveCollision(std::shared_ptr<GameObject> circle, std::shared_ptr<GameObject> concave);
    
    std::shared_ptr<Background> background;
};

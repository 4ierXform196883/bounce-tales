#include "object_manager.hpp"

#include "game.hpp"

std::shared_ptr<Background> ObjectManager::background;

void ObjectManager::load(const std::string &path)
{
    background = std::make_shared<Background>();
}

void ObjectManager::save(const std::string &path)
{
}

void ObjectManager::updateAll()
{
    background->fullUpdate();
    // updates

    // checkCollisions

    // this->move(physical->speedUpdate());
}

void ObjectManager::drawBackground(sf::RenderTarget &target)
{
    target.draw(*background);
}

void ObjectManager::drawObjects(sf::RenderTarget &target)
{
}

void ObjectManager::drawUI(sf::RenderTarget &target)
{
}

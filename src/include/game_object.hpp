#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "transformable.hpp"
#include "collidable.hpp"
#include "physical.hpp"
#include "sound_player.hpp"

class GameObject : public Transformable, public sf::Drawable, public std::enable_shared_from_this<GameObject>
{
    friend class ObjectManager;

public:
    GameObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable);
    virtual ~GameObject() = default;

    virtual const std::string &getTag() const { return tag; }
    virtual std::shared_ptr<GameObject> findChild(const std::string &tag);
    inline virtual std::shared_ptr<const GameObject> findChild(const std::string &tag) const { return findChild(tag); }

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void fullUpdate();
    virtual void update() = 0;

    std::string tag;
    std::shared_ptr<sf::Drawable> drawable;
    std::vector<std::shared_ptr<GameObject>> children;
};
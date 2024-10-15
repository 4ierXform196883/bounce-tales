#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "transformable.hpp"
#include "collidable.hpp"
#include "physical.hpp"
#include "sound_player.hpp"

class IGameObject : public Transformable
{
public:
    virtual const std::string &getTag() const = 0;
    virtual std::shared_ptr<IGameObject> findChild(const std::string &tag) = 0;
    virtual std::shared_ptr<const IGameObject> findChild(const std::string &tag) const = 0;
};

class GameObject : public std::enable_shared_from_this<GameObject>, public IGameObject
{
public:
    GameObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable);
    virtual ~GameObject() = default;

    inline virtual const std::string &getTag() const override { return tag; }
    virtual std::shared_ptr<IGameObject> findChild(const std::string &tag) override;
    inline virtual std::shared_ptr<const IGameObject> findChild(const std::string &tag) const override { return findChild(tag); }

    inline virtual void update() {};
    inline virtual void onCollision(std::shared_ptr<const GameObject> other) {};

    // For object manager only
    inline const std::vector<std::shared_ptr<GameObject>> &getChildren() { return children; }
    inline std::shared_ptr<sf::Drawable> getDrawable() { return drawable; }
    inline std::shared_ptr<Transformable> getTransformable() { return transformable; }
    inline std::shared_ptr<Collidable> getCollidable() { return collidable; }
    inline std::shared_ptr<Physical> getPhysical() { return physical; }
    inline std::shared_ptr<SoundPlayer> getSoundPlayer() { return soundPlayer; }

protected:
    std::string tag;
    std::vector<std::shared_ptr<GameObject>> children;

    std::shared_ptr<sf::Drawable> drawable;
    std::shared_ptr<Transformable> transformable;
    std::shared_ptr<Collidable> collidable;
    std::shared_ptr<Physical> physical;
    std::shared_ptr<SoundPlayer> soundPlayer;
};
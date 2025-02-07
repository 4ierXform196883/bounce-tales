#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "transformable.hpp"
#include "collidable.hpp"
#include "physical.hpp"
#include "sound_player.hpp"

class IGameObject : public ITransformable
{
public:
    virtual const std::string &getTag() const = 0;
    virtual std::shared_ptr<IGameObject> findChild(const std::string &tag) = 0;
    virtual std::shared_ptr<const IGameObject> findChild(const std::string &tag) const = 0;
};

class GameObject : public IGameObject
{
public:
    GameObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable);
    virtual ~GameObject() = default;

    inline virtual const std::string &getTag() const override { return tag; }
    virtual std::shared_ptr<IGameObject> findChild(const std::string &tag) override;
    inline virtual std::shared_ptr<const IGameObject> findChild(const std::string &tag) const override { return findChild(tag); }

    inline virtual void update() {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) {};

    // For object manager only
    inline const std::vector<std::shared_ptr<GameObject>> &getChildren() { return children; }
    inline std::shared_ptr<sf::Drawable> getDrawable() { return drawable; }
    inline std::shared_ptr<Transformable> getTransformable() { return transformable; }
    inline std::shared_ptr<Collidable> getCollidable() { return collidable; }
    inline std::shared_ptr<Physical> getPhysical() { return physical; }
    inline std::shared_ptr<SoundPlayer> getSoundPlayer() { return soundPlayer; }

    // Transformable override
    virtual void setPosition(float x, float y) override;
    virtual void setPosition(const sf::Vector2f &position) override;
    virtual void setRotation(float angle) override;
    virtual void setScale(float factorX, float factorY) override;
    virtual void setScale(const sf::Vector2f &factors) override;
    virtual void setOrigin(float x, float y) override;
    virtual void setOrigin(const sf::Vector2f &origin) override;
    virtual void move(float offsetX, float offsetY) override;
    virtual void move(const sf::Vector2f &offset) override;
    virtual void rotate(float angle) override;
    virtual void scale(float factorX, float factorY) override;
    virtual void scale(const sf::Vector2f &factor) override;
    inline virtual const sf::Vector2f &getPosition() const override { return transformable->getPosition(); }
    inline virtual float getRotation() const override { return transformable->getRotation(); }
    inline virtual const sf::Vector2f &getScale() const override { return transformable->getScale(); }
    inline virtual const sf::Vector2f &getOrigin() const override { return transformable->getOrigin(); }
    inline virtual const sf::Transform &getTransform() const override { return transformable->getTransform(); }
    inline virtual const sf::Transform &getInverseTransform() const override { return transformable->getInverseTransform(); }

protected:
    std::string tag;
    std::vector<std::shared_ptr<GameObject>> children;

    std::shared_ptr<sf::Drawable> drawable;
    std::shared_ptr<Transformable> transformable;
    std::shared_ptr<Collidable> collidable;
    std::shared_ptr<Physical> physical;
    std::shared_ptr<SoundPlayer> soundPlayer;
};
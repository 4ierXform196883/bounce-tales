#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "transformable.hpp"
#include "collidable.hpp"
#include "physical.hpp"
#include "sound_player.hpp"

class IGameObject : public sf::Drawable, public ITransformable
{
public:
    virtual const std::string &getTag() const = 0;
    virtual std::shared_ptr<IGameObject> findChild(const std::string &tag) = 0;
    virtual std::shared_ptr<const IGameObject> findChild(const std::string &tag) const = 0;
};

class IGameObjectPrivate : public IGameObject
{
public:
    virtual void fullUpdate() = 0;
};

class GameObject : public std::enable_shared_from_this<GameObject>, public IGameObjectPrivate
{
public:
    GameObject(
        const std::string &tag,
        std::shared_ptr<sf::Drawable> drawable,
        std::shared_ptr<Collidable> collidable = nullptr,
        std::shared_ptr<Physical> physical = nullptr,
        std::shared_ptr<SoundPlayer> sound = nullptr);
    virtual ~GameObject() = default;

    virtual void fullUpdate() override;

    virtual const std::string &getTag() const override { return tag; }
    virtual std::shared_ptr<IGameObject> findChild(const std::string &tag) override;
    inline virtual std::shared_ptr<const IGameObject> findChild(const std::string &tag) const override { return findChild(tag); }

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
    inline virtual const sf::Vector2f &getPosition() const override { return trans->getPosition(); }
    inline virtual float getRotation() const override { return trans->getRotation(); }
    inline virtual const sf::Vector2f &getScale() const override { return trans->getScale(); }
    inline virtual const sf::Vector2f &getOrigin() const override { return trans->getOrigin(); }
    inline virtual const sf::Transform &getTransform() const override { return trans->getTransform(); }
    inline virtual const sf::Transform &getInverseTransform() const override { return trans->getInverseTransform(); }

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void update() = 0;

    std::string tag;
    std::vector<std::shared_ptr<GameObject>> children;

    std::shared_ptr<sf::Drawable> drawable;
    std::shared_ptr<sf::Transformable> trans;
    std::shared_ptr<Collidable> collidable;
    std::shared_ptr<Physical> physical;
    std::shared_ptr<SoundPlayer> soundPlayer;
};
#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "transformable.hpp"
#include "collidable.hpp"
#include "physical.hpp"
#include "sound_player.hpp"

#define COLLIDABLE                                                                         \
    inline virtual const Hitbox &getHitbox() const override { return collidable->hitbox; } \
    inline virtual bool isTrigger() const override { return collidable->trigger; }

#define PHYSICAL                                                                                                                                                                                     \
    inline virtual void addForce(const sf::Vector2f &force) override { physical->addForce(force); }                                                                                                  \
    inline virtual void addForce(const sf::Vector2f &direction, float power) override { physical->addForce(direction, power); }                                                                      \
    inline virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) override { physical->addLongForce(name, direction, power, duration); } \
    inline virtual const LongForce &getLongForce(const std::string &name) const override { return physical->getLongForce(name); }                                                                    \
    inline virtual LongForce &getLongForce(const std::string &name) override { return physical->getLongForce(name); }                                                                                \
    inline virtual void removeLongForce(const std::string &name) override { physical->removeLongForce(name); }                                                                                       \
    inline virtual const sf::Vector2f &getSpeed() const override { return physical->getSpeed(); }                                                                                                    \
    inline virtual void setGravity(float power) override { physical->setGravity(power); }                                                                                                            \
    inline virtual void setFriction(float power) override { physical->setFriction(power); }                                                                                                          \
    inline virtual void setMaxSpeed(float value) override { physical->setMaxSpeed(value); }                                                                                                          \
    inline virtual void setMass(float value) override { physical->setMass(value); }                                                                                                                  \
    inline virtual float getGravity() const override { return physical->getGravity(); }                                                                                                              \
    inline virtual float getFriction() const override { return physical->getFriction(); }                                                                                                            \
    inline virtual float getMaxSpeed() const override { return physical->getMaxSpeed(); }                                                                                                            \
    inline virtual float getMass() const override { return physical->getMass(); }

class GameObject : public ITransformable, public sf::Drawable
{
public:
    static void calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second, bool notify = true);
    static void update(std::shared_ptr<GameObject> obj);
    static void draw(std::shared_ptr<GameObject> obj, sf::RenderTarget &target);

    GameObject(const std::string &tag);
    virtual ~GameObject() = default;

    inline bool isAlive() const { return alive; }
    inline const std::string &getTag() const { return tag; }
    std::shared_ptr<GameObject> findChild(const std::string &tag);
    inline std::shared_ptr<const GameObject> findChild(const std::string &tag) const { return findChild(tag); }

    // Interface overrides
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
    inline virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const { target.draw(*drawable, states); }

protected:
    virtual void update() = 0;
    virtual void onCollision(std::shared_ptr<GameObject> other) = 0;

    bool alive = true;
    std::string tag;
    std::vector<std::shared_ptr<GameObject>> children;

    std::shared_ptr<sf::Drawable> drawable;
    std::shared_ptr<sf::Transformable> transformable;
    std::shared_ptr<Collidable> collidable;
    std::shared_ptr<Physical> physical;
    std::shared_ptr<SoundPlayer> soundPlayer;
};
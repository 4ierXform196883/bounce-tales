#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "drawable.hpp"
#include "transformable.hpp"
#include "collidable.hpp"
#include "physical.hpp"

#define COLLIDABLE                                                                         \
    inline virtual const Hitbox &getHitbox() const override { return collidable->hitbox; } \
    inline virtual float getFricCoef() const override { return collidable->fricCoef; }     \
    inline virtual bool isTrigger() const override { return collidable->trigger; }

#define PHYSICAL                                                                                                                                                                                     \
    inline virtual void addForce(const sf::Vector2f &force) override { physical->addForce(force); }                                                                                                  \
    inline virtual void addForce(const sf::Vector2f &direction, float power) override { physical->addForce(direction, power); }                                                                      \
    inline virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) override { physical->addLongForce(name, direction, power, duration); } \
    inline virtual const LongForce &getLongForce(const std::string &name) const override { return physical->getLongForce(name); }                                                                    \
    inline virtual LongForce &getLongForce(const std::string &name) override { return physical->getLongForce(name); }                                                                                \
    inline virtual void removeLongForce(const std::string &name) override { physical->removeLongForce(name); }                                                                                       \
    inline virtual const sf::Vector2f &getSpeed() const override { return physical->getSpeed(); }                                                                                                    \
    inline virtual void setMass(float value) override { physical->setMass(value); }                                                                                                                  \
    inline virtual float getMass() const override { return physical->getMass(); }                                                                                                                    \
    inline virtual void setAirResistance(float value) override { physical->setAirResistance(value); }                                                                                                \
    inline virtual float getAirResistance() const override { return physical->getAirResistance(); }

class GameObject : public ITransformable, public IDrawable
{
public:
    static void calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second, bool notify = true);
    static void update(std::shared_ptr<GameObject> obj);
    static void draw(std::shared_ptr<GameObject> obj, sf::RenderTarget &target);

    GameObject(const std::string &tag);
    virtual ~GameObject() = default;

    inline bool isHidden() const { return hidden; }
    inline void setHidden(bool value) { this->hidden = value; }
    inline const std::string &getTag() const { return tag; }

    inline bool isCollidable() const { return collidable != nullptr; }
    inline bool isPhysical() const { return physical != nullptr; }

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
    inline virtual const sf::Color &getColor() const override { return drawable->getColor(); }
    inline virtual void setColor(const sf::Color &color) override { drawable->setColor(color); }
    inline virtual sf::FloatRect getLocalBounds() const override { return drawable->getLocalBounds(); }
    inline sf::FloatRect getGlobalBounds() const { return getTransform().transformRect(getLocalBounds()); }

    bool alive = true;

protected:
    inline virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const { target.draw(*drawable, states); }
    virtual void update() = 0;
    virtual void onCollision(std::shared_ptr<GameObject> other) = 0;

    bool hidden = false;
    std::string tag;
    std::map<std::string, std::shared_ptr<GameObject>> children;

    std::shared_ptr<IDrawable> drawable;
    std::shared_ptr<sf::Transformable> transformable;
    std::shared_ptr<Collidable> collidable;
    std::shared_ptr<Physical> physical;
};
#pragma once

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "virtual_transformable.hpp"
#include "sound_component.hpp"
#include "collidable_component.hpp"
#include "physical_component.hpp"

class GameObject : public sf::Drawable, public VirtualTransformable, public std::enable_shared_from_this<GameObject>
{
public:
    GameObject() = default;
    GameObject(
        const std::string &tag,
        std::shared_ptr<sf::Drawable> drawable,
        std::shared_ptr<CollidableComponent> collidable = nullptr,
        std::shared_ptr<PhysicalComponent> physical = nullptr,
        std::shared_ptr<SoundComponent> sound = nullptr);
    virtual ~GameObject() = default;

    virtual void update();
    virtual void onCollision(std::shared_ptr<const GameObject> other) = 0;

    void addChild(std::shared_ptr<GameObject> obj);
    std::shared_ptr<GameObject> findChild(const std::string &tag);

    const std::string &getTag() const;
    std::shared_ptr<sf::Drawable> getDrawable() { return drawable; }
    std::shared_ptr<const sf::Drawable> getDrawable() const { return drawable; }
    std::shared_ptr<CollidableComponent> getCollidable() { return collidable; }
    std::shared_ptr<const CollidableComponent> getCollidable() const { return collidable; }
    std::shared_ptr<PhysicalComponent> getPhysical() { return physical; }
    std::shared_ptr<const PhysicalComponent> getPhysical() const { return physical; }
    std::shared_ptr<SoundComponent> getSound() { return sound; }
    std::shared_ptr<const SoundComponent> getSound() const { return sound; }

    virtual void setPositionV(float x, float y) override;
    virtual void setPositionV(const sf::Vector2f &position) override;
    virtual void setRotationV(float angle) override;
    virtual void setScaleV(float factorX, float factorY) override;
    virtual void setScaleV(const sf::Vector2f &factors) override;
    virtual void setOriginV(float x, float y) override;
    virtual void setOriginV(const sf::Vector2f &origin) override;
    virtual void moveV(float offsetX, float offsetY) override;
    virtual void moveV(const sf::Vector2f &offset) override;
    virtual void rotateV(float angle) override;
    virtual void scaleV(float factorX, float factorY) override;
    virtual void scaleV(const sf::Vector2f &factor) override;

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    std::string tag;
    std::shared_ptr<sf::Drawable> drawable;
    std::shared_ptr<CollidableComponent> collidable;
    std::shared_ptr<PhysicalComponent> physical;
    std::shared_ptr<SoundComponent> sound;
    std::vector<std::shared_ptr<GameObject>> children;
};
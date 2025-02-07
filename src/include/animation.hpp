#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "timer.hpp"
#include "primitive_sprite.hpp"

class Animation : public PrimitiveSprite
{
public:
    Animation() = default;
    Animation(const sf::Texture &texture, const std::vector<sf::IntRect> bounds, double fps);
    virtual ~Animation() = default;

    void changeSpeed(double speedMultiplier);
    void restart();
    inline size_t getCurrentFrame() const { return currentFrame; };

protected:
    void update();

    const std::vector<sf::IntRect> bounds;
    const double fps;

    std::shared_ptr<Timer> changeBoundsTimer;
    size_t currentFrame = 0;
};
#include "animation.hpp"
#include "asset_manager.hpp"

Animation::Animation(const sf::Texture &texture, const std::vector<sf::IntRect> bounds, double fps)
    : PrimitiveSprite(texture, bounds[0]), bounds(bounds), fps(fps)
{
    this->changeBoundsTimer = Timer::create(1 / fps, [this]() { this->update(); });
}

void Animation::changeSpeed(double speedMultiplier)
{
    changeBoundsTimer->duration = 1 / (this->fps * speedMultiplier);
}

void Animation::restart()
{
    changeBoundsTimer->restart();
    currentFrame = 0;
    this->setTextureRect(this->bounds[this->currentFrame]);
}
void Animation::update()
{
    this->currentFrame = (this->currentFrame + 1) % this->bounds.size();
    this->setTextureRect(this->bounds[this->currentFrame]);
}
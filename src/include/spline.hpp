#pragma once

#include <SFML/Graphics.hpp>
#include "drawable.hpp"

// С1 непрерывный сплайн из кривых Безье 3-го порядка
class Spline : public IDrawable, public sf::Transformable
{
public:
    Spline() = default;
    Spline(const std::vector<sf::Vector2f> &controlPoints);
    ~Spline() = default;

    // Добавить точку: только главную (P0 и P3)
    // доп точки добавляются автоматически ради C1 непрерывности
    void addControlPoint(const sf::Vector2f &point);
    void insertControlPoint(int nearIndex, const sf::Vector2f &point);
    int removeControlPoint(int mainIndex);

    // Передвинуть точку
    void movePoint(int index, const sf::Vector2f &delta);

    inline const sf::Vector2f &getPoint(int index) const { return controlPoints.at(index); }
    inline const std::vector<sf::Vector2f> &getPoints() const { return controlPoints; }
    inline size_t getPointCount() const { return controlPoints.size(); }
    inline size_t getSplineCount() const { return controlPoints.size() / 3; }

    // Завершить сплайн (+ доп точка, + глав. точка, равная нулевой)
    void setFinished(bool finished);
    inline bool isFinished() const { return finished; }

    // i - номер сплайна, t - параметр от 0 до 1
    sf::Vector2f evaluate(int i, float t) const;
    sf::Vector2f derivative(int i, float t) const;

    virtual const sf::Color &getColor() const override { return sf::Color::White; }
    virtual void setColor(const sf::Color &color) override {};
    virtual sf::FloatRect getLocalBounds() const override;

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    std::vector<sf::Vector2f> controlPoints;
    bool finished = false;
};
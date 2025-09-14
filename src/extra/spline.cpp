#include "spline.hpp"
#include <iostream>

Spline::Spline(const std::vector<sf::Vector2f> &controlPoints)
    : controlPoints(controlPoints)
{
    if (controlPoints.empty())
        return;
    auto &first = controlPoints.front();
    auto &last = controlPoints.back();
    if (controlPoints.size() == 1)
    {
        this->controlPoints.push_back(last + sf::Vector2f(30, 0));
        return;
    }

    auto &prelast = controlPoints[controlPoints.size() - 2];
    if (controlPoints.size() % 3 == 1 && first == last)
    {
        this->finished = true;
    }
    else if (controlPoints.size() % 3 == 1)
    {
        sf::Vector2f delta = last - prelast;
        this->controlPoints.push_back(last + delta);
    }
    else if (controlPoints.size() % 3 == 0)
    {
        this->controlPoints.pop_back();
    }
}
void Spline::addControlPoint(const sf::Vector2f &point)
{
    if (finished)
        return;
    if (controlPoints.empty())
    {
        controlPoints.push_back(point);
        controlPoints.push_back({point.x + 30, point.y});
    }
    else
    {
        auto &last = controlPoints.back();
        sf::Vector2f delta = point - last;
        controlPoints.push_back(point - delta / 3.f);
        controlPoints.push_back(point);
        controlPoints.push_back(point + delta / 3.f);
    }
}

void Spline::insertControlPoint(int nearIndex, const sf::Vector2f &point)
{
    if (!finished || controlPoints.size() < 4)
        return;
    // Найдём ближайшую главную точку
    int mainIndex = nearIndex - (nearIndex % 3);
    // if можно и по короче но так понятнее: не берем последнюю точку
    if (finished && mainIndex == controlPoints.size() - 1 /*|| !finished && mainIndex == controlPoints.size() - 2*/)
        mainIndex -= 3;
    // Итого ставим после ближайшей (сзади) главной точки (но не последней)
    // Наверное установим точку параллельно отрезку нужного сплайна
    sf::Vector2f cpDelta = controlPoints[mainIndex + 3] - controlPoints[mainIndex];
    sf::Vector2f newP0 = point;
    sf::Vector2f newP1 = point - cpDelta / 3.f;
    sf::Vector2f newP2 = point + cpDelta / 3.f;

    controlPoints.insert(controlPoints.begin() + mainIndex + 2, newP2);
    controlPoints.insert(controlPoints.begin() + mainIndex + 2, newP0);
    controlPoints.insert(controlPoints.begin() + mainIndex + 2, newP1);
}

int Spline::removeControlPoint(int mainIndex)
{
    if (controlPoints.size() <= 2)
        return 0;
    if (mainIndex % 3 != 0)
        return 0;
    if (finished && (mainIndex == 0 || mainIndex == controlPoints.size() - 1))
        return 0;
    if (mainIndex == 0)
    {
        controlPoints.erase(controlPoints.begin(), controlPoints.begin() + 3);
        return 3;
    }
    else
    {
        controlPoints.erase(controlPoints.begin() + mainIndex - 1, controlPoints.begin() + mainIndex + 2);
        return 3;
    }
    return 0;
}

void Spline::movePoint(int index, const sf::Vector2f &delta)
{
    if (index < 0 || index >= controlPoints.size())
        return;

    // Незамкнутые точки 0 и 1 просто двигаем
    if (!finished && (index == 0 || index == 1))
    {
        controlPoints[index] += delta;
    }
    // После замыкания: точка 0 == последней точке, двигаем их как в дефолт случае для главной точки
    else if (finished && (index == 0 || index == controlPoints.size() - 1))
    {
        controlPoints[0] += delta;
        controlPoints[controlPoints.size() - 1] += delta;
        controlPoints[1] += delta;
        controlPoints[controlPoints.size() - 2] += delta;
    }
    // Двигаем точку 1: её парная точка после замыкания на самом деле находится в конце
    else if (finished && index == 1)
    {
        controlPoints[index] += delta;
        controlPoints[controlPoints.size() - 2] -= delta;
    }
    // Зеркально: для последней не опорной точки парная - точка 1
    else if (finished && index == controlPoints.size() - 2)
    {
        controlPoints[index] += delta;
        controlPoints[1] -= delta;
    }
    // Дефолтный случай: главная точка
    else if (index % 3 == 0)
    {
        controlPoints[index - 1] += delta;
        controlPoints[index] += delta;
        controlPoints[index + 1] += delta;
    }
    // Дефолтный случай: точка, следующая за главной точкой
    else if (index % 3 == 1)
    {
        controlPoints[index] += delta;
        controlPoints[index - 2] -= delta;
    }
    // Дефолтный случай: точка, предшествующая главной точке
    else if (index % 3 == 2)
    {
        controlPoints[index] += delta;
        controlPoints[index + 2] -= delta;
    }
}

void Spline::setFinished(bool finished)
{
    if (controlPoints.size() <= 2 || finished == this->finished)
        return;
    if (!this->finished)
    {
        sf::Vector2f delta = controlPoints[0] - controlPoints[1];
        controlPoints.push_back(controlPoints[0] + delta);
        controlPoints.push_back(controlPoints[0]);
    }
    else
    {
        controlPoints.pop_back();
        controlPoints.pop_back();
    }
    this->finished = finished;
}

sf::Vector2f Spline::evaluate(int i, float t) const
{
    auto &p0 = controlPoints[i * 3];
    auto &p1 = controlPoints[i * 3 + 1];
    auto &p2 = controlPoints[i * 3 + 2];
    auto &p3 = controlPoints[i * 3 + 3];

    float u = 1 - t;
    return u * u * u * p0 +
           3 * u * u * t * p1 +
           3 * u * t * t * p2 +
           t * t * t * p3;
}

sf::Vector2f Spline::derivative(int i, float t) const
{
    auto &p0 = controlPoints[i * 3];
    auto &p1 = controlPoints[i * 3 + 1];
    auto &p2 = controlPoints[i * 3 + 2];
    auto &p3 = controlPoints[i * 3 + 3];

    float u = 1 - t;
    return -3 * u * u * p0 +
           (3 * u * u - 6 * u * t) * p1 +
           (6 * u * t - 3 * t * t) * p2 +
           3 * t * t * p3;
}

sf::FloatRect Spline::getLocalBounds() const
{
    // INCORRECT???
    if (controlPoints.empty())
        return sf::FloatRect();

    float minX = controlPoints[0].x;
    float maxX = controlPoints[0].x;
    float minY = controlPoints[0].y;
    float maxY = controlPoints[0].y;

    for (const auto &point : controlPoints)
    {
        if (point.x < minX)
            minX = point.x;
        if (point.x > maxX)
            maxX = point.x;
        if (point.y < minY)
            minY = point.y;
        if (point.y > maxY)
            maxY = point.y;
    }

    return sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
}

void Spline::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // std::cout << "Drawing spline with " << getPointCount() << " points, finished: " << finished << ", spline count: " << getSplineCount() << "\n";
    sf::VertexArray splineDrawable(sf::PrimitiveType::LineStrip);
    sf::CircleShape circle(3);
    states.transform *= getTransform();
    for (size_t i = 0; i < getSplineCount(); ++i)
    {
        // Рисуем линии
        for (float t = 0; t <= 1; t += 0.01f)
        {
            sf::Vector2f point = evaluate(i, t);
            splineDrawable.append(sf::Vertex(point, sf::Color::White));
        }
        target.draw(splineDrawable, states);
        // Рисуем контрольные точки
        circle.setRadius(3);
        circle.setOrigin(3, 3);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(controlPoints[i * 3]);
        target.draw(circle, states);
        circle.setFillColor(sf::Color::Blue);
        circle.setRadius(2);
        circle.setOrigin(2, 2);
        circle.setPosition(controlPoints[i * 3 + 1]);
        target.draw(circle, states);
        circle.setPosition(controlPoints[i * 3 + 2]);
        target.draw(circle, states);
    }
    if (!finished)
    {
        size_t i = getPointCount();
        circle.setRadius(3);
        circle.setOrigin(3, 3);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(controlPoints[i - 2]);
        target.draw(circle, states);
        circle.setRadius(2);
        circle.setOrigin(2, 2);
        circle.setFillColor(sf::Color::Blue);
        circle.setPosition(controlPoints[i - 1]);
        target.draw(circle, states);
    }
}

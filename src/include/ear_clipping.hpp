#pragma once

#include <SFML/Graphics.hpp>

namespace ear_clipping
{
    struct Node
    {
        sf::Vector2f value;
        int next = -1;
        int prev = -1;

        bool isReflex = false;
        int index;
    };

    bool isEar(const Node &n, const std::vector<Node> &nodes, const std::vector<sf::Vector2f> &vertices);
    std::vector<sf::Vector2f> processConcaveShape(const std::vector<sf::Vector2f>& vertices);
};
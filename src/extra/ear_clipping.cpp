#include "ear_clipping.hpp"
#include "collision_math.hpp"
#include <unordered_map>

bool ear_clipping::isEar(const Node &n, const std::vector<Node> &nodes, const std::vector<sf::Vector2f> &vertices)
{
    bool isEar = true;

    for (unsigned int j = 0; j < nodes.size(); ++j)
    {
        if (nodes[j].index != n.index && nodes[j].index != n.next && nodes[j].index != n.prev)
            if (collision_math::isInTriangle(vertices[n.index], vertices[n.prev], vertices[n.next], vertices[nodes[j].index]))
            {
                isEar = false;
                break;
            }
    }

    return isEar;
}

#include <iostream>

std::vector<sf::Vector2f> ear_clipping::processConcaveShape(const std::vector<sf::Vector2f> &vertices)
{
    // for (auto vert : vertices)
    // {
    //     std::cout << vert.x << " " << vert.y << "   ";
    // }
    std::vector<sf::Vector2f> resultVertices;
    int pointCount = vertices.size();
    std::vector<sf::ConvexShape> shapes;
    std::vector<Node> nodes;
    nodes.resize(pointCount);
    std::vector<int> reflexVertices;         // for ear clipping
    std::vector<int> reflexVerticesForShape; // to pass to shape for M-sum
    reflexVertices.resize(pointCount, -1);
    std::vector<int> convexVertices;
    convexVertices.resize(pointCount, -1);
    std::unordered_map<int, int> ears;

    for (int i = 0; i < pointCount; ++i)
    {
        nodes[i].value = vertices[i];
        nodes[i].index = i;
        nodes[i].prev = collision_math::mod((i - 1), pointCount);
        nodes[i].next = collision_math::mod((i + 1), pointCount);
    }
    /*
    Just in case passing nodes vector starts shwoing bugs, gonna save this.
    std::vector<int> temp;
    temp.reserve(reflexVertices.size() + convexVertices.size());
    temp.insert(temp.end(), reflexVertices.begin(), reflexVertices.end());
    temp.insert(temp.end(), convexVertices.begin(), convexVertices.end());
    */

    for (int i = 0; i < pointCount; ++i)
    {

        printf("%i \n", i);
        if (collision_math::isReflex(nodes[i].value, nodes[nodes[i].prev].value, nodes[nodes[i].next].value))
        {
            reflexVertices[i] = i;
            reflexVerticesForShape.push_back(i);
            nodes[i].isReflex = true;
            printf(" %i is reflex \n", i);
        }
        else
        {
            convexVertices[i] = i;
        }

        if (!nodes[i].isReflex)
            if (isEar(nodes[i], nodes, vertices))
                ears[i] = i;
    }

    sf::ConvexShape cs;
    int steps = 0;
    while (!ears.empty())
    {
        int ear = ears.begin()->second;
        ears.erase(ear);

        Node currentNode = nodes[ear];
        int next = currentNode.next;
        int prev = currentNode.prev;

        if ((next == prev) || (next == ear) || (prev == ear))
            break;

        cs = sf::ConvexShape(3);
        resultVertices.push_back(currentNode.value);
        resultVertices.push_back(nodes[currentNode.prev].value);
        resultVertices.push_back(nodes[currentNode.next].value);
        cs.setPoint(0, currentNode.value);
        cs.setPoint(1, nodes[currentNode.prev].value);
        cs.setPoint(2, nodes[currentNode.next].value);
        cs.setOutlineThickness(0);
        std::cout << currentNode.value.x << " " << currentNode.value.y << "   " << nodes[currentNode.prev].value.x << " " << nodes[currentNode.prev].value.y << "   " << nodes[currentNode.next].value.x << " " << nodes[currentNode.next].value.y << "\n";
        shapes.push_back(cs);

        nodes[prev].next = next;
        nodes[next].prev = prev;

        if (nodes[prev].isReflex)
        {
            if (!(nodes[prev].isReflex = collision_math::isReflex(nodes[prev].value, nodes[nodes[prev].prev].value, nodes[nodes[prev].next].value)))
            {
                reflexVertices[prev] = -1;
                convexVertices[prev] = prev;
            }
        }

        if (nodes[next].isReflex)
        {
            if (!(nodes[next].isReflex = collision_math::isReflex(nodes[next].value, nodes[nodes[next].prev].value, nodes[nodes[next].next].value)))
            {
                reflexVertices[next] = -1;
                convexVertices[next] = next;
            }
        }

        auto it = ears.begin();
        while (it != ears.end())
        {
            if (!isEar(nodes[it->second], nodes, vertices))
                it = ears.erase(it);
            else
                it++;
        }

        if (!nodes[prev].isReflex)
            if (ears.find(prev) == ears.end() && isEar(nodes[prev], nodes, vertices)) //; not sure why this was here
                ears[prev] = prev;

        if (!nodes[next].isReflex)
            if (ears.find(next) == ears.end() && isEar(nodes[next], nodes, vertices)) //; not sure why this was here
                ears[next] = next;

        steps++;
    }

    // ConcaveShape concaveShape(shapes, vertices, reflexVerticesForShape);
    // concaveShape.SetFont(m_labelFont);
    // if (!m_unusedConcaveShapes.empty())
    // {
    //     int index = m_unusedConcaveShapes.front();
    //     m_concaveShapes[index] = concaveShape;
    //     m_unusedConcaveShapes.pop();
    // }
    // else
    //     m_concaveShapes.push_back(concaveShape);
    return resultVertices;
}

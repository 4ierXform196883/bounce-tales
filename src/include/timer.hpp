#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Timer
{
public:
    static std::shared_ptr<Timer> create(double duration, std::function<void()> callback, bool autoReset = true);

    static void updateAll();

    sf::Clock& getClock();
    void restart();
    
    double duration = 0;

private:
    Timer(double duration, std::function<void()> callback, bool autoReset);
    
    sf::Clock clock;
    std::function<void()> callback;
    bool autoReset;
    bool done = false;

    static std::vector<std::weak_ptr<Timer>> globalTimers;
};

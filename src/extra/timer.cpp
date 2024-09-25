#include "timer.hpp"

std::vector<std::weak_ptr<Timer>> Timer::globalTimers;

std::shared_ptr<Timer> Timer::create(double duration, std::function<void()> callback, bool autoReset)
{
    auto timer = std::shared_ptr<Timer>(new Timer(duration, callback, autoReset));
    globalTimers.push_back(timer);
    return timer;
}

void Timer::updateAll()
{
    auto isExpired = [](std::weak_ptr<Timer> timer) { return timer.expired(); };
    globalTimers.erase(std::remove_if(globalTimers.begin(), globalTimers.end(), isExpired), globalTimers.end());

    for (auto &timer : globalTimers)
    {
        if (auto timerPtr = timer.lock())
        {
            if (!timerPtr->done && timerPtr->getClock().getElapsedTime().asSeconds() > timerPtr->duration)
            {
                timerPtr->callback();
                if (timerPtr->autoReset) timerPtr->getClock().restart();
                else timerPtr->done = true;
            }
        }
    }
}

sf::Clock &Timer::getClock()
{
    return clock;
}

void Timer::restart()
{
    this->clock.restart();
    this->done = false;
}

Timer::Timer(double duration, std::function<void()> callback, bool autoReset)
{
    this->duration = duration;
    this->callback = callback;
    this->autoReset = autoReset;
}

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Boid.hpp"

constexpr float WIDTH = 1800;
constexpr float HEIGHT = 800;

class Flock {
private:

    std::shared_ptr<sf::RenderWindow> m_window;
    sf::VideoMode m_videoMode;
    sf::Event m_ev;
    bool m_isWindowFocused;

    void initVariables();
    void initWindow();
    void initEnviroment();

    sf::Clock m_clock;

    std::vector<Boid> m_boids;

public:
    Flock();
    virtual ~Flock();

    const bool running() const;

    void pollEvents();
    void update();
    void render();
};
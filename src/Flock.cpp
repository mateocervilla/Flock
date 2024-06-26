#include "Flock.hpp"
#include <math.h>
#include <iostream>
#include <random>

Flock::Flock() {
    initVariables();
    initWindow();
    initEnviroment();
}

Flock::~Flock() {
}

void Flock::initVariables() {
    m_window = nullptr;
    m_isWindowFocused = true;

}

void Flock::initWindow() {
    m_videoMode.height = HEIGHT;
    m_videoMode.width = WIDTH;

    m_window = std::make_shared<sf::RenderWindow>(m_videoMode, "Flock", sf::Style::Close);
    m_window->setFramerateLimit(75);
}

void Flock::initEnviroment() {
    for(int i = 0; i < 100; i++) {
        m_boids.push_back(Boid(i, {WIDTH, HEIGHT}));
    }
}

void Flock::update() {
    pollEvents();

    for(int b = 0; b < m_boids.size(); b++) {
        m_boids[b].flock(m_boids);
        m_boids[b].update();
    }
}

void Flock::render() {

    /*
        - clear old frame
        - render objects
        - display frame in window

        Renders objects
    */

    m_window->clear(sf::Color::Black);

    for(int b = 0; b < m_boids.size(); b++) {
        m_boids[b].draw(*m_window);
    }

    m_window->display();
}

void Flock::pollEvents() {
    while (m_window->pollEvent(m_ev)) {
        switch (m_ev.type) {
            case sf::Event::Closed:
                m_window->close();
                break;
            // Window focus events
            case sf::Event::GainedFocus:
                m_isWindowFocused = true;
                break;
            case sf::Event::LostFocus:
                m_isWindowFocused = false;
                break;
        }
    }
}

const bool Flock::running() const {
    return m_window->isOpen();
}

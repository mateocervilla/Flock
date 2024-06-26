#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Boid : public sf::Drawable {
private:
    size_t m_id;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_acceleration;

    sf::Vector2f m_totalPos;
    sf::Vector2f m_totalVel;
    sf::Vector2f m_totalSep;
    float m_totalAlgNeighbours;
    float m_totalCohNeighbours;
    float m_totalSepNeighbours;

    sf::CircleShape m_boidShape;
    sf::Vector2f m_screenSize;

    void updateNeighbours(const std::vector<Boid>& boids);
    sf::Vector2f align(const std::vector<Boid>& boids);
    sf::Vector2f cohesion(const std::vector<Boid>& boids);
    sf::Vector2f separation(const std::vector<Boid>& boids);
    void checkEdges();

    float m_maxForce;
    float m_maxSpeed;
    void setAcceleration(sf::Vector2f acc);
    void setMaxMagnitude(sf::Vector2f &vect, float maxMag);
    void setMagnitude(sf::Vector2f &vect, float mag);

public:
    Boid(size_t id, sf::Vector2f screenSize);
    virtual ~Boid();

    void update();
    void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

    size_t getId();
    sf::Vector2f getPosition();
    sf::Vector2f getVelocity();

    void flock(const std::vector<Boid>& boids);

};
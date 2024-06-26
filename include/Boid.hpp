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

    void updateNeighbours(const std::vector<Boid>& boids, const std::vector<size_t>& ids);
    sf::Vector2f align();
    sf::Vector2f cohesion();
    sf::Vector2f separation();
    void checkEdges();

    const float m_maxForce;
    const float m_maxSpeed;
    const float m_perceptionRadiusAlg;
    const float m_perceptionRadiusCoh;
    const float m_perceptionRadiusSep;
    const float m_maxRadius;
    void setAcceleration(sf::Vector2f acc);
    void setMaxMagnitude(sf::Vector2f &vect, float maxMag);
    void setMagnitude(sf::Vector2f &vect, float mag);
    void flockInt(const std::vector<Boid>& boids, const std::vector<size_t>& ids);

public:
    Boid(size_t id, sf::Vector2f screenSize);
    virtual ~Boid();

    void update();
    void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

    size_t getId() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    float getMaxRadius() const;

    void flock(const std::vector<Boid>& boids, const std::vector<size_t>& ids);
    void flock(const std::vector<Boid>& boids);

};
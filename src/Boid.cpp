#include "Boid.hpp"

#include <random>
#include <iostream>

#define RADIUS 2.5f

float calculateMagnitude(const sf::Vector2f& vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float calculateDistance(const sf::Vector2f& point1, const sf::Vector2f& point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    return calculateMagnitude({dx, dy});
}

Boid::Boid(size_t id, sf::Vector2f screenSize)
: m_id(id), m_screenSize(screenSize)
{
    // Shape props
    m_boidShape.setOrigin(RADIUS/2,RADIUS/2);
    m_boidShape.setRadius(RADIUS);
    m_boidShape.setPosition(m_position);
    m_boidShape.setFillColor(sf::Color::White);


    // Initialize random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> randPosX(0, 1800);
    std::uniform_real_distribution<float> randPosY(0, 800);
    std::uniform_real_distribution<float> randVel(-5, 5);
    // std::uniform_real_distribution<float> disY(0, 20);

    // Initial position
    m_position = {randPosX(gen),randPosY(gen)};

    m_maxForce = 1;
    m_maxSpeed = 4;
    m_velocity = {randVel(gen),randVel(gen)};
    m_acceleration = {0,0};
}

Boid::~Boid() {
}

void Boid::update() {
    m_position += m_velocity;
    checkEdges();
    m_velocity += m_acceleration;
    setMaxMagnitude(m_velocity, m_maxSpeed);
    m_boidShape.setPosition(m_position);
}

void Boid::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    window.draw(m_boidShape);
}

size_t Boid::getId()
{
    return m_id;
}

sf::Vector2f Boid::getPosition()
{
    return m_position;
}

sf::Vector2f Boid::getVelocity()
{
    return m_velocity;
}

void Boid::checkEdges() {
    if (m_position.x > m_screenSize.x) {
        m_position.x -= m_screenSize.x;
    } else if (m_position.x < 0) {
        m_position.x += m_screenSize.x;
    }
    if (m_position.y > m_screenSize.y) {
        m_position.y -= m_screenSize.y;
    } else if (m_position.y < 0) {
        m_position.y += m_screenSize.y;
    }
}

sf::Vector2f Boid::align(const std::vector<Boid>& boids) {
    const float perceptionRadius = 100;
    sf::Vector2f steering = {0,0};
    float total = 0;
    for(auto other : boids) {
        if (other.getId() == m_id) continue;
        float dist = calculateDistance(m_position, other.getPosition());
        if (dist < perceptionRadius) {
            steering += other.getVelocity();
            total++;
        }
    }

    if(total > 0) {
        steering /= total;
        setMagnitude(steering, m_maxSpeed);
        steering -= m_velocity;
        setMaxMagnitude(steering, m_maxForce);
    }

    return steering;
}

sf::Vector2f Boid::cohesion(const std::vector<Boid>& boids) {
    const float perceptionRadius = 100;
    sf::Vector2f steering = {0,0};
    float total = 0;
    for(auto other : boids) {
        if (other.getId() == m_id) continue;
        sf::Vector2f otherPos = other.getPosition();
        float dist = calculateDistance(m_position, otherPos);
        if (dist < perceptionRadius) {
            steering += otherPos;
            total++;
        }
    }

    if(total > 0) {
        steering /= total;
        steering -= m_position;
        setMagnitude(steering, m_maxSpeed);
        steering -= m_velocity;
        setMaxMagnitude(steering, m_maxForce);
    }

    return steering;
}

sf::Vector2f Boid::separation(const std::vector<Boid>& boids) {
    const float perceptionRadius = 50;
    sf::Vector2f steering = {0,0};
    float total = 0;
    for(auto other : boids) {
        if (other.getId() == m_id) continue;
        sf::Vector2f otherPos = other.getPosition();
        float dist = calculateDistance(m_position, otherPos);
        if (dist < perceptionRadius) {
            steering += (m_position - otherPos) / (dist * dist);
            total++;
        }
    }

    if(total > 0) {
        steering /= total;
        setMagnitude(steering, m_maxSpeed);
        steering -= m_velocity;
        setMaxMagnitude(steering, m_maxForce);
    }

    return steering;
}

void Boid::setMaxMagnitude(sf::Vector2f &vect, float maxMag)
{
    float mag = calculateMagnitude(vect);
    if(mag > maxMag) {
        vect *= maxMag / mag;
    }
}

void Boid::setMagnitude(sf::Vector2f &vect, float mag)
{
    vect *= mag / calculateMagnitude(vect);
}

void Boid::setAcceleration(sf::Vector2f acc)
{
    m_acceleration = acc;
    setMaxMagnitude(m_acceleration, m_maxForce);
}

void Boid::flock(const std::vector<Boid>& boids) {
    m_acceleration = {0,0};
    m_acceleration += align(boids);
    m_acceleration += cohesion(boids);
    m_acceleration += 1.2f * separation(boids);
}
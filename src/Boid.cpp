#include "Boid.hpp"

#include <random>
#include <iostream>
#include <numeric>

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
: m_id(id), m_screenSize(screenSize),
m_maxForce(1),
m_maxSpeed(3),
m_perceptionRadiusAlg(50),
m_perceptionRadiusCoh(50),
m_perceptionRadiusSep(25),
m_maxRadius(std::max(std::max(m_perceptionRadiusAlg, m_perceptionRadiusCoh),m_perceptionRadiusSep))
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

    // Initial position
    m_position = {randPosX(gen),randPosY(gen)};
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

size_t Boid::getId() const
{
    return m_id;
}

sf::Vector2f Boid::getPosition() const
{
    return m_position;
}

sf::Vector2f Boid::getVelocity() const
{
    return m_velocity;
}

float Boid::getMaxRadius() const
{
    return m_maxRadius;
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

void Boid::updateNeighbours(const std::vector<Boid>& boids, const std::vector<size_t>& ids) {
    m_totalVel = {0,0};
    m_totalPos = {0,0};
    m_totalSep = {0,0};
    m_totalAlgNeighbours = 0;
    m_totalCohNeighbours = 0;
    m_totalSepNeighbours = 0;

    for(const auto id : ids) {
        if (id == m_id) continue;
        float dist = calculateDistance(m_position, boids[id].getPosition());
        if (dist < m_perceptionRadiusAlg) {
            m_totalVel += boids[id].getVelocity();
            m_totalAlgNeighbours++;
        }
        if (dist < m_perceptionRadiusCoh) {
            m_totalPos += boids[id].getPosition();
            m_totalCohNeighbours++;
        }
        if (dist < m_perceptionRadiusSep) {
            m_totalSep += (m_position - boids[id].getPosition()) / (dist * dist);
            m_totalSepNeighbours++;
        }
    }
}

sf::Vector2f Boid::align() {
    sf::Vector2f steering = {0,0};

    if(m_totalAlgNeighbours > 0) {
        steering = m_totalVel / m_totalAlgNeighbours;
        setMagnitude(steering, m_maxSpeed);
        steering -= m_velocity;
        setMaxMagnitude(steering, m_maxForce);
    }

    return steering;
}

sf::Vector2f Boid::cohesion() {
    sf::Vector2f steering = {0,0};

    if(m_totalCohNeighbours > 0) {
        steering = m_totalPos / m_totalCohNeighbours;
        steering -= m_position;
        setMagnitude(steering, m_maxSpeed);
        steering -= m_velocity;
        setMaxMagnitude(steering, m_maxForce);
    }

    return steering;
}

sf::Vector2f Boid::separation() {
    sf::Vector2f steering = {0,0};

    if(m_totalSepNeighbours > 0) {
        steering = m_totalSep / m_totalSepNeighbours;
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

void Boid::flockInt(const std::vector<Boid>& boids, const std::vector<size_t>& ids) {
    updateNeighbours(boids, ids);
    m_acceleration = {0,0};
    m_acceleration += align();
    m_acceleration += cohesion();
    m_acceleration += 1.2f * separation();
}

void Boid::flock(const std::vector<Boid>& boids, const std::vector<size_t>& ids) {
    if(!ids.empty()){
        flockInt(boids, ids);
    }
}

void Boid::flock(const std::vector<Boid>& boids) {
    std::vector<size_t> ids(boids.size());
    std::iota(std::begin(ids), std::end(ids), 0);
    flockInt(boids, ids);
}

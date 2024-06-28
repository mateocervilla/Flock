#include "QuadTree.hpp"
#include <iostream>
#include <cmath>

Rectangle::Rectangle(sf::Vector2f position, sf::Vector2f size)
: m_position(position), m_size(size) {
    m_rectangle = sf::RectangleShape(size);
    m_rectangle.setOrigin(size.x / 2, size.y / 2);
    m_rectangle.setFillColor(sf::Color::Transparent);
    m_rectangle.setOutlineColor(sf::Color::Green);
    m_rectangle.setOutlineThickness(2);
    m_rectangle.setPosition(position.x, position.y);
}

Rectangle::~Rectangle(){}

bool Rectangle::contains(const sf::Vector2f& point) const {
    return (point.x >= m_position.x - m_size.x / 2  &&
            point.x < m_position.x + m_size.x / 2 &&
            point.y >= m_position.y - m_size.y / 2 &&
            point.y < m_position.y + m_size.y / 2);
}

bool Rectangle::intersects(const Rectangle& rect) const {
    return !(rect.getPosition().x - rect.getSize().x > m_position.x + m_size.x ||
             rect.getPosition().x + rect.getSize().x < m_position.x - m_size.x ||
             rect.getPosition().y - rect.getSize().y > m_position.y + m_size.y ||
             rect.getPosition().y + rect.getSize().y < m_position.y - m_size.y);
}

bool Rectangle::intersects(const Circle& circle) const {
    // Calculate the half dimensions of the rectangle
    float halfWidth = m_size.x / 2.0f;
    float halfHeight = m_size.y / 2.0f;

    // Find the closest point on the rectangle to the circle's center
    float closestX = std::max(m_position.x - halfWidth, std::min(circle.getPosition().x, m_position.x + halfWidth));
    float closestY = std::max(m_position.y - halfHeight, std::min(circle.getPosition().y, m_position.y + halfHeight));

    // Calculate the distance between the circle's center and this closest point
    float distanceX = circle.getPosition().x - closestX;
    float distanceY = circle.getPosition().y - closestY;

    // Calculate the square of the distance and compare it with the square of the radius
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared <= (circle.getRadius() * circle.getRadius());
}

void Rectangle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    window.draw(m_rectangle);
}

Circle::Circle(sf::Vector2f position, float radius)
: m_position(position), m_radius(radius) {
    m_circle = sf::CircleShape(radius);
    m_circle.setOrigin(radius, radius);
    m_circle.setPosition(position.x, position.y);
}

Circle::~Circle(){}

bool Circle::contains(const sf::Vector2f& point) const {
    float distance = std::sqrt(std::pow(point.x - m_position.x, 2) + std::pow(point.y - m_position.y, 2));
    // Check if the distance is less than or equal to the radius
    return distance <= m_radius;
}

void Circle::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    window.draw(m_circle);
}

QuadTree::QuadTree(const Rectangle& boundary, size_t capacity)
: m_boundary(boundary), m_capacity(capacity), m_divided(false)
{
}

QuadTree::~QuadTree() {
}

void QuadTree::subdivide() {
    const float x = m_boundary.getPosition().x;
    const float y = m_boundary.getPosition().y;
    const float w = m_boundary.getSize().x;
    const float h = m_boundary.getSize().y;
    const Rectangle ne = Rectangle({x + w / 4, y - h / 4}, {w / 2, h / 2});
    m_northeast = std::make_unique<QuadTree>(ne, m_capacity);
    const Rectangle nw = Rectangle({x - w / 4, y - h / 4}, {w / 2, h / 2});
    m_northwest = std::make_unique<QuadTree>(nw, m_capacity);
    const Rectangle se = Rectangle({x + w / 4, y + h / 4}, {w / 2, h / 2});
    m_southeast = std::make_unique<QuadTree>(se, m_capacity);
    const Rectangle sw = Rectangle({x - w / 4, y + h / 4}, {w / 2, h / 2});
    m_southwest = std::make_unique<QuadTree>(sw, m_capacity);
    m_divided = true;
}

bool QuadTree::insert(sf::Vector2f point, size_t index) {

    if (!m_boundary.contains(point)) {
        return false;
    }

    if (m_boidPoints.size() < m_capacity) {
        m_boidPoints.push_back({point, index});
        return true;
    }
    else {
        if (!m_divided) {
            subdivide();
        }
        if (m_northeast->insert(point, index)) {
            return true;
        } else if (m_northwest->insert(point, index)) {
            return true;
        } else if (m_southeast->insert(point, index)) {
            return true;
        } else if (m_southwest->insert(point, index)) {
            return true;
        }
    }
    return false;
}

void QuadTree::query(const Circle& range, const size_t id, std::vector<size_t>& found) {
    if (!m_boundary.intersects(range)) {
      return;
    }
    for (const auto& p : m_boidPoints) {
        if (p.id != id && range.contains(p.position)) {
            found.push_back(p.id);
        }
    }
    if (m_divided) {
        m_northwest->query(range, id, found);
        m_northeast->query(range, id, found);
        m_southwest->query(range, id, found);
        m_southeast->query(range, id, found);
    }
}

void QuadTree::draw(sf::RenderTarget& window, sf::RenderStates states) const {
    m_boundary.draw(window);

    if (m_divided) {
        m_northeast->draw(window);
        m_northwest->draw(window);
        m_southeast->draw(window);
        m_southwest->draw(window);
    }
}

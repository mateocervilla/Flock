#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct BoidPoint {
    sf::Vector2f position;
    size_t id;
};

class Circle : public sf::Drawable {
private:
    sf::Vector2f m_position;
    float m_radius;
    sf::CircleShape m_circle;
public:
    Circle(sf::Vector2f position, float radius);
    ~Circle();

    sf::Vector2f getPosition() const {return m_position;}
    float getRadius() const {return m_radius;}
    void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

    bool contains(sf::Vector2f point);
};

class Rectangle : public sf::Drawable {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::RectangleShape m_rectangle;
public:
    Rectangle(sf::Vector2f position, sf::Vector2f size);
    ~Rectangle();

    sf::Vector2f getPosition() const {return m_position;}
    sf::Vector2f getSize() const {return m_size;}
    void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;

    bool intersects(Rectangle rect);
    bool intersects(Circle circle);
    bool contains(sf::Vector2f point);
};

class QuadTree : public sf::Drawable {
private:
    Rectangle m_boundary;
    size_t m_capacity;
    bool m_divided;
    std::vector<BoidPoint> m_boidPoints;

    std::unique_ptr<QuadTree> m_northeast;
    std::unique_ptr<QuadTree> m_northwest;
    std::unique_ptr<QuadTree> m_southeast;
    std::unique_ptr<QuadTree> m_southwest;

    std::shared_ptr<sf::RenderWindow> m_window;

public:
    QuadTree(const Rectangle& boundary, size_t capacity);
    ~QuadTree();

    void subdivide();
    bool insert(sf::Vector2f point, size_t index);
    void query(Circle range, std::vector<size_t>& found);
    void draw(sf::RenderTarget& window, sf::RenderStates states = sf::RenderStates::Default) const override;
};
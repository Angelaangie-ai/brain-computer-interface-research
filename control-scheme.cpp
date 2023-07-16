#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

/**
  Abstract base class for all control scheme elements
*/
class ControlElement {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void update() = 0;
    virtual void handleInput(sf::Event event) = 0;
};

/**
  Rectangle class that inherits the base class
*/
class Rectangle : public ControlElement {
public:
    Rectangle(float x, float y, float width, float height, sf::Color color)
        : shape(sf::Vector2f(width, height)) 
  //initializes the rectangle shape
  {
        shape.setPosition(x, y);
        shape.setFillColor(color);
        selected = false;
    }

  //overriding the draw function
    void draw(sf::RenderWindow& window) override {
        if (selected) {
            shape.setOutlineThickness(2.f);
            shape.setOutlineColor(sf::Color::Red);
        }
        else {
            shape.setOutlineThickness(0.f);
        }

        window.draw(shape);
    }

  /**
    Processing user events and updating the state of the rectangle
  */
    void handleInput(sf::Event event) override {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
            if (shape.getGlobalBounds().contains(mousePosition)) {
                selected = !selected;
            }
        }
    }

private:
    sf::RectangleShape shape;
    bool selected;
};

// Control scheme diagram class
class ControlSchemeDiagram {
public:
    ControlSchemeDiagram() {
        setupElements();
    }

    void run() {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Control Scheme Diagram");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                handleInput(event);
            }

            update();

            window.clear();
            draw(window);
            window.display();
        }
    }

private:
    std::unordered_map<std::string, std::unique_ptr<ControlElement>> elements;

    void setupElements() {
        elements["start_stop"] = std::make_unique<Rectangle>(35.f, 35.f, 30.f, 30.f, sf::Color::White);
        elements["left"] = std::make_unique<Rectangle>(0.f, 35.f, 30.f, 30.f, sf::Color::White);
        elements["right"] = std::make_unique<Rectangle>(70.f, 35.f, 30.f, 30.f, sf::Color::White);
        elements["forward"] = std::make_unique<Rectangle>(35.f, 70.f, 30.f, 30.f, sf::Color::White);
        elements["backward"] = std::make_unique<Rectangle>(35.f, 0.f, 30.f, 30.f, sf::Color::White);
        elements["powerOn"] = std::make_unique<Rectangle>(14.f, 9.f, 12.f, 12.f, sf::Color::White);
        elements["powerOff"] = std::make_unique<Rectangle>(0.f, 9.f, 12.f, 12.f, sf::Color(165, 20, 47));
        elements["forwardIdle"] = std::make_unique<Rectangle>(81.f, 15.f, 12.f, 12.f, sf::Color::White);
        elements["backwardIdle"] = std::make_unique<Rectangle>(81.f, 0.f, 12.f, 12.f, sf::Color::White);
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& [name, element] : elements) {
            element->draw(window);
        }
    }

    void update() {
        for (const auto& [name, element] : elements) {
            element->update();
        }
    }

    void handleInput(sf::Event event) {
        for (const auto& [name, element] : elements) {
            element->handleInput(event);
        }
    }
};

int main() {
    ControlSchemeDiagram controlScheme;
    controlScheme.run();

    return 0;
}
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include <random>
#include <iostream>

constexpr auto PI = 3.141592653;
float xChange = -0.4f, yChange = 0.0f;
std::random_device rd;

double randomNumberGenerator(double start, double end) {
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dist(start, end);
    return dist(engine);
}

inline bool isBetween(float start, float value, float end) { return (value >= start) && (value <= end); }

bool areTouching(sf::CircleShape& ball, sf::RectangleShape& rect) {
    float xBall = ball.getPosition().x, yBall = ball.getPosition().y, dBall = 2 * ball.getRadius();
    float xRect = rect.getPosition().x, yRect = rect.getPosition().y, wRect = rect.getSize().x, lRect = rect.getSize().y;
    if((isBetween(xRect, xBall + dBall, xRect + wRect) || isBetween(xRect, xBall, xRect + wRect)) &&
        (isBetween(yRect, yBall + dBall, yRect + lRect) || isBetween(yRect, yBall, yRect + lRect))) {
        return true;
    }
    return false;
}

void bounce(sf::CircleShape& ball, sf::Angle& angle) {
    xChange *= -1;
    if(!angle.asRadians() || angle.asRadians() == PI / 2) {
        angle = sf::radians(randomNumberGenerator(PI / 8, 7 * PI / 8));
    }
    else {
        if(isBetween(0, angle.asRadians(), PI / 2)) {
            angle = sf::radians(randomNumberGenerator(13 * PI / 24, 5 * PI / 6));
        }
        else if(isBetween(PI / 2, angle.asRadians(), PI)) {
            angle = sf::radians(randomNumberGenerator(PI / 24, 5 * PI / 12));
        }
    }
    yChange = xChange / tan(angle.asRadians());
}

int main()
{
    unsigned int width = 2000, length = 1500;
    sf::RenderWindow window(sf::VideoMode({ width, length }), "Pong", sf::Style::Default);

    sf::RectangleShape edge1({ 10, float(length) }), edge2 = edge1, edge3({ float(width), 10 }), edge4 = edge3;
    edge1.setPosition({ -10, 0 });//left-side edge
    edge2.setPosition({ float(width + 10), 0 });//right-side edge
    edge3.setPosition({ 0, -10 });//upper edge
    edge4.setPosition({ 0, float(length + 10) });//lower edge

    sf::RectangleShape p1({ 100, 500 })
        , p2({ 100, 500 });
    p1.setPosition({ 0, 500 });
    p1.setFillColor(sf::Color::White);
    p2.setPosition({ 1900, 500 });
    p2.setFillColor(sf::Color::White);
    float pVelocity = 0.23f;

    sf::CircleShape ball(50.0f);
    ball.setFillColor(sf::Color::Red);
    ball.setPosition({ 950, 750 });

    sf::Angle angle(sf::radians(0));

    int score[2] = { 0, 0 };
    sf::Font font("arial.ttf");
    sf::String str1 = std::to_string(score[0]), str2 = std::to_string(score[1]);
    sf::Text p1Score(font, str1), p2Score(font, str2);
    p1Score.setPosition({ 900, 0 });
    p1Score.setCharacterSize(40);
    p2Score.setPosition({ 1100, 0 });
    p2Score.setCharacterSize(40);
    while(window.isOpen()) {
        window.clear();
        while(const auto event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            p1.move({ 0, -1 * pVelocity });
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            p1.move({ 0, pVelocity });
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            p2.move({ 0, -1 * pVelocity });
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            p2.move({ 0, pVelocity });
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            window.close();
        }
        if(areTouching(ball, p1) || areTouching(ball, p2)) { bounce(ball, angle); }
        if(areTouching(ball, edge3) || areTouching(ball, edge4)) yChange *= -1;
        if(areTouching(ball, edge1)) {
            score[1]++;
            p2Score.setString(std::to_string(score[1]));
            ball.setPosition({ 950, 750 });
            xChange *= -1;
            yChange = 0;
        }
        else if(areTouching(ball, edge2)) {
            score[0]++;
            p1Score.setString(std::to_string(score[0]));
            ball.setPosition({ 950, 750 });
            xChange *= -1;
            yChange = 0;
        }
        ball.move({ xChange, yChange });
        window.draw(p1);
        window.draw(p2);
        window.draw(ball);
        window.draw(p1Score);
        window.draw(p2Score);
        window.display();

    }
}
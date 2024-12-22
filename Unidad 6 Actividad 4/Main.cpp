#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Estructura para los objetivos en movimiento
struct Target {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    Target(sf::Texture& texture, sf::Vector2f position, sf::Vector2f velocity) : sprite(texture), velocity(velocity) {
        sprite.setPosition(position);
        sprite.setScale(0.1f, 0.1f); // Ajusta esta escala según el tamaño de tu imagen
    }
};

// Función para dibujar un dígito en la pantalla
void drawDigit(sf::RenderWindow& window, int digit, float x, float y) {
    float size = 10.0f; // Tamaño del dígito
    sf::RectangleShape segment(sf::Vector2f(size, size / 2));
    segment.setFillColor(sf::Color::Red);

    // Configuraciones de los segmentos para cada dígito
    std::vector<std::vector<bool>> digitSegments = {
        { true, true, true, true, true, true, false }, // 0
        { false, true, true, false, false, false, false }, // 1
        { true, true, false, true, true, false, true }, // 2
        { true, true, true, true, false, false, true }, // 3
        { false, true, true, false, false, true, true }, // 4
        { true, false, true, true, false, true, true }, // 5
        { true, false, true, true, true, true, true }, // 6
        { true, true, true, false, false, false, false }, // 7
        { true, true, true, true, true, true, true }, // 8
        { true, true, true, true, false, true, true } // 9
    };

    // Dibujar segmentos horizontales y verticales
    std::vector<sf::Vector2f> positions = {
        { x, y }, // Top
        { x + size, y + size / 2 }, // Top right
        { x + size, y + 1.5f * size }, // Bottom right
        { x, y + 2 * size }, // Bottom
        { x - size, y + 1.5f * size }, // Bottom left
        { x - size, y + size / 2 }, // Top left
        { x, y + size } // Middle
    };

    for (int i = 0; i < 7; ++i) {
        if (digitSegments[digit][i]) {
            segment.setPosition(positions[i]);
            window.draw(segment);
        }
    }
}

// Función para dibujar el puntaje utilizando dígitos
void drawScore(sf::RenderWindow& window, int score) {
    float x = 10.0f;  // Posición inicial en x
    float y = 10.0f;  // Posición inicial en y
    std::string scoreStr = std::to_string(score);

    for (char c : scoreStr) {
        int digit = c - '0';
        drawDigit(window, digit, x, y);
        x += 50.0f; //espacio entre los dígitos
    }
}

const float GRAVITY = 981.0f;  // Gravedad en píxeles por segundo al cuadrado

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Juego de Disparos Cinemáticos");

    sf::Texture texture;
    if (!texture.loadFromFile("C:/Users/Administrator/source/repos/Unidad 6 Actividad 4/beach-ball-hd-png-image-41208-1233.png")) {
        std::cerr << "Error al cargar la imagen\n";
        return -1;
    }

    std::vector<Target> targets;
    sf::Clock clock;

    int score = 0;

    srand(static_cast<unsigned int>(time(nullptr)));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (auto it = targets.begin(); it != targets.end(); ++it) {
                    if (it->sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        targets.erase(it);
                        score++;
                        break;
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        if (rand() % 100 < 10 && targets.size() < 20) { // Limitar el número de objetos en pantalla
            sf::Vector2f position(rand() % window.getSize().x, 0);
            sf::Vector2f velocity(rand() % 200 - 100, 100.0f + rand() % 200); // MRUV
            targets.emplace_back(texture, position, velocity);
        }

        for (auto& target : targets) {
            target.velocity.y += GRAVITY * dt; // Aplicar gravedad (MRUV)
            target.sprite.move(target.velocity * dt);

            // Rebote en el suelo
            if (target.sprite.getPosition().y + target.sprite.getGlobalBounds().height >= window.getSize().y) {
                target.velocity.y *= -0.7f;
                target.sprite.setPosition(target.sprite.getPosition().x, window.getSize().y - target.sprite.getGlobalBounds().height);
            }
        }

        window.clear();

        for (const auto& target : targets) {
            window.draw(target.sprite);
        }

        drawScore(window, score);

        window.display();
    }

    return 0;
}


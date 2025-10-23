#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Config.h"
#include "Interfaces.h"
#include "Entidade.h"
#include "Atrib.h"

int main() {
    const int W = Config::GRID_W;
    const int H = Config::GRID_H;
    const int C = Config::CS;

    sf::RenderWindow window(
        sf::VideoMode(static_cast<unsigned>(W*C), static_cast<unsigned>(H*C)),
        "Chromasnake Arena (Base)",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    Grid grid(W, H, C);

    sf::Clock clock;
    float t = 0.f;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) window.close();
        }

        float dt = clock.restart().asSeconds();
        t += dt;

        window.clear(sf::Color(15,15,15));
        grid.drawBackground(window);
        window.display();
    }
    return 0;
}

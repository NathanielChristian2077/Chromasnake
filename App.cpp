#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Config.h"
#include "Game.h"

int main() {
    const int W = Config::GRID_W, H = Config::GRID_H, C = Config::CS;
    sf::RenderWindow window(
        sf::VideoMode((unsigned)(W*C), (unsigned)(H*C)),
        "Chromasnake Arena", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    Grid grid(W,H,C);
    Game game(grid);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type==sf::Event::Closed) window.close();
            if (e.type==sf::Event::KeyPressed && e.key.code==sf::Keyboard::Escape) window.close();
        }
        float dt = clock.restart().asSeconds();
        game.update(dt);

        window.clear(sf::Color(15,15,15));
        game.draw(window);
        window.display();
    }
    return 0;
}


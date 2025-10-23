#pragma once
#include <SFML/Graphics.hpp>

class Grid {
public:
    Grid(int w, int h, int cellSize);

    sf::Vector2i wrap(const sf::Vector2i& c) const;
    sf::Vector2f toPixels(const sf::Vector2i& c) const;

    int width() const{
	return W;
    };

    int height() const {
	return H;
    };

    int cell() const {
	return CS;
    };

    void drawBackground(sf::RenderWindow& win) const;

private:
    int W;
    int H;
    int CS;
};

#include "Grid.h"

Grid::Grid(int w, int h, int cs): W(w), H(h), CS(cs) {}

sf::Vector2i Grid::wrap(const sf::Vector2i& c_) const {
    sf::Vector2i c = c_;

    c.x = (c.x % W + W) % W;
    c.y = (c.y % H + H) % H;
    return c;
};

sf::Vector2f Grid::toPixels(const sf::Vector2i& c) const {
    return sf::Vector2f(static_cast<float>(c.x * CS), static_cast<float>(c.y * CS));
};

void Grid::drawBackground(sf::RenderWindow& win) const {
    sf::RectangleShape cellRect(sf::Vector2f(static_cast<float>(CS), static_cast<float>(CS)));
    cellRect.setFillColor(sf::Color(25,25,25));

    for (int y = 0; y < H; ++y) {
	for (int x = 0; x < W; ++x) {
	    cellRect.setPosition(toPixels({x, y}));
	    win.draw(cellRect);
	}
    }
};

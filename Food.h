#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "RNG.h"

class Food {
public:
    explicit Food(const Grid& g);

    void ensureCount(int n, const std::vector<std::vector<int>>& occ);
    bool consumeAt(const sf::Vector2i& cell);
    void draw(sf::RenderWindow& w) const;

    const std::vector<sf::Vector2i>& list() const {
	return foods; 
    }

private:
    sf::Vector2i randomFreeCell(const std::vector<std::vector<int>>& occ);

    const Grid& grid;
    std::vector<sf::Vector2i> foods;
};


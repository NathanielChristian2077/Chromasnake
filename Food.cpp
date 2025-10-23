#include "Food.h"
#include <algorithm>

Food::Food(const Grid& g) : grid(g) {}

void Food::ensureCount(int n, const std::vector<std::vector<int>>& occ){
    while ((int)foods.size() < n) foods.push_back(randomFreeCell(occ));
}

bool Food::consumeAt(const sf::Vector2i& cell){
    auto it = std::find(foods.begin(), foods.end(), cell);
    if (it!=foods.end()){ foods.erase(it); return true; }
    return false;
}

void Food::draw(sf::RenderWindow& w) const{
    sf::RectangleShape r({(float)grid.cell(), (float)grid.cell()});
    r.setFillColor(sf::Color(80,255,80));
    for (auto& c: foods){
	r.setPosition(grid.toPixels(c));
	w.draw(r); 
    }
}

sf::Vector2i Food::randomFreeCell(const std::vector<std::vector<int>>& occ){
    for(;;){
        int x = RNG::instance().irand(0, grid.width()-1);
        int y = RNG::instance().irand(0, grid.height()-1);
        if (occ[y][x]==0) return {x,y};
    }
}


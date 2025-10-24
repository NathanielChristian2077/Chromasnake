#pragma once
#include <memory>
#include <vector>
#include "Interfaces.h"
#include "Grid.h"
#include "Food.h"
#include "Snake.h"
#include "Config.h"

namespace sf {class RenderWindow;}

class Game : public Updatable, public Drawable {
public:
    explicit Game(const Grid& g);
    void reset();

    void update(float dt) override;
    void draw(sf::RenderWindow& w) const override;

private:
    template<class T> void spawn();
    void connectAll();
    void occupyInitialBodies();
    void evaluateEnd();

    const Grid& grid;
    Food food;
    int desiredFoods = 2;
    std::vector<std::unique_ptr<Entidade>> entities;
    std::vector<std::vector<int>> occupancy;
    bool gameOver = false;
    int winnerIndex = -1;
};


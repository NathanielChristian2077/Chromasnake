#include "Grid.h"
#include "Food.h"
#include "Snake.h"
#include "Config.h"
#include "Game.h"
#include <unordered_map>

Game::Game(const Grid& g) : grid(g), food(g) { reset(); }

void Game::reset(){
    entities.clear();
    occupancy.assign(grid.height(), std::vector<int>(grid.width(), 0));

    // Participantes
    spawn<RedSnake>();
    spawn<YellowSnake>();
    spawn<BlueSnake>();
    spawn<OrangeSnake>();   

    desiredFoods = std::max(2, (int)entities.size()/2);
    connectAll();
    occupyInitialBodies();
}

void Game::update(float dt){
    food.ensureCount(desiredFoods, occupancy);

    for (auto& e: entities) e->update(dt);

    std::unordered_map<long long,int> headClaims;
    for (size_t i=0;i<entities.size();++i){
        auto* s = dynamic_cast<Snake*>(entities[i].get());
        if (!s || !s->alive()) continue;
        s->lateStep(headClaims, occupancy, food);
    }
    for (size_t i=0;i<entities.size();++i){
        auto* s = dynamic_cast<Snake*>(entities[i].get());
        if (!s || !s->alive()) continue;
        s->resolveAndMove(headClaims, occupancy, (int)(i+1));
    }
}

void Game::draw(sf::RenderWindow& w) const{
    grid.drawBackground(w);
    food.draw(w);
    for (auto& e: entities) e->draw(w);
}

template<class T>
void Game::spawn(){
    auto s = std::make_unique<T>(grid);
    entities.emplace_back(std::move(s));
}

void Game::connectAll(){
    for (auto& e: entities){
        auto* s = dynamic_cast<Snake*>(e.get());
        if (!s) continue;
        s->connect(&food, &entities, &occupancy);
    }
}

void Game::occupyInitialBodies(){
    for (size_t i=0;i<entities.size();++i){
        auto* s = dynamic_cast<Snake*>(entities[i].get());
        if (!s) continue;
        for (auto& c : s->bodyCells()) {
            occupancy[c.y][c.x] = (int)(i+1);
        }
    }
}


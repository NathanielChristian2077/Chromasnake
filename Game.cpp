#include "Grid.h"
#include "Food.h"
#include "Snake.h"
#include "Config.h"
#include "Game.h"
#include <unordered_map>

Game::Game(const Grid &g) : grid(g), food(g) { reset(); }

void Game::reset()
{
    entities.clear();
    occupancy.assign(grid.height(), std::vector<int>(grid.width(), 0));
    gameOver = false;
    winnerIndex = -1;

    // Participantes
    spawn<RedSnake>();
    spawn<YellowSnake>();
    spawn<BlueSnake>();
    spawn<OrangeSnake>();

    desiredFoods = std::max(2, (int)entities.size() / 2);
    connectAll();
    occupyInitialBodies();
}

void Game::update(float dt)
{
    if (gameOver)
        return;

    food.ensureCount(desiredFoods, occupancy);

    for (auto &e : entities)
        e->update(dt);

    std::unordered_map<long long, int> headClaims;
    for (size_t i = 0; i < entities.size(); ++i)
    {
        auto *s = dynamic_cast<Snake *>(entities[i].get());
        if (!s || !s->alive())
            continue;
        s->lateStep(headClaims, occupancy, food);
    }
    for (size_t i = 0; i < entities.size(); ++i)
    {
        auto *s = dynamic_cast<Snake *>(entities[i].get());
        if (!s || !s->alive())
            continue;
        s->resolveAndMove(headClaims, occupancy, (int)(i + 1));
    }

    evaluateEnd();
}

void Game::draw(sf::RenderWindow &w) const
{
    grid.drawBackground(w);
    food.draw(w);
    for (auto &e : entities)
        e->draw(w);

    if (gameOver)
    {
        static sf::Font font;
        static bool loaded = font.loadFromFile("../assets/fonts/DejaVuSans.ttf");
        if (loaded && winnerIndex >= 0)
        {
            auto *s = dynamic_cast<Snake *>(entities[winnerIndex].get());
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(24);
            t.setFillColor(sf::Color::White);
            t.setString("Vencedor: " + std::string(s ? s->name() : "Snake"));
            t.setPosition(8.f, 6.f);
            w.draw(t);
        }
        else if (loaded && winnerIndex < 0)
        {
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(24);
            t.setFillColor(sf::Color::White);
            t.setString("Sem vencedores");
            t.setPosition(8.f, 6.f);
            w.draw(t);
        }
    }
}

template <class T>
void Game::spawn()
{
    auto s = std::make_unique<T>(grid);
    entities.emplace_back(std::move(s));
}

void Game::connectAll()
{
    for (auto &e : entities)
    {
        auto *s = dynamic_cast<Snake *>(e.get());
        if (!s)
            continue;
        s->connect(&food, &entities, &occupancy);
    }
}

void Game::occupyInitialBodies()
{
    for (size_t i = 0; i < entities.size(); ++i)
    {
        auto *s = dynamic_cast<Snake *>(entities[i].get());
        if (!s)
            continue;
        for (auto &c : s->bodyCells())
        {
            occupancy[c.y][c.x] = (int)(i + 1);
        }
    }
}

void Game::evaluateEnd()
{
    int aliveCount = 0;
    int lastIdx = -1;
    for (size_t i = 0; i < entities.size(); ++i)
    {
        if (entities[i]->alive())
        {
            aliveCount++;
            lastIdx = (int)i;
        }
    }
    if (aliveCount <= 1)
    {
        gameOver = true;
        winnerIndex = (aliveCount == 1) ? lastIdx : -1;
    }
}

#pragma once
#include <deque>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Entidade.h"
#include "Grid.h"
#include "Atrib.h"
#include "RNG.h"
#include <unordered_map>

class Food;

class Snake : public Entidade, public virtual Atrib {
public:
    enum class Dir { Up, Down, Left, Right };

    Snake(const Grid& g, sf::Color c);

    void connect(const Food* f,
                 const std::vector<std::unique_ptr<Entidade>>* ents,
                 const std::vector<std::vector<int>>* occ);

    // Entidade
    void update(float dt) override;
    void draw(sf::RenderWindow& w) const override;
    const char* name() const override { return "Snake"; }
    bool alive() const override { return alive_; }

    void lateStep(std::unordered_map<long long,int>& headClaims,
                  std::vector<std::vector<int>>& occ,
                  Food& foods);

    void resolveAndMove(const std::unordered_map<long long,int>& headClaims,
                        std::vector<std::vector<int>>& occ, int myId);

    // util
    sf::Vector2i head() const { return body.front(); }
    int length() const { return (int)body.size(); }
    const std::deque<sf::Vector2i>& bodyCells() const { return body; }

protected:
    // IA
    Dir chooseDirection();
    float scoreMove(Dir d);
    sf::Vector2i nextCell(Dir d) const;
    float forageTerm(const sf::Vector2i& nh) const;
    float rivalApproach(const sf::Vector2i& nh) const;
    float localRisk(const sf::Vector2i& nh) const;

    void kill(std::vector<std::vector<int>>& occ);
    static void remove(std::vector<Dir>& v, Dir d);

    const Grid& grid;
    sf::Color color;

    std::deque<sf::Vector2i> body;
    Dir dir = Dir::Right;
    Dir nextDir = Dir::Right;
    float baseTick = 0.12f;
    float tickAccum = 0.f;
    bool alive_ = true;
    bool growNext = false;
    bool dying = false;
    sf::Vector2i nextHead{0,0};

    const Food* food = nullptr;
    const std::vector<std::unique_ptr<Entidade>>* entities = nullptr;
    const std::vector<std::vector<int>>* occupancy = nullptr;
};

class Red; class Yellow; class Blue;
template<class... T> class Mix;

class RedSnake    : public Snake, public Red { public: explicit RedSnake(const Grid& g)    : Snake(g, sf::Color::Red) {} };
class YellowSnake : public Snake, public Yellow { public: explicit YellowSnake(const Grid& g) : Snake(g, sf::Color::Yellow) {} };
class BlueSnake   : public Snake, public Blue  { public: explicit BlueSnake(const Grid& g)  : Snake(g, sf::Color::Blue) {} };

class OrangeSnake : public Snake, public Mix<Red,Yellow> {
public: explicit OrangeSnake(const Grid& g) : Snake(g, sf::Color(255,165,0)) {}
};
class PurpleSnake : public Snake, public Mix<Red,Blue> {
public: explicit PurpleSnake(const Grid& g) : Snake(g, sf::Color(128,0,128)) {}
};
class LimeSnake   : public Snake, public Mix<Yellow,Blue> {
public: explicit LimeSnake(const Grid& g) : Snake(g, sf::Color(191,255,0)) {}
};
class WhiteSnake  : public Snake, public Mix<Red,Yellow,Blue> {
public: explicit WhiteSnake(const Grid& g) : Snake(g, sf::Color::White) {}
};


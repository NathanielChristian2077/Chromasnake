#include "Snake.h"
#include "Food.h"
#include "Atrib.h"
#include <algorithm>
#include <array>
#include <unordered_map>
#include <cmath>

Snake::Snake(const Grid& g, sf::Color c) : grid(g), color(c){
    // Ticks (local, pq eu achei mais prático)
    baseTick = 0.28f;
    int x = RNG::instance().irand(0, grid.width()-1);
    int y = RNG::instance().irand(0, grid.height()-1);
    body.push_front({x,y});
    body.push_back(grid.wrap({x-1,y}));
    body.push_back(grid.wrap({x-2,y}));
}

void Snake::connect(const Food* f,
                    const std::vector<std::unique_ptr<Entidade>>* ents,
                    const std::vector<std::vector<int>>* occ)
{
    food = f; entities = ents; occupancy = occ;
}

void Snake::update(float dt){
    if (!alive_) return;
    tickAccum += dt;
    float stepTime = baseTick / speedMult;
    if (tickAccum >= stepTime) {
        nextDir = chooseDirection();
        tickAccum = 0.f;
    }
}

void Snake::lateStep(std::unordered_map<long long,int>& headClaims,
                     std::vector<std::vector<int>>& occ,
                     Food& foods)
{
    if (!alive_) return;
    sf::Vector2i head = body.front();
    sf::Vector2i nh = head;
    switch(nextDir){
        case Dir::Up:    nh.y -= 1; break;
        case Dir::Down:  nh.y += 1; break;
        case Dir::Left:  nh.x -= 1; break;
        case Dir::Right: nh.x += 1; break;
    }
    nh = grid.wrap(nh);
    nextHead = nh;

    long long key = (long long)nh.y*100000 + nh.x;
    headClaims[key]++;

    if ((*occupancy)[nh.y][nh.x] > 0) dying = true;

    bool ate = foods.consumeAt(nh);
    if (ate) ++growPending;
}

void Snake::resolveAndMove(const std::unordered_map<long long,int>& headClaims,
                           std::vector<std::vector<int>>& occ, int myId)
{
    if (!alive_) return;
    long long key = (long long)nextHead.y*100000 + nextHead.x;
    auto it = headClaims.find(key);
    if (it!=headClaims.end() && it->second > 1) dying = true;

    if (dying) { kill(occ); return; }

    sf::Vector2i tail = body.back();
    if (growPending == 0) {
        occ[tail.y][tail.x] = 0;
        body.pop_back();
    } else {
        --growPending;
    }

    body.push_front(nextHead);
    occ[nextHead.y][nextHead.x] = myId;
}

void Snake::draw(sf::RenderWindow& w) const{
    if (!alive_) return;
    sf::RectangleShape r({(float)grid.cell(), (float)grid.cell()});
    for (size_t i=0;i<body.size();++i){
        auto c = color;
        if (i==0){
            c.r = std::min(255, int(c.r)+40);
            c.g = std::min(255, int(c.g)+40);
            c.b = std::min(255, int(c.b)+40);
        }
        r.setFillColor(c);
        r.setPosition(grid.toPixels(body[i]));
        w.draw(r);
    }
}

Snake::Dir Snake::chooseDirection(){
    std::vector<Dir> dirs = {Dir::Up,Dir::Down,Dir::Left,Dir::Right};
    if (body.size()>=2){
        auto neck = body[1];
        auto h = body[0];
        if (neck.y==h.y-1) remove(dirs,Dir::Up);
        if (neck.y==h.y+1) remove(dirs,Dir::Down);
        if (neck.x==h.x-1) remove(dirs,Dir::Left);
        if (neck.x==h.x+1) remove(dirs,Dir::Right);
    }
    float best = -1e9f; Dir bestDir = dir;
    for (auto d: dirs){
        float sc = scoreMove(d);
        if (sc > best){ best = sc; bestDir = d; }
    }
    dir = bestDir;
    return bestDir;
}

float Snake::scoreMove(Dir d){
    auto nh = nextCell(d);
    if ((*occupancy)[nh.y][nh.x] > 0) return -1e6f;

    float s = 0.f;
    s += forageTerm(nh);
    s += aggression * rivalApproach(nh);
    s -= caution   * localRisk(nh);
    s += RNG::instance().frand(-0.05f,0.05f);
    return s;
}

sf::Vector2i Snake::nextCell(Dir d) const{
    auto h = body.front();
    if (d==Dir::Up)    h.y -= 1;
    if (d==Dir::Down)  h.y += 1;
    if (d==Dir::Left)  h.x -= 1;
    if (d==Dir::Right) h.x += 1;
    return grid.wrap(h);
}

float Snake::forageTerm(const sf::Vector2i& nh) const{
    if (!food) return 0.f;
    int best = 1e9;
    for (auto& f: food->list()){
        int d = std::abs(f.x-nh.x)+std::abs(f.y-nh.y);
        if (d < best) best = d;
    }
    if (best>=1e9) return 0.f;
    if (best>vision) return 0.f;
    return 1.0f / (1 + (float)best);
}

float Snake::rivalApproach(const sf::Vector2i& nh) const{
    if (!entities) return 0.f;
    int best = 1e9;
    for (auto& e : *entities){
        if (e.get()==this || !e->alive()) continue;
        auto* s = dynamic_cast<Snake*>(e.get());
        if (!s) continue;
        auto h = s->head();
        int d = std::abs(h.x-nh.x)+std::abs(h.y-nh.y);
        if (d < best) best = d;
    }
    if (best>=1e9) return 0.f;
    return 1.0f / (1 + (float)best);
}

float Snake::localRisk(const sf::Vector2i& nh) const{
    int freeN = 0;
    std::array<sf::Vector2i,4> vs{sf::Vector2i{nh.x,nh.y-1}, {nh.x,nh.y+1}, {nh.x-1,nh.y}, {nh.x+1,nh.y}};
    for (auto v: vs){
        v = grid.wrap(v);
        if ((*occupancy)[v.y][v.x]==0) freeN++;
    }
    return float(3 - freeN);
}

void Snake::kill(std::vector<std::vector<int>>& occ){
    alive_ = false;
    for (auto& c: body) occ[c.y][c.x] = 0;
}

void Snake::remove(std::vector<Dir>& v, Dir d){
    v.erase(std::remove(v.begin(), v.end(), d), v.end());
}

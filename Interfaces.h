#pragma once

#ifdef sqrt
#	undef sqrt
#endif
class Updatable {
public:
    virtual void update(float dt) = 0;
    virtual ~Updatable() = default;
};

class Drawable {
public:
    virtual void draw(class sf::RenderWindow& w) const = 0;
    virtual ~Drawable() = default;
};

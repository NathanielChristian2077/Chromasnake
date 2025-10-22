#pragma once
#include <cmath>
#include "Config.h"

// Base dos atributos presentes em todos
class Atrib {
public:
    float speedMult = Config::SPEED_MULT_BASE;
    int vision = Config::VISION_BASE;
    float aggression = Config::AGGR_BASE;
    float caution = Config::CAUT_BASE;

    virtual ~Atrib() = default;
};

class Red : public virtual Atrib {
public:
    Red() {
	aggression += 1.0f;
	speedMult += 0.10f;
    }
};

class Yellow : public virtual Atrib {
public:
    Yellow() {
	vision += 4;
    }
};

class Blue : public virtual Atrib {
public:
    Blue() {
	caution += 1.0f;
	speedMult -= 0.05f;
    }
};

template<class... Atribs>
class Mix : public Atribs... {
public:
    Mix() {
	const float f = Config::WEAK_FACTOR;
	this->speedMult = 1.0f + (this->speedMult - 1.0f) * f;
	this->vision = std::max(1, static_cast<int>(std::lround(this->vision * f)));
	this->aggression *= f;
	this->caution *= f;
    }
};

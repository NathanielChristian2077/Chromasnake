#pragma once
#include "Interfaces.h"

class Entidade: public virtual Updatable, public virtual Drawable{
public:
    virtual const char* name() const = 0;
    virtual bool alive() const = 0;
    virtual ~Entidade() = default;
};

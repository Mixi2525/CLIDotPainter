#pragma once

#include "core/Vector2.hpp"

class IPrintable
{
public:
    virtual void print() const = 0;
    virtual void print(Vector2 offset) const = 0;
    virtual ~IPrintable() {}
};

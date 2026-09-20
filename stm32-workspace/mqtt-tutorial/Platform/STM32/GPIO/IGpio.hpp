#pragma once

#include <cstdint>

class IGpio
{
public:
    virtual ~IGpio() = default;

    virtual void write(bool state) = 0;
    virtual void toggle() = 0;
};
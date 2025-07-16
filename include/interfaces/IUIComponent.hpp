#pragma once

class IUIComponent
{
public:
    virtual ~IUIComponent() = default;

    virtual void print() = 0;
};

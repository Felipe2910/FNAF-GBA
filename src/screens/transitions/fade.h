#pragma once

#include "bn_color.h"

class Fade
{
public:
    void start_in();      // negro → visible
    void start_out();     // visible → negro

    void update();
    bool finished() const;

    bool active() const { return _active; }

private:
    int _alpha = 0;
    int _speed = 2;

    bool _active = false;
    bool _fade_in = true;
};
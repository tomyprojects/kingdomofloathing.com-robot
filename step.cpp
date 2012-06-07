#include "step.h"

Step::Step()
{
    _state = 0;
    _step = 0;
}

void Step::wait()
{
    _state = Step::WAIT;
}

void Step::next()
{
    _step++;
    _state = _step;
}

int Step::state()
{
    return _state;
}

void Step::go(int k)
{
    _state = k;
    _step = k;
}


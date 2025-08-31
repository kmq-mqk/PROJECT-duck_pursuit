#ifndef _LOGIC_HPP_
#define _LOGIC_HPP_

#include "object.h"
#include "raylib.h"

void InputMove(MobiObj* mobiObj, Maze* mazeInfo);
bool WinCheck(Vector2 objPos, Vector2 goalPos);
//void ResetVal();

#endif

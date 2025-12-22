#pragma once
#include <algorithm>
#include <cfloat>

inline float Clamp(float value, float min, float max){
    return std::max(min, std::min(max, value));
}
#pragma once
#include <algorithm>

inline float Clamp(float value, float min, float max){
    return std::max(min, std::min(max, value));
}
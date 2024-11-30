#pragma once

#include <iostream>
#include <string>
#include <vector>

// самая быстрая версия
std::vector<float> ComputeAvgTemp(const std::vector<std::vector<float>>& measures);
// самая худшая версия
std::vector<float> ComputeAvgTemp1(const std::vector<std::vector<float>>& measures);

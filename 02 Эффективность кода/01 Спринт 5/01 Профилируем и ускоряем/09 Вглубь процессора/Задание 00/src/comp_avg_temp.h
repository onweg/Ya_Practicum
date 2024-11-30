#pragma once

#include <iostream>
#include <string>
#include <vector>

// самая быстрая версия
std::vector<float> ComputeAvgTemp(const std::vector<std::vector<float>>& measures);
// много делений
std::vector<float> ComputeAvgTemp1(const std::vector<std::vector<float>>& measures);
// for if нет резервации 
std::vector<float> ComputeAvgTemp2(const std::vector<std::vector<float>>& measures);

#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

/*
vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    // место для вашего решения
}
*/

vector<float> ComputeAvgTemp_1(const vector<vector<float>>& measures) {
    // место для вашего решения
    size_t days = measures.size(), measurements = measures[0].size();
    vector<float> result;
    float mid = 0;
    for (size_t i = 0; i < measurements; ++i) {
        mid = 0;
        for (size_t j = 0; j < days; ++j) {
            if (measures[i][j] > 0) {
                mid += measures[i][j];
            }        
        }
        mid /= days;
        result.push_back(mid);
    }
    return result;
}

vector<float> ComputeAvgTemp_2(const vector<vector<float>>& measures) {
    // место для вашего решения
    size_t days = measures.size(), measurements = measures[0].size();
    vector<float> result(measurements);
    float mid = 0;
    for (size_t i = 0; i < measurements; ++i) {
        mid = 0;
        for (size_t j = 0; j < days; ++j) {
            if (measures[i][j] > 0) {
                mid += measures[i][j];
            }        
        }
        mid /= days;
        result[i] = mid;
    }
    return result;
}

vector<float> ComputeAvgTemp_3(const vector<vector<float>>& measures) {
    // место для вашего решения
    size_t days = measures.size(), measurements = measures[0].size();
    vector<float> result(measurements);
    float mid = 0;
    for (size_t i = 0; i < measurements; ++i) {
        mid = 0;
        for (size_t j = 0; j < days; ++j) {
            mid += measures[i][j] > 0 ? measures[i][j] : 0;     
        }
        mid /= days;
        result[i] = mid;
    }
    return result;
}

vector<float> ComputeAvgTemp_4(const vector<vector<float>>& measures) {
    // место для вашего решения
    size_t days = measures.size(), measurements = measures[0].size();
    vector<float> result(measurements);
    for (size_t i = 0; i < days; ++i) {
        for (size_t j = 0; j < measurements; ++j) {
            measures[i][j] > 0 ? result[j] += measures[i][j] / days : 0;     
        }
    }
    return result;
}

vector<float> ComputeAvgTemp_5(const vector<vector<float>>& measures) {
    // место для вашего решения
    size_t days = measures.size(), measurements = measures[0].size();
    vector<float> result(measurements);
    for (size_t i = 0; i < days; ++i) {
        for (size_t j = 0; j < measurements; ++j) {
            measures[i][j] > 0 ? result[j] += measures[i][j] : 0;     
        }
    }
    for (size_t i = 0; i < measurements; ++i) {
        result[i] /= days;
    }
    return result;
}

vector<float> ComputeAvgTemp_6(const vector<vector<float>>& measures) {
    // место для вашего решения
    size_t days = measures.size(), measurements = measures[0].size();
    vector<float> result(measurements);
    for (size_t i = 0; i < days; ++i) {
        for (size_t j = 0; j < measurements; ++j) {
            if (measures[i][j] > 0) {
                result[j] += measures[i][j];
            }
        }
    }
    for (size_t i = 0; i < measurements; ++i) {
        result[i] /= days;
    }
    return result;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

int main() {
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp_1"s);
            avg = ComputeAvgTemp_1(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp_2"s);
            avg = ComputeAvgTemp_2(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp_3"s);
            avg = ComputeAvgTemp_3(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp_4"s);
            avg = ComputeAvgTemp_4(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp_5"s);
            avg = ComputeAvgTemp_5(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp_6"s);
            avg = ComputeAvgTemp_6(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
}

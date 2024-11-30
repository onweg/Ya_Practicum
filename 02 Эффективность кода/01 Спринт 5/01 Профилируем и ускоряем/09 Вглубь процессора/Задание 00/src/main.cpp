#include "log_duration.h"
#include "comp_avg_temp.h"

#include <numeric>
#include <random>

using namespace std;

vector<float> GetRandomVector(int size) {
    mt19937 engine(static_cast<unsigned int>(time(0)));
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
            LOG_DURATION("ComputeAvgTemp"s);
            avg = ComputeAvgTemp(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    
    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp1"s);
            avg = ComputeAvgTemp1(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    {
        vector<float> avg;
        {
            LOG_DURATION("ComputeAvgTemp2"s);
            avg = ComputeAvgTemp2(data);
        }

        cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    }
    /*
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
    */
}


// правильный цикл
// тернарный оператор
// заранее резервировать память
// поменьше использовать деление


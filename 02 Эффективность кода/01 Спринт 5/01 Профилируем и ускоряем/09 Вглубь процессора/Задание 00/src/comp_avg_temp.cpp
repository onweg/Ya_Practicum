#include "comp_avg_temp.h"

using namespace std;

// самая быстрая версия
vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
	size_t days = measures.size(), measuremets = measures[0].size();
	vector<float> result(measures.size());
	for (size_t day = 0; day < days; day++) {
		for (size_t ms = 0; ms < measuremets; ms++) {
			result[ms] += measures[day][ms] > 0 ? measures[day][ms] : 0;
		}
	}
	for (auto &ms : result) {
		ms /= days;
	}
	return result;
}
/*
// много делений
vector<float> ComputeAvgTemp1(const vector<vector<float>>& measures) {
	size_t days = measures.size(), measuremets = measures[0].size();
	vector<double> result(measures.size());
	for (size_t day = 0; day < days; day++) {
		for (size ms = 0; ms < measuremets; ms++) {
			result[ms] += measures[day][ms] > 0 ? measures[day][ms] / days : 0;
		}
	}
	return result;
}
// плохой проход циклами
// if
// много делений
// незарезервировал память
vector<float> ComputeAvgTemp2(const vector<vector<float>>& measures) {
	size_t days = measures.size(), measuremets = measures[0].size();
	vector<double> result;
	for (size_t ms = 0; ms < measuremets; ms++) {
		int mid = 0;
		for (size_t day = 0; day < days; day++) {
			if (measures[day][ms] > 0){
				mid += 1.0 * measures[day][ms] / days;
			}
		}
		result.push_back(mid);
	}
	return result;
}
*/
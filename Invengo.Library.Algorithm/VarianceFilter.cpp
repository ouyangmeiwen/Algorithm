// src/VarianceFilter.cpp
#include "pch.h"

#include "VarianceFilter.h"
#include <algorithm>
#include <cmath>
#include <numeric>

VarianceFilter::VarianceFilter(const std::vector<double>& inputData, double varianceThreshold)
    : data(inputData), varianceThreshold(varianceThreshold) {}

double VarianceFilter::calculateVariance(const std::vector<double>& data, double mean) const {
    double variance = 0.0;
    for (const auto& value : data) {
        variance += (value - mean) * (value - mean);
    }
    return variance / data.size();
}

std::vector<double> VarianceFilter::filterData() const {
    // �����ֵ
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();

    // ���㷽��
    double variance = calculateVariance(data, mean);

    // �����׼��
    double stddev = std::sqrt(variance);

    // ʹ�÷����ķַ�����������
    std::vector<double> filteredData;
    for (const auto& value : data) {
        if (std::abs(value - mean) <= varianceThreshold * stddev) {
            filteredData.push_back(value);
        }
    }

    return filteredData;
}

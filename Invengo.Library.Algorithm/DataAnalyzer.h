// include/DataAnalyzer.h

#ifndef DATA_ANALYZER_H
#define DATA_ANALYZER_H

#include <vector>

// ����һ�������� DataAnalyzer�����й��˷������̳и���
class DataAnalyzer {
public:
    virtual ~DataAnalyzer() = default;

    // ���鷽�������ع��˺������
    virtual std::vector<double> filterData() const = 0;
};

#endif // DATA_ANALYZER_H

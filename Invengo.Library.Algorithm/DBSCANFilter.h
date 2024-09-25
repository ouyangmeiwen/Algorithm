// include/DBSCANFilter.h

#ifndef DBSCAN_FILTER_H
#define DBSCAN_FILTER_H

#include "DataAnalyzer.h"

class DBSCANFilter : public DataAnalyzer {
public:
    /***
    epsilon = 0.5�����ֵͨ����ͨ��ʵ������ݵľ���������ȷ���ġ�������ݵ�֮��ľ����ձ��С����ô��С�� epsilon ���ܻ�����ʡ�ѡ���ʵ��� epsilon �ǹؼ�����Ϊ�������˵�������С��Ӱ��ص��γɡ�
    minPoints = 3��ͨ����˵��minPoints ֵӦ�������ݵ�ά�ȡ����ڶ�ά���ݣ�������ѡ���� 4 �� 5�������ȷ���ص��ȶ��ԺͿɿ��ԡ���ʵ��Ӧ���У�������Ҫ�������ݵ��ܶ����������ֵ��
    ****/
    DBSCANFilter(const std::vector<double>& inputData, double epsilon = 0.5, int minPoints = 3);

    std::vector<double> filterData() const override;

private:
    std::vector<double> data;
    double epsilon;
    int minPoints;

    std::vector<int> performDBSCAN() const;
};

#endif // DBSCAN_FILTER_H

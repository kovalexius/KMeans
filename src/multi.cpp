#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

#include <point.h>
#include <generate.h>
#include <kmeansMultiThread.h>

const uint32_t NUM_POINTS = 50000; // Количество точек
const uint32_t NUM_METRICS = 5;    // Метрика-размерность точек
const uint32_t NUM_CLUSTERS = 20;  // Количество кластеров

int main()
{
    /*
    typedef CPoint<NUM_METRICS> MYPOINT;
    std::vector<MYPOINT> vpoints;

    generators::generate<NUM_METRICS>(NUM_POINTS, vpoints);

    multiThread::CKmeans<MYPOINT> kmeans(vpoints, NUM_CLUSTERS);

    std::vector<std::pair<MYPOINT, std::vector<MYPOINT>>> result = kmeans.Run();


    for (auto e : result)
        std::cout << e.first << " " << std::endl;

    std::cin.get();
    */
    return 0;
}
#include "astar.h"
#include <math.h>
#include <algorithm>

Astar::Astar(double HW, bool BT)
{
    hweight = HW;
    breakingties = BT;

    std::cout << "    astart init\n";
}

double Astar::computeHFromCellToCell(int i1, int j1, int i2, int j2, const EnvironmentOptions &options)
{
    int i_dist, j_dist;
    double metric_dist = 0;

    if (options.metrictype == CN_SP_MT_EUCL) {
        i_dist = i1 - i2;
        j_dist = j1 - j2;
        metric_dist = std::sqrt(i_dist * i_dist + j_dist * j_dist);
    }
    if (options.metrictype == CN_SP_MT_DIAG) {
        i_dist = abs(i1-i2);
        j_dist = abs(j1-j2);
        return (i_dist+j_dist) + (CN_SQRT_TWO - 2)*std::min(i_dist,j_dist);
    }
    if (options.metrictype == CN_SP_MT_MANH) {
        i_dist = abs(i1-i2);
        j_dist = abs(j1-j2);
        return (i_dist+j_dist);
    }

    return hweight*metric_dist;
}

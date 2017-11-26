#include "theta.h"
Theta::~Theta()
{
}

bool Theta::lineOfSight(int i1, int j1, int i2, int j2, const Map &map, bool cutcorners)
{
    int di, dj, si, sj, f;
    di = i2-i1;
    dj = j2-j1;
    f = 0;

    if (di < 0) {
        di = -di;
        si = -1;
    } else {
        si = 1;
    }
    if (dj < 0) {
        dj = -dj;
        sj = -1;
    } else {
        sj = 1;
    }

    if (di >= dj) {
        while (i1!=i2) {
            f += dj;
            if (f>= di) {
                if (map.CellIsObstacle(i1 + (si - 1)/2, j1 + (sj - 1)/2)) {
                    return false;
                }
                j1+=sj;
                f-=di;
            }
            if (f!= 0 && map.CellIsObstacle(i1 + (si - 1)/2, j1 + (sj - 1)/2)) {
                return false;
            }
            if (dj == 0 && map.CellIsObstacle(i1 + (si - 1)/2, j1) &&
                    map.CellIsObstacle(i1 + (si - 1)/2, j1 -1)) {
                return false;
            }
            i1+=si;
        }
    } else {
        while (j1 != j2) {
            f+=di;
            if (f >= dj) {
                if (map.CellIsObstacle(i1 + (si - 1)/2, j1 + (sj - 1)/2)) {
                    return false;
                }
                i1 += si;
                f -= dj;
            }
            if (f!=0 && (map.CellIsObstacle(i1 + (si - 1)/2, j1 + (sj - 1)/2))) {
                return false;
            }
            if (di == 0 && map.CellIsObstacle(i1, j1 + (sj - 1)/2) &&
                    map.CellIsObstacle(i1 -1, j1 + (sj - 1)/2)) {
                return false;
            }
            j1 += sj;
        }
    }
    return true;
}

Node Theta::resetParent(Node current, Node parent, const Map &map, const EnvironmentOptions &options )
{
    //need to implement
    return current;
}

void Theta::makeSecondaryPath()
{
    //need to implement
}

void Theta::makePrimaryPath(Node curNode)
{
    //need to implement
}

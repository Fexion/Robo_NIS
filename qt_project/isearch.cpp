#include "isearch.h"
#include <string>

ISearch::ISearch()
{
    hweight = 1;
    breakingties = CN_SP_BT_GMAX;
    std::cout << "    isearch init\n";
}

ISearch::~ISearch(void) {}



SearchResult ISearch::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    std::cout << hweight<<"     startsearch in func\n";
    std::cout <<computeHFromCellToCell(0,0,1,1, options) << std::endl;
    int num_of_dir;
    if (options.allowdiagonal) {
        num_of_dir = 8;
    } else {
        num_of_dir = 4;

    }
    std::vector<int> di(num_of_dir);
    std::vector<int> dj(num_of_dir);

    if (options.allowdiagonal) {
        di = {0,1,1,1,0,-1,-1,-1};
        dj = {1,1,0,-1,-1,-1,0,1};
    } else {
        di = {0,1,0,-1};
        dj = {1,0,-1,0};
    }



    int i, ni, cwi, ccwi;
    int j, nj, cwj, ccwj;
    int g, dir;
    char c;
    std::string path = "";
    int start_i, start_j, goal_i, goal_j;
    start_i = map.getStarti(); start_j = map.getStartj();
    goal_i = map.getGoali(), goal_j = map.getGoalj();
    Node *n0, *m0, *clean, *parent;
    n0 = new Node(start_i, start_j, 0, 0);
    open[pqi].push(*n0);
    delete n0;

    std::cout << start_i << " " << start_j << std::endl;
    while(!open[pqi].empty()) {

        n0 = new Node(open[pqi].top().i, open[pqi].top().j,
                      open[pqi].top().g, open[pqi].top().H);
        open[pqi].pop();
        i = n0->i;
        j = n0->j;
        g = n0->g;

        close.push_back(*n0);

        if (i == goal_i && j == goal_j) {
            parent = n0;
            while (!(i==start_i && j==start_j)) {
                std::cout << "qwe" << std::endl;


                parent=parent->parent;
                i= parent->i;
                j= parent->j;
                std::cout << i << "qwe " << j << start_i << start_j<< std::endl;
            }
            //while(!open[pqi].empty()) open[pqi].pop();
            std::cout << i << "qwe " << j << std::endl;
            return sresult;

        }
        for (int d = 0; d < num_of_dir; ++d) {
            ni = i + di[d];
            nj = j + dj[d];

            ccwi = i + di[(d+1)%num_of_dir];
            ccwj = j + dj[(d+1)%num_of_dir];

            cwi = i+di[(d-1+num_of_dir)%num_of_dir];
            cwj = j + dj[(d-1+num_of_dir)%num_of_dir];
            if (map.CellIsTraversable(ni, nj) && map.CellOnGrid(ni,nj)) {
                if (num_of_dir == 8) {
                    if(i%2 == 1) {
                        if ((map.CellIsObstacle(cwi, cwj) ||
                                map.CellIsObstacle(ccwi, ccwj))&&
                                !options.cutcorners) {
                            continue;
                        }
                        if (map.CellIsObstacle(cwi, cwj) &&
                             map.CellIsObstacle(ccwi, ccwj)&&
                             !options.allowsqueeze) {
                         continue;
                        }
                    }
                }
            } else {
                continue;
            }

            m0 = new Node(ni, nj, g+map.getValue(ni,nj),
                          computeHFromCellToCell(ni,nj,goal_i, goal_j,options));
            m0->parent = n0;
            if ((std::find(close.begin(), close.end(), *m0) != close.end() )) {
                continue;
            }

            while (!(open[pqi].top()== *m0) && !open[pqi].empty()) {
                open[1-pqi].push(open[pqi].top());
                open[pqi].pop();
            }

            if (open[pqi].empty()) {
                open[pqi].push(*m0);
                delete m0;
            }else {
                if (m0->F < open[pqi].top().F) {
                    *clean = open[pqi].top();
                    open[pqi].pop();
                    delete clean;
                    open[pqi].push(*m0);
                    delete m0;
                } else {
                    delete m0;
                }
            }

            if(open[pqi].size() > open[1-pqi].size()) pqi=1-pqi;
            while(!open[1 - pqi].empty()) {
                open[pqi].push(open[1-pqi].top());
                open[1-pqi].pop();
            }
        }

    }
    //need to implement

    /*sresult.pathfound = ;
    sresult.nodescreated =  ;
    sresult.numberofsteps = ;
    sresult.time = ;
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;*/
    return sresult;
}

/*std::list<Node> ISearch::findSuccessors(Node curNode, const Map &map, const EnvironmentOptions &options)
{
    std::list<Node> successors;
    //need to implement
    return successors;
}*/

/*void ISearch::makePrimaryPath(Node curNode)
{
    //need to implement
}*/

/*void ISearch::makeSecondaryPath()
{
    //need to implement
}*/

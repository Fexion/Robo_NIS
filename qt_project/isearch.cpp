#include "isearch.h"




ISearch::ISearch()
{
    hweight = 1;
    breakingties = CN_SP_BT_GMAX;
    std::cout << "    isearch init\n";
}

ISearch::~ISearch(void) {}



SearchResult ISearch::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    std::cout <<"     startsearch in func\n";

    clock_t t;
    t = clock();

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
    int rdi, rdj;
    double g;
    int step = 0;

    int start_i, start_j, goal_i, goal_j;
    start_i = map.getStarti(); start_j = map.getStartj();
    goal_i = map.getGoali(), goal_j = map.getGoalj();

    Node *n0, *m0, *parent;
    rdi = 0;
    rdj = 0;
    //std::cout << "  i" << start_i << "   j" << start_j << "\n";
    //std::cout << "   los " << lineOfSight(start_i, start_j, 7, 3, map, options.cutcorners) << "\n";

    n0 = new Node(start_i, start_j, 0,
                  computeHFromCellToCell(start_i,start_j,goal_i, goal_j,options));
    n0->parent = n0;
    open[pqi].push(*n0);


    while(!open[pqi].empty()) {
        ++step;
        n0 = new Node(open[pqi].top().i, open[pqi].top().j,
                      open[pqi].top().g, open[pqi].top().H,
                      open[pqi].top().parent, breakingties);
        open[pqi].pop();


        i = n0->i;
        j = n0->j;
        g = n0->g;

        //close.push_back(*n0);
        close.insert((std::make_pair(i,j)));

        if (i == goal_i && j == goal_j) {
            t = clock()-t;
            sresult.time = ((float)t)/CLOCKS_PER_SEC;
            parent = n0;

            sresult.pathfound = true;
            sresult.nodescreated =  open[pqi].size() + close.size();
            sresult.numberofsteps = step;
            sresult.pathlength = g;
            while (!(i==start_i && j==start_j)) {
//                if (parent->parent == nullptr) {
//                    hppath.push_front(*parent);
//                } else {
                    if (!(rdi == (parent->i - i) &&
                          rdj == (parent->j - j))) {
                        hppath.push_front(*(n0));

                        rdi = parent->i - i;
                        rdj = parent->j - j;
                    }
               // }

                i = parent->i;
                j = parent->j;
                //std::cout << i << "   " << start_i << "\n";
                lppath.push_front(*parent);
                n0=parent;
                parent=parent->parent;


            }
            hppath.push_front(*parent);
            sresult.lppath = &lppath;
            sresult.hppath = &hppath;

            while(!open[pqi].empty()) open[pqi].pop();
            return sresult;

        }
        for (int d = num_of_dir-1; d>=0;--d){//0; d < num_of_dir; ++d) {
            if (!accessibility(i,j,d,di,dj,num_of_dir,map,options)) {
                continue;
            }
            ni = i + di[d];
            nj = j + dj[d];


//            if ((std::find(close.begin(), close.end(), *m0) != close.end() )) {
//                delete m0;
//                continue;
//            }
            if (close.count(std::make_pair(ni,nj))) {
                //delete m0;
                continue;
            }

            double cost;
            if (options.allowdiagonal) {
                if (lineOfSight(n0->parent->i, n0->parent->j, ni, nj,
                                map, options.cutcorners)) {

                    cost = sqrt((n0->parent->i-ni)*(n0->parent->i-ni) +
                                    (n0->parent->j-nj)*(n0->parent->j-nj));
                    parent = n0->parent;

                } else {
                    cost = (d%2)*(CN_SQRT_TWO-1)+1;
                    parent = n0;
                }
            } else {
                cost = 1;
                parent = n0;
            }

            //std::cout<< "    nd created\n";

            m0 = new Node(ni, nj, parent->g+cost,
                          computeHFromCellToCell(ni,nj,goal_i, goal_j,options),
                          parent, breakingties);


            while (!(open[pqi].top()== *m0) && !open[pqi].empty()) {
                open[1-pqi].push(open[pqi].top());
                open[pqi].pop();

            }
            if (open[pqi].empty()) {
                open[pqi].push(*m0);
                delete m0;
            }else {
                if (m0->g <= open[pqi].top().g && open[pqi].top()== *m0) { // changes path a bit
                    open[pqi].pop();
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

bool ISearch::accessibility(int i, int j, int d, const std::vector<int> &di, const std::vector<int> &dj,
                            int num_of_dir, const Map &map, const EnvironmentOptions &options)
{
    int ni = i + di[d];
    int nj = j + dj[d];

    int ccwi = i + di[(d+1)%num_of_dir];
    int ccwj = j + dj[(d+1)%num_of_dir];

    int cwi = i+di[(d-1+num_of_dir)%num_of_dir];
    int cwj = j + dj[(d-1+num_of_dir)%num_of_dir];
    if (map.CellOnGrid(ni,nj)) {
        if (map.CellIsTraversable(ni, nj)) {
            if (num_of_dir == 8) {
                if(d%2 == 1) {
                    if ((!map.CellIsTraversable(cwi, cwj) ||
                         !map.CellIsTraversable(ccwi, ccwj))&&
                            !options.cutcorners) {
                        return false;
                    }
                    if (map.CellIsObstacle(cwi, cwj) &&
                            map.CellIsObstacle(ccwi, ccwj)&&
                            !options.allowsqueeze) {
                        return false;
                    }
                }
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;

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

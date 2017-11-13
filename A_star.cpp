#include <iostream>
#include <queue>
#include <math.h>
#include <vector>
const int num_of_dir = 8;

int dx[num_of_dir] = {0,1,1,1,0,-1,-1,-1};
int dy[num_of_dir] = {1,1,0,-1,-1,-1,0,1};

// const int num_of_dir = 4;
//
// int dx[num_of_dir] = {0,1,0,-1};
// int dy[num_of_dir] = {1,0,-1,0};

class node
{
  int x;
  int y;
  int cost;
  int priority;

public:
  node(int xp, int yp, int costp, int priorityp)
      {x = xp; y = yp; cost = costp; priority = priorityp;}

  int get_xpos() const{return x;}
  int get_ypos() const{return y;}
  int get_cost() const{return cost;}
  int get_priority() const{return priority;}


  int calc_dist(const int x_dest, const int y_dest) {
    int x_dist, y_dist, metric_dist;
    x_dist = x - x_dest;
    y_dist = y - y_dest;

    metric_dist = std::sqrt(x_dist * x_dist + y_dist * y_dist);

    return metric_dist;
  }
  void update_cost(int dir) {
    if (num_of_dir == 8) {
      cost += dir%2 * 14 + (dir+1)%2 * 10;
    }
    if (num_of_dir == 4) {
      cost+= 1;
    }
  }
  void update_priority(const int x_dest, const int y_dest) {
    priority = cost + calc_dist(x_dest, y_dest);
  }
};

bool operator<(const node & a, const node & b){
  return a.get_priority()>b.get_priority();
}

bool corners(int x, int y, int m, int n) {
  return x>=0 && y>=0 && x < m && y < n;
}

std::string find_path(int start_x, int start_y, int fin_x, int fin_y, std::vector<std::vector<int>> & map) {
  std::string path = "";
  int n = map.size();
  int m = map[0].size();
  int x, nx, ccwx, cwx;
  int y, ny, ccwy, cwy;
  char c;
  int dir;
  node * n0, * m0;
  bool f;

  std::vector<std::vector<int>> used(n, std::vector<int> (m,0));
  std::vector<std::vector<int>> priority_map(n, std::vector<int> (m,0));
  std::vector<std::vector<int>> dir_map(n, std::vector<int> (m,0));

  std::priority_queue<node> pq[2];
  int pqi = 0;

  n0 = new node(start_x, start_y, 0, 0);
  n0->update_priority(fin_x, fin_y);
  pq[pqi].push(*n0);
  priority_map[start_y][start_x] = n0->get_priority();

  while(!pq[pqi].empty()) {
    n0 = new node(pq[pqi].top().get_xpos(), pq[pqi].top().get_ypos(),
                  pq[pqi].top().get_cost(), pq[pqi].top().get_priority());
    pq[pqi].pop();
    x = n0->get_xpos();
    y = n0->get_ypos();

    used[y][x] = 1;

    if (x == fin_x && y == fin_y) {
      while (!(x == start_x && y == start_y)) {
        dir = dir_map[y][x];
        x+= dx[dir];
        y+= dy[dir];
        c = '0' + (dir + num_of_dir/2)%num_of_dir;
        path = c + path;
      }
      delete n0;
      while(!pq[pqi].empty()) pq[pqi].pop();
      return path;
    }
    for (int i = 0; i < num_of_dir; ++i) {
      f = 1;
      nx = x+dx[i];
      ny = y + dy[i];
      ccwx = x+dx[(i+1)%num_of_dir];
      ccwy = y + dy[(i+1)%num_of_dir];
      cwx = x+dx[(i-1+num_of_dir)%num_of_dir];
      cwy = y + dy[(i-1+num_of_dir)%num_of_dir];
      if (i%2==1 && corners(cwx,cwy,m,n) && corners(ccwx,ccwy,m,n)) {
        f = !map[cwy][cwx] && !map[ccwy][ccwx];
      }
      if (corners(nx,ny,m,n) && !used[ny][nx] && !map[ny][nx] && f) {

        m0 = new node(nx, ny, n0->get_cost(), 0);
        m0->update_cost(i);
        m0->update_priority(fin_x, fin_y);
        if (priority_map[ny][nx] == 0) {
          dir_map[ny][nx] = (i + num_of_dir/2)%num_of_dir;

          priority_map[ny][nx] = m0->get_priority();
          pq[pqi].push(*m0);
        } else if(m0->get_priority() < priority_map[ny][nx]) {
          dir_map[ny][nx] = (i + num_of_dir/2)%num_of_dir;

          priority_map[ny][nx] = m0->get_priority();
          while (pq[pqi].top().get_xpos()!=nx && pq[pqi].top().get_ypos()!=ny) {
            pq[1-pqi].push(pq[pqi].top());
            pq[pqi].pop();
          }
          pq[pqi].pop();
          pq[pqi].push(*m0);

          if(pq[pqi].size() > pq[1-pqi].size()) pqi=1-pqi;
          while(!pq[1 - pqi].empty())
          {
              pq[pqi].push(pq[1-pqi].top());
              pq[1-pqi].pop();
          }
        } else delete m0;
      }

    }
    delete n0;
  }
  return path;
}





int main() {
  std::vector<std::vector<int>> map =
  { {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}};
  int n = map.size();

  int m = map[0].size();

  int startx = m-1, starty = n-1;
  int finx = 0, finy = 0;

  std::cout << find_path(startx, starty, finx, finy, map) << std::endl;


  return 0;
}

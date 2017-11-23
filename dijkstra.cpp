#include <iostream>
#include <queue>
#include <math.h>
#include <vector>
const int num_of_dir = 8;

int dx[num_of_dir] = {0,1,1,1,0,-1,-1,-1};
int dy[num_of_dir] = {1,1,0,-1,-1,-1,0,1};



//Обратно можно идти по значению веса в вершине = вес ребра + значение смежной вершины
//для closed(used) nodes использовать хэши


// const int num_of_dir = 4;
//
// int dx[num_of_dir] = {0,1,0,-1};
// int dy[num_of_dir] = {1,0,-1,0};

class node
{
  int x;
  int y;
  int priority;

public:
  node(int xp, int yp, int priorityp)
      {x = xp; y = yp; priority = priorityp;}

  int get_xpos() const{return x;}
  int get_ypos() const{return y;}
  int get_priority() const{return priority;}

  void update_priority(const int field_cost) {
    priority += field_cost;
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

  n0 = new node(start_x, start_y, 0);
  n0->update_priority(map[start_y][start_y]);
  pq[pqi].push(*n0);
  priority_map[start_y][start_x] = n0->get_priority();

  while(!pq[pqi].empty()) {
    n0 = new node(pq[pqi].top().get_xpos(), pq[pqi].top().get_ypos(),
                  pq[pqi].top().get_priority());
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
        f = map[cwy][cwx]!=-1 && map[ccwy][ccwx]!=-1;
      }
      if (corners(nx,ny,m,n) && !used[ny][nx] && map[ny][nx]!=-1 && f) {

        m0 = new node(nx, ny, n0->get_priority());
        m0->update_priority(map[ny][nx]);
        if (priority_map[ny][nx] == 0) {
          dir_map[ny][nx] = (i + num_of_dir/2)%num_of_dir;
          //std::cout << nx <<' '<< ny<< ' '<< m0->get_priority() << '\n';
          priority_map[ny][nx] = m0->get_priority();
          pq[pqi].push(*m0);
        } else if(m0->get_priority() < priority_map[ny][nx]) {
          dir_map[ny][nx] = (i + num_of_dir/2)%num_of_dir;
          //std::cout << nx <<' '<< ny<< ' '<< m0->get_priority() << '\n';
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
  { {0, 0, 100, -1, -1},
    {0, 30, 1000, 300, 0},
    {0, 1, 1000, 300, 0},
    {0, 0, 1500, 300, 0}};
  int n = map.size();

  int m = map[0].size();

  int startx = m-1, starty = n-1;
  int finx = 0, finy = 0;

  std::cout << find_path(startx, starty, finx, finy, map) << std::endl;


  return 0;
}

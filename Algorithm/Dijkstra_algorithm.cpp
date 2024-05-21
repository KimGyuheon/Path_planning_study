#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <ctime>

using namespace std;

struct Point {
    int x, y;

    Point(int x, int y) : x(x), y(y) {}
};

// 다익스트라 알고리즘
vector<Point*> dijkstra(vector<vector<int>> grid, Point* start, Point* end) {
    // 각 좌표까지의 최단 거리 저장 벡터
    vector<vector<int>> distance(grid.size(), vector<int>(grid[0].size(), INT_MAX));
    // 방문 여부 벡터
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    
    vector<Point*> path;

    // 동서남북 비교를 위한 벡터
    vector<pair<int,int>> direction = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    // 시점에 가중치 0
    distance[start->x][start->y] = 0;

    // 우선순위 큐 : 벡터구조, greater : 오름차순
    priority_queue<pair<int, Point*>, vector<pair<int, Point*>>, greater<pair<int, Point*>>> pq;

    // 시점에 대한 정보 추가
    pq.push({distance[start->x][start->y], start});

    while (!pq.empty()) {
        Point* current = pq.top().second;
        pq.pop();

        // 방문했던 노드는 건너뜀
        if (visited[current->x][current->y]) continue;
        // 방문안했다면 방문했음을 표시
        visited[current->x][current->y] = true;

        for (auto dir : direction) {
            int nx = current->x + dir.first, ny = current->y + dir.second;

            if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] == 0){
                // 가중치 추가
                int new_distance = distance[current->x][current->y] + 1;

                // 새로운 거리가 기존의 거리보다 짧은 경우 업데이트하고 우선순위 큐에 추가
                if (new_distance < distance[nx][ny]) {
                    distance[nx][ny] = new_distance;
                    // 이동한 좌표에 대한 정보 우선순위에 큐에 입력
                    pq.push({new_distance, new Point(nx, ny)});
                }
            }
        }
    }

    // 최단 경로를 역추적하여 path 벡터에 추가
    Point* current = end;
    path.push_back(current);

    // 역순이므로 시점에 도달할때 까지 반복
    while (!(current->x == start->x && current->y == start->y)) {
        for (auto dir : direction) {
            int nx = current->x + dir.first, ny = current->y + dir.second;

            // 이동 가능한 경로이고, 현재 좌표의 거리가 다음 좌표의 거리보다 1 작은 경우 (역순임을 유의)
            if(nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] == 0 && distance[nx][ny] == distance[current->x][current->y] - 1) {
                // current 업데이트
                current = new Point(nx, ny);
                path.push_back(current);
            }
        }
    }

    // 역순으로 저장된 경로를 뒤집어서 반환
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    clock_t start_time, finish_time;
    double duration;
    // 시간 측정 시작
    start_time = clock();

    // 입력 maze
    // 2차원 벡터, 크기: 20 x 20 
    // maze 설정: 0은 이동 가능, 1은 장애물
    vector<vector<int>> maze = {{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                                {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0},
                                {0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
                                {0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                                {1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1},
                                {0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0},
                                {0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0},
                                {1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0},
                                {1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
                                {0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
                                {0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0},
                                {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                                {0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}};

    // 결과 경로 출력용 벡터 (O: 미방문 노드, X: 방문한 노드, . : 경로)
    vector<vector<char>> res_map(maze.size(), vector<char>(maze[0].size(), 'O'));

    // 시작점과 도착점 설정
    Point* start = new Point(0, 0);
    Point* end = new Point(maze.size()-1, maze[0].size()-1);

    // 다익스트라 알고리즘으로 최단 경로 찾기
    vector<Point*> shortest_path = dijkstra(maze, start, end);

    if(!shortest_path.empty()){
        cout << "Dijkstra Path Found!" << endl;
        cout << "Path Cost : " << shortest_path.size() << endl;

        for(auto p : shortest_path){
            res_map[p->x][p->y] = '.';
        }

        for(auto row : res_map){
            for(char c : row){
                cout << c << " ";
            }
            cout << endl;
        }
    }
    else{
        cout << "Path Not Found" << endl;
    }

    // 동적으로 할당된 메모리 해제
    delete start;
    delete end;
    for (const auto& point : shortest_path) {
        delete point;
    }

    // 시간 측정 종료
    finish_time = clock();
    duration = (finish_time - start_time);
    cout << "Time: " << duration << "ms" << endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <ctime>

using namespace std;

struct Point {
    int x, y;

    Point(int _x, int _y) : x(_x), y(_y) {}
};

// 다익스트라 알고리즘
vector<Point*> dijkstra(vector<vector<int>> maze, Point* start, Point* end, vector<vector<bool>>& visited) {
    // 각 좌표까지의 최단 거리를 저장하는 배열
    vector<vector<int>> distance(maze.size(), vector<int>(maze[0].size(), INT_MAX));

    // 최단 경로를 저장하는 벡터
    vector<Point*> path;

    vector<pair<int,int>> direction = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    // 시작 좌표의 최단 거리를 0으로 설정
    distance[start->x][start->y] = 0;

    // 우선순위 큐를 사용하여 최단 거리를 업데이트하며 탐색
    priority_queue<pair<int, Point*>, vector<pair<int, Point*>>, greater<pair<int, Point*>>> pq;

    // 시점에 대한 정보 추가
    pq.push({distance[start->x][start->y], start});

    while (!pq.empty()) {
        // 초기 노드 혹은 이후 현재 노드에 대한 생성자
        Point* current = pq.top().second;
        pq.pop();

        // 방문했던 노드는 건너뜀
        if (visited[current->x][current->y]) continue;

        // 방문안했다면 방문했음을 표시
        visited[current->x][current->y] = true;

        // 현재 위치에서 상하좌우로 이동
        for (auto dir : direction) {
            int nx = current->x + dir.first, ny = current->y + dir.second;

            // 유효한 좌표인지 확인하고, 이동 가능한 경로인지 확인
            if (nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && maze[nx][ny] == 0){
                // 이동했으므로 nx, ny에 대한 new_distance 즉, 가중치 현재값+1
                int new_distance = distance[current->x][current->y] + 1;
                
                // 새로운 거리가 기존의 거리보다 짧은 경우 업데이트하고 우선순위 큐에 추가
                // 각 노드간 가중치가 1이기 때문에 사실상 거의 항상 참
                if (new_distance < distance[nx][ny]) {
                    distance[nx][ny] = new_distance;
                    // 이동한 좌표에대한 정보 우선순위에 큐에 입력
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
            if(nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && maze[nx][ny] == 0 && distance[nx][ny] == distance[current->x][current->y] - 1) {
                // current 업데이트
                current = new Point(nx, ny);
                // path에 current 추가
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
    start_time = clock();

    // TEST CASE 1
    // 지도 설정: 20X20, 0은 이동 가능, 1은 장애물
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

    // TEST CASE 2
    // 지도 설정: 20X29, 0은 이동 가능, 1은 장애물
    // vector<vector<int>> maze = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));

    // 결과 경로 출력용 벡터 (O: 미방문 노드, X: 방문한 노드, . : 경로)
    vector<vector<char>> res_map(maze.size(), vector<char>(maze[0].size(), 'O'));

    // 시점과 종점에 대한 생성자

    // TEST CASE 1 coordinate
    Point* start = new Point(0, 0);
    Point* end = new Point(maze.size()-1, maze[0].size()-1);
    
    // TEST CASE 2 coordinate
    // Point* start = new Point(maze.size()-1, 10);
    // Point* end = new Point(4, 10);

    // 다익스트라 알고리즘으로 최단 경로 찾기
    vector<Point*> shortest_path = dijkstra(maze, start, end, visited);

    if(!shortest_path.empty()){
        cout << "TEST: Dijkstra Path Found!" << endl;
        cout << "Path Cost : " << shortest_path.size() << endl;
        cout << "Number of Visited Node : " << visited.size() * visited[0].size() << endl;

        for(int i=0; i<visited.size(); i++){
            for(int j=0; j<visited[0].size(); j++){
                res_map[i][j] = 'X';
            }
        }

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

    finish_time = clock();
    duration = (finish_time - start_time);
    cout << "Time: " << duration << "ms" << endl;

    // 동적으로 할당된 메모리 해제
    delete start;
    delete end;
    for (const auto& point : shortest_path) {
        delete point;
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <ctime>

using namespace std;

struct Node {
    int x, y;           // 좌표
    int g, h, f;        // 휴리스틱 요소
    Node* parent;

    Node(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
};

struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

int heuristic(int x1, int y1, int x2, int y2) {
    // 유클리디안 거리
    // return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));

    // 맨해튼 거리
    return (abs(x1 - x2) + abs(y1 - y2));
}

// A* 알고리즘
bool aStarAlgorithm(vector<vector<int>>& maze, Node* start, Node* goal, vector<Node*>& path) {

    priority_queue<Node*, vector<Node*>, CompareNode> openSet;
    vector<vector<bool>> closedSet(maze.size(), vector<bool>(maze[0].size(), false));

    //초기 값
    start->h = heuristic(start->x, start->y, goal->x, goal->y);
    start->f = start->g + start->h;
    openSet.push(start);

    while (!openSet.empty()) {
        // 현재 비교할 노드에 대한 정보를 current로 불러옴.
        // 정방향으로 진행되는 경로를 current에 저장.
        Node* current = openSet.top();
        openSet.pop();

        // 목표 도달 시
        if (current->x == goal->x && current->y == goal->y) {
            // current가 가장 처음 주소인 nullptr이 될때 까지 반복.
            while (current != nullptr) {
                path.push_back(current);
                // pushback이후 current의 이전값인 parent를 가리킴.
                current = current->parent;
            }
            // path에 역순(시점->종점)으로 저장했기 때문에 reverse.
            reverse(path.begin(), path.end());
            return true;
        }

        // goal 도달 안했을 경우
        // closedset의 요소 중 현재 노드에 대해 true로 변환. (default는 false로 초기화 했음.)
        closedSet[current->x][current->y] = true;
        
        // 주변 노드 순회
        vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // 동, 남, 서, 북
        for (auto& dir : directions) {
            // nx, ny에 이동할 새로운 좌표의 값을 저장.
            int nx = current->x + dir.first, ny = current->y + dir.second;

            // maze 범위 및 장애물 검사
            // maze범위 내에 존재하는 유효한 좌표인지 탐색, !closedset에서 false일 경우에 참.
            if (nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && maze[nx][ny] == 0 && !closedSet[nx][ny]) {
                int new_g = current->g + 1;
                int new_h = heuristic(nx, ny, goal->x, goal->y);
                int new_f = new_g + new_h;

                Node* neighbor = new Node(nx, ny);
            
                neighbor->g = new_g;
                neighbor->h = new_h;
                neighbor->f = new_f;
                neighbor->parent = current;

                openSet.push(neighbor);
            }
        }
    }

    return false;
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

    Node* start = new Node(0, 0);
    Node* goal = new Node(maze.size()-1, maze[0].size()-1);

    vector<Node*> path;

    if (aStarAlgorithm(maze, start, goal, path)) {
        cout << "Astar Path found!" << endl;
        cout << "Path Cost:" << path.size() << endl;
        
        for (Node* n : path) {
            res_map[n->x][n->y] = '.';
        }

        for(auto row : res_map){
            for(char n : row){
                cout << n << " ";
            }
            cout << endl;
        }

    } else {
        cout << "No path found." << endl;
    }

    // 시간 측정 종료
    finish_time = clock();
    duration = (finish_time - start_time);
    cout << "Time: " << duration << "ms" << endl;

    return 0;
}
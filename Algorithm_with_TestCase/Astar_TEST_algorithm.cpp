#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <ctime>

using namespace std;

struct Node {
    int x, y;           // 노드의 위치
    int g, h, f;        // 비용 g: 시점->현재,  h: 현재->종점,  f: g+h
    Node* parent;       // 부모 노드

    // 노드의 위치(시점, 종점)의 생성은 필요하기 때문에 (int x, int y)로 작성. 나머지는 모두 0 또는 nullptr로 생성.
    // parent는 현재 노드의 이전 노드를 의미하고 시점의 이전 노드는 nullptr임을 의미함.
    Node(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
};

// f값 비교를 위한 구조체
// 우선순위 큐 (priority queue)에 대한 것. min heap 구현
struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

// 휴리스틱 함수
int heuristic(int x1, int y1, int x2, int y2) {
    // 유클리디안 거리
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));

    // 맨해튼 거리
    // return (abs(x1 - x2) + abs(y1 - y2));
}


// A* 알고리즘 구현
bool aStarAlgorithm(priority_queue<Node*, vector<Node*>, CompareNode> openSet, vector<vector<int>>& maze, Node* start, Node* goal, vector<Node*>& path, vector<pair<int, int>>& v_map, int& visit_cnt) {

    // closedset을 grid와 같은크기로 생성하고 모든 요소를 false로 초기화.
    vector<vector<bool>> closedSet(maze.size(), vector<bool>(maze[0].size(), false));

    // heuristic 계산: start, goal 각 구조체의 x, y를 pointer로 뽑아 휴리스틱 계산.
    start->h = heuristic(start->x, start->y, goal->x, goal->y);

    // f 계산: g값과 heuristic 값을 합친 f 계산.
    start->f = start->g + start->h;

    openSet.push(start);

    while (!openSet.empty()) {
        // 정방향으로 진행되는 경로를 current에 저장. 목표 도달시 path로 경로 내용 옮김.
        // current에 openset의 top 즉 비용이 가장 작은 값을 입력.
        Node* current = openSet.top();
        openSet.pop();

        // 목표 도달 시
        if (current->x == goal->x && current->y == goal->y) {
            // v_map에 최적의 경로를 찾기위해 방문한 노드들에 대한 기록. 
            for(int i=0; i < closedSet.size(); i++){
                for(int j=0; j < closedSet[0].size(); j++){
                    if(closedSet[i][j] == true){
                        v_map.push_back({i, j});
                    }
                }
            }
            // current가 가장 처음 주소인 nullptr이 될때 까지 반복.
            while (current != nullptr) {
                path.push_back(current);
                // pushback이후 current는 current의 이전값인 parent의 주소를 저장.
                current = current->parent;
            }
            // 현재 path에는 goal에서 start까지 거꾸로 pushback 되어 있으므로 reverse 해줌.
            reverse(path.begin(), path.end());
            visit_cnt = v_map.size();
            // 목표 도달했으므로 true 반환.
            return true;
        }

        // goal 도달 안했을 경우
        // closedset의 요소중 현재 노드에 대해 true로 변환. (default는 false로 초기화 했었음. 위에 있음.) 
        closedSet[current->x][current->y] = true;
        
        // 주변 노드 순회
        vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // 동, 남, 서, 북
        for (auto& dir : directions) {
            // nx, ny에 이동할 새로운 좌표의 값을 저장.
            int nx = current->x + dir.first, ny = current->y + dir.second;

            // maze 범위 및 장애물 검사
            // maze범위 내에 존재하는 유효한 좌표인지 탐색, !closedset에서 false일 경우에 참.
            if (nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && maze[nx][ny] == 0 && !closedSet[nx][ny]) {
                // 한칸 이동했으므로 g+1
                int new_g = current->g + 1;
                // 이동한 좌표에 대한 휴리스틱함수 계산.
                int new_h = heuristic(nx, ny, goal->x, goal->y);
                // f 계산.
                int new_f = new_g + new_h;

                // 이웃한 노드(이동한 주변 노드)에 대한 생성자
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

    int visit_cnt = 0;

    // 결과 경로 출력용 벡터 (O: 미방문 노드, X: 방문한 노드, . : 경로)
    vector<vector<char>> res_map(maze.size(), vector<char>(maze[0].size(), 'O'));
    vector<pair<int, int>> v_map;

    // 시점과 종점에 대한 생성자

    // TEST CASE 1 coordinate
    Node* start = new Node(0, 0);
    Node* goal = new Node(maze.size()-1, maze[0].size()-1);
    
    // TEST CASE 2 coordinate
    // Node* start = new Node(4, 10);
    // Node* goal = new Node(maze.size()-1, 10);

    // 이동내용을 담을 path 라는 vector를 생성. Node의 포인터를 저장함. Node의 객체들의 주소를 보관하는 역할.
    vector<Node*> path;

    //openset이라는 우선순위 큐 생성. CompareNode로 인해 min heap의 형태 -> 작은값이 앞에 옴.
    priority_queue<Node*, vector<Node*>, CompareNode> openSet;

    if (aStarAlgorithm(openSet, maze, start, goal, path, v_map, visit_cnt)) {
        cout << "TEST: Astar Path found!" << endl;
        cout << "Path Cost:" << path.size() << endl;
        cout << "Visited Node:" << visit_cnt << endl;
        
        for(auto coor : v_map){
            res_map[coor.first][coor.second] = 'X';
        }

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

    // 동적할당 메모리 해제
    delete start;
    delete goal;
    for (Node* node : path) {
    delete node;
    }
    while (!openSet.empty()) {
        Node* node = openSet.top();
        openSet.pop();
        delete node;
    }

    return 0;
}
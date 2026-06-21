#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <windows.h>

using namespace std;

// 方向数组：上、下、左、右
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

// ==================== 迷宫类 ====================
class Maze {
public:
    int rows, cols;
    vector<vector<int>> grid;          // 0=通路, 1=墙壁
    pair<int,int> start, end;

    Maze(int r, int c) : rows(r), cols(c) {
        grid = vector<vector<int>>(rows, vector<int>(cols, 0));
        start = {0, 0};
        end = {rows-1, cols-1};
    }

    // 打印迷宫（可选择高亮路径）
    void print(const vector<pair<int,int>>& path = {}) {
        vector<vector<bool>> inPath(rows, vector<bool>(cols, false));
        for (auto& p : path) {
            inPath[p.first][p.second] = true;
        }

        // 上边框
        cout << "+";
        for (int j = 0; j < cols; j++) cout << "--";
        cout << "+" << endl;

        for (int i = 0; i < rows; i++) {
            cout << "|";
            for (int j = 0; j < cols; j++) {
                if (make_pair(i,j) == start)
                    cout << "S ";
                else if (make_pair(i,j) == end)
                    cout << "E ";
                else if (inPath[i][j])
                    cout << "**";   // 路径
                else if (grid[i][j] == 1)
                    cout << "##";   // 墙壁
                else
                    cout << "  ";   // 空地
            }
            cout << "|" << endl;
        }

        // 下边框
        cout << "+";
        for (int j = 0; j < cols; j++) cout << "--";
        cout << "+" << endl;
    }

    // 检查坐标是否合法且可通行
    bool isValid(int x, int y, vector<vector<bool>>& visited) {
        return x >= 0 && x < rows && y >= 0 && y < cols
            && grid[x][y] == 0 && !visited[x][y];
    }
};

// ==================== BFS 寻路（最短路径）====================
vector<pair<int,int>> bfs(Maze& maze) {
    int r = maze.rows, c = maze.cols;
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    vector<vector<pair<int,int>>> parent(r, vector<pair<int,int>>(c, {-1,-1}));

    queue<pair<int,int>> q;
    auto [sx, sy] = maze.start;
    auto [ex, ey] = maze.end;

    q.push({sx, sy});
    visited[sx][sy] = true;

    bool found = false;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();

        if (x == ex && y == ey) {
            found = true;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (maze.isValid(nx, ny, visited)) {
                visited[nx][ny] = true;
                parent[nx][ny] = {x, y};
                q.push({nx, ny});
            }
        }
    }

    // 回溯路径
    vector<pair<int,int>> path;
    if (!found) return path;

    int cx = ex, cy = ey;
    while (!(cx == sx && cy == sy)) {
        path.push_back({cx, cy});
        auto [px, py] = parent[cx][cy];
        cx = px; cy = py;
    }
    path.push_back({sx, sy});
    reverse(path.begin(), path.end());
    return path;
}

// ==================== DFS 寻路 ====================
bool dfsRecursive(Maze& maze, int x, int y,
                  vector<vector<bool>>& visited,
                  vector<pair<int,int>>& path) {
    auto [ex, ey] = maze.end;
    if (x == ex && y == ey) {
        path.push_back({x, y});
        return true;
    }

    visited[x][y] = true;
    path.push_back({x, y});

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (maze.isValid(nx, ny, visited)) {
            if (dfsRecursive(maze, nx, ny, visited, path))
                return true;
        }
    }

    path.pop_back();
    return false;
}

vector<pair<int,int>> dfs(Maze& maze) {
    vector<vector<bool>> visited(maze.rows, vector<bool>(maze.cols, false));
    vector<pair<int,int>> path;
    dfsRecursive(maze, maze.start.first, maze.start.second, visited, path);
    return path;
}

// ==================== 随机生成迷宫（Prim 算法）====================
Maze generateMaze(int rows, int cols) {
    Maze maze(rows, cols);

    // 先全部设为墙壁
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            maze.grid[i][j] = 1;

    srand((unsigned)time(nullptr));

    // 候选墙列表：存储 (墙x, 墙y, 来源通路x, 来源通路y)
    vector<tuple<int,int,int,int>> walls;

    // 从起点开始
    maze.grid[0][0] = 0;
    for (int d = 0; d < 4; d++) {
        int nx = dx[d], ny = dy[d];
        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols)
            walls.push_back({nx, ny, 0, 0});
    }

    while (!walls.empty()) {
        int idx = rand() % walls.size();
        auto [wx, wy, px, py] = walls[idx];
        walls.erase(walls.begin() + idx);

        if (maze.grid[wx][wy] == 1) {
            int count = 0;
            for (int d = 0; d < 4; d++) {
                int nx = wx + dx[d], ny = wy + dy[d];
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze.grid[nx][ny] == 0)
                    count++;
            }

            if (count <= 1) {
                maze.grid[wx][wy] = 0;
                for (int d = 0; d < 4; d++) {
                    int nx = wx + dx[d], ny = wy + dy[d];
                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze.grid[nx][ny] == 1)
                        walls.push_back({nx, ny, wx, wy});
                }
            }
        }
    }

    // 确保终点和起点是通路
    maze.grid[rows-1][cols-1] = 0;
    maze.grid[0][0] = 0;

    return maze;
}

// ==================== 主程序 ====================
int main() {
    // 设置控制台为 UTF-8 编码
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "====================================" << endl;
    cout << "       Maze Pathfinding v1.0       " << endl;
    cout << "====================================" << endl;
    cout << endl;

    // ========== 示例1：手工迷宫 ==========
    cout << "[1] Handcrafted Maze (8x10)" << endl;
    cout << "----------------------------" << endl;

    Maze maze1(8, 10);
    int preset[8][10] = {
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
        {1, 1, 0, 1, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 1, 0, 1},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    };
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 10; j++)
            maze1.grid[i][j] = preset[i][j];

    cout << endl << "[Map]" << endl;
    maze1.print();

    cout << endl << "[BFS - Shortest Path]" << endl;
    auto path1 = bfs(maze1);
    if (!path1.empty()) {
        maze1.print(path1);
        cout << "  => Path length: " << path1.size() << " steps" << endl;
    } else {
        cout << "  x No path found!" << endl;
    }

    cout << endl << "[DFS - Depth First]" << endl;
    auto path1_dfs = dfs(maze1);
    if (!path1_dfs.empty()) {
        maze1.print(path1_dfs);
        cout << "  => Path length: " << path1_dfs.size() << " steps" << endl;
    } else {
        cout << "  x No path found!" << endl;
    }

    // ========== 示例2：随机生成迷宫 ==========
    cout << endl << endl;
    cout << "[2] Random Maze (15x20)" << endl;
    cout << "------------------------" << endl;

    Maze maze2 = generateMaze(15, 20);
    cout << endl << "[Map]" << endl;
    maze2.print();

    cout << endl << "[BFS - Shortest Path]" << endl;
    auto path2 = bfs(maze2);
    if (!path2.empty()) {
        maze2.print(path2);
        cout << "  => Path length: " << path2.size() << " steps" << endl;
    } else {
        cout << "  x No path found!" << endl;
    }

    // ========== 示例3：BFS vs DFS 对比 ==========
    cout << endl << endl;
    cout << "[3] BFS vs DFS Comparison (5x5)" << endl;
    cout << "---------------------------------" << endl;

    Maze maze3(5, 5);
    int preset3[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0},
    };
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            maze3.grid[i][j] = preset3[i][j];

    auto bfs_path = bfs(maze3);
    auto dfs_path = dfs(maze3);

    cout << endl << "BFS (guaranteed shortest):" << endl;
    if (!bfs_path.empty()) {
        maze3.print(bfs_path);
        cout << "  Path length: " << bfs_path.size() << endl;
    } else {
        cout << "  x No solution" << endl;
    }

    cout << endl << "DFS (may not be shortest):" << endl;
    if (!dfs_path.empty()) {
        maze3.print(dfs_path);
        cout << "  Path length: " << dfs_path.size() << endl;
    } else {
        cout << "  x No solution" << endl;
    }

    cout << endl << endl;
    cout << "Press Enter to exit..." << endl;
    cin.get();
    cin.get();
    return 0;
}

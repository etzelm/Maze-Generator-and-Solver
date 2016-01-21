// File:    project.cpp
// Name:    Mitchell Etzel
// DVC ID:  1416569
// IDE:     CodeBlock
// Description: Final Project
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <utility>
#include<queue>
#include<algorithm>
#include<climits>
#include <stdlib.h>
#include <time.h>
#define ii pair<int, int>
#define MAX 201
#define CELL 10000
enum GRAPH_TYPE {DI, BI};
enum BORDER {N=1, E, W, S};
using namespace std;

bool TRACE = false;

struct compareII {bool operator()(const ii &j, const ii &k) {return j.second > k.second;}};


class Edge {
    int source, dest, weight;

public:
    Edge(int s, int d, int w) : source(s), dest(d), weight(w) {}
    void operator = (const Edge &right) {
        source = right.source;
        dest = right.dest;
        weight = right.weight;
    }
	void showEdge() {
        cout << "source:" << source
			 << " dest:" << dest
			 << " weight:" << weight << "\n";
	}
};

class Graph {
	int V, E;
	list<ii> *edge;
	vector<ii> cord;
	vector<int> distance, pv;
	priority_queue<ii, vector<ii>, compareII > Q;

public:
	Graph(int v_num) : V(v_num), E(0) {
		edge = new list<ii>[V];
		distance = vector<int> (V, INT_MAX);
	}
	int output() {return distance[0];}
	void addEdge(int u, int v, int w, int type = BI) {
		edge[u].push_back(ii(v, w)); E++;
		if (type != DI) edge[v].push_back(ii(u, w)); E++;
	}
	void addNode(int x, int y) {
		cord.push_back(make_pair(x, y));
	}
	int findNode(int x, int y) {
		int count = 0;
		for(auto n:cord) {
			if(n.first == x && n.second == y) return count;
			else count++;
		}
		return -1;
	}
	void printCord(){
		int i = 0;
		for(auto c:cord) cout << "\n " << i++ << "-node: (" << c.first << "," << c.second << ")";
	}
	void setSize(int &num_vertices) {V = num_vertices;}
	void getSize(int &num_vertices) {num_vertices = V;}
	void dijkstra(int x) {
        distance[x] = 0;
        Q.push(ii(x, 0));
        while (!Q.empty()) {
            ii top = Q.top();
            Q.pop();
            int y = top.first, z = top.second;
            bool picked = false;
            for (auto w:pv) if (w == y) {picked = true; break;}
            if (!picked) {pv.push_back(y); cout << y << "," << z << " \n";}
            if (z <= distance[y]) {
                for (auto r:edge[y]) {
                    int y2 = r.first;
                    int z2 = r.second;
                    if (distance[y2] > distance[y] + z2) {
                        distance[y2] = distance[y] + z2;
                        Q.push(ii(y2, distance[y2]));
                        pv.erase(remove(pv.begin(), pv.end(), y2), pv.end());
                    }
                }
            }
        }
        cout << "Picked Node, Distance ";
        for (int i = 0; i < V; i++) cout << pv[i] << " ";
        cout << "\nDistance Array: ";
        for (int i = 0; i < V; i++) cout << distance[i] << " ";
        cout << endl;
	}
	void printGraph() {
        cout << "\nGraph of (" << V << ", " << E << ") ";
        for(int n = 0; n < V; n++) {
            cout << "\nVertex-" << n << " : ";
            for(auto e:edge[n]) cout << n << "->(" << e.first << ", " << e.second << ") ";
        }
    }
};

class Maze {
    int num_rows, num_cols, grid_size, start_row=0, start_col=0, end_row, end_col, num_nodes=0, DFS_index;
	char maze[MAX][MAX];
	vector<ii> DFS_Stack;

  public:
    Maze() : num_rows(20), num_cols(75), start_row(0), start_col(0), end_row(19), end_col(74) {}
    Maze(int s) : num_rows(s*2+1), num_cols(s*2+1), start_row(0), start_col(0), end_row(s*2), end_col(s*2), grid_size(s) {
        for(int x = 0; x < num_rows; x++) {
            for(int y = 0; y < num_cols; y++) {
                if(x % 2 == 0 || y % 2 == 0) maze[x][y] = 'X';
                else maze[x][y] = ' ';
            }
        }
        DFS_index = 0;
        DFS_Stack.push_back(make_pair(1,1));
    }
	~Maze() {}
	void ReadMaze(string name) {
        ifstream fin;
        fin.open(name.c_str());
        string line;
        for (int r = 0; r < num_rows; r++) {
            getline(fin, line);
            for (int c = 0; c < num_cols; c++) {
                if (c < int(line.length())) {
                    maze[r][c] = line[c];
                }
            }
        }
        fin.close();
    }
	void WriteMaze(string name) {
        ofstream fout;
        fout.open(name.c_str());
        for (int r = 0; r < num_rows; r++) {
            for (int c = 0; c < num_cols; c++) {
                fout << maze[r][c];
            }
            fout << endl;
        }
        fout.close();
    }
	void PrintMaze() {
        for (int r = 0; r < num_rows; r++) {
            for (int c = 0; c < num_cols; c++) {
                cout << maze[r][c];
            }
            cout << endl;
        }
    }
    void getGraphSize(int &n) {n = num_nodes;}
    void GetStart(int &r, int &c) {r = start_row; c = start_col;}
	void SetStart(int r, int c) {start_row = r; start_col = c;}
	void GetEnd(int &r, int &c) {r = end_row; c = end_col;}
	void SetEnd(int r, int c) {end_row = r; end_col = c;}
	void MarkStartEnd() {
		maze[end_row][end_col] = '@'; num_nodes++;
		maze[0][0] = '@'; num_nodes++;
	}
	void GetNumNodes(int &n) {n = num_nodes;}
	void SetNumNodes(int n) {num_nodes = n;}
	bool MazeToGraph(Graph &G) {
        int v;
        G.getSize(v);
        bool *visited = new bool[v];
        for(int i = 0; i < v; i++) visited[i] = false;
        MazeToGraph(G, 0, 0);
        cout << "\nNumber of nodes: " << num_nodes << "\n";
        return true;
    }
	bool MazeToGraph(Graph &G, int r, int c) {
        static int edge_counter = 0, step_count = 0, current_node = 0;
        if ((r < 0) || (r > num_rows) || (c < 0) || (c > num_cols)) return false;
        if (maze[r][c] != ' ') return false;
        if (r==0 && c==0) {
            maze[r][c] = '@';
            G.addNode(r,c);
        }
        else {
            maze[r][c] = '-';
            step_count++;
        }
        int path_count = 0;
        if (r > 0 && maze[r-1][c] == ' ') path_count++;
        if (c > 0 && maze[r][c-1] == ' ') path_count++;
        if (r < num_rows - 1 && maze[r+1][c] == ' ') path_count++;
        if (c < num_cols - 1 && maze[r][c+1] == ' ') path_count++;
        if (path_count == 0) {
            ii joint(ii(-1,-1));
            if(maze[r-1][c] == '+') joint = ii(r-1, c);
            if(maze[r+1][c] == '+') joint = ii(r+1, c);
            if(maze[r][c-1] == '+') joint = ii(r, c-1);
            if(maze[r][c+1] == '+') joint = ii(r, c+1);
            if(joint.first != -1 || joint.second != -1) {
                maze[r][c] = '-';
                int thisNode = G.findNode(joint.first,joint.second);
                step_count++;
                G.addEdge(current_node, thisNode, step_count);
                edge_counter++;
                cout << "+ addEdge(" << thisNode << ", " << current_node << ", " << step_count << ")\n";
                step_count = 0;
                return false;
            }
            else {
                maze[r][c] = '@';
                G.addNode(r,c);
                int thisNode = G.findNode(r,c);
                step_count++;
                G.addEdge(thisNode, current_node, step_count);
                cout << "@ addEdge(" << current_node << ", " << thisNode << ", " << step_count++ << ")\n";
                current_node = G.findNode(r,c);
                step_count = 0;
                num_nodes++;
            }
        }
        if(path_count > 1) {
            maze[r][c] = '+';
            G.addNode(r,c);
            num_nodes++;
            int thisNode = G.findNode(r,c);
            G.addEdge(thisNode, current_node, step_count);
            edge_counter++;
            cout << "+ addEdge(" << current_node << ", " << thisNode << ", " << step_count << ")\n";
            current_node = G.findNode(r,c);
            step_count = 0;
        }
        if (MazeToGraph(G, r + 1, c) || MazeToGraph(G, r - 1, c) ||
            MazeToGraph(G, r, c + 1) || MazeToGraph(G, r, c - 1)) {
            return true;
        }
        return false;
    }
    bool closedTile(int x, int y) {
        if( maze[x - 1][y] == 'X' &&
            maze[x][y - 1] == 'X' &&
            maze[x][y + 1] == 'X' &&
            maze[x + 1][y] == 'X' ) return true;
        return false;
    }
    void genMaze() {
        ii pos(make_pair(1,1));
        genMaze(0, pos, 1);
        maze[start_row][start_col] = ' ';
        maze[0][1] = ' ';
        maze[end_row][end_col] = ' ';
        maze[end_row][end_col - 1] = ' ';
    }
    void genMaze(int DFS_index, ii pos, int visited_tiles) {
        int valid_count = -1, border[4];
        ii neighbor[4], next_tile;

        if(visited_tiles < grid_size*grid_size) {
            int x = pos.first, y = pos.second;

            if(x-2 > 0 && closedTile(x-2, y)) {
                valid_count++;
                neighbor[valid_count].first=x-2;
                neighbor[valid_count].second=y;
            }
            if(y+2 < MAX && closedTile(x, y+2)) {
                valid_count++;
                neighbor[valid_count].first=x;
                neighbor[valid_count].second=y+2;
            }
            if(y-2 > 0 && closedTile(x, y-2)) {
                valid_count++;
                neighbor[valid_count].first=x;
                neighbor[valid_count].second=y-2;
            }
            if(x+2 < MAX && closedTile(x+2, y)) {
                valid_count++;
                neighbor[valid_count].first=x+2;
                neighbor[valid_count].second=y;
            }

            if(valid_count == -1) {
                next_tile = DFS_Stack.back();
                DFS_Stack.pop_back();
                if (!DFS_Stack.empty()) genMaze(DFS_index, next_tile, visited_tiles);
            }

            if(valid_count!=-1) {
                visited_tiles++;
                next_tile=neighbor[rand()%(valid_count+1)];
                DFS_Stack.push_back(next_tile);
                int xa = x - next_tile.first;
                int ya = y - next_tile.second;
                switch (xa) {
                case 0:
                    xa = x;
                    break;
                case -2:
                    xa = x + 1;
                    break;
                case 2:
                    xa = x - 1;
                    break;
                }
                switch (ya) {
                case 0:
                    ya = y;
                    break;
                case -2:
                    ya = y + 1;
                    break;
                case 2:
                    ya = y - 1;
                    break;
                }
                maze[xa][ya] = ' ';
                genMaze(DFS_index, next_tile, visited_tiles);
            }
        }
    }
};

int main() {
    srand(time(NULL));
	Maze M(10);
	Graph G(64);
	int num_nodes;

	cout << "Starting Grid Maze:\n";
	M.PrintMaze();

	M.genMaze();
	cout << "\n\nGenerated Maze:\n";
    M.PrintMaze();
	M.WriteMaze("maze.txt");

	cout << "\n\nConverting Generated Maze to a Graph:\n";
	M.MazeToGraph(G);
	M.PrintMaze();
	M.getGraphSize(num_nodes);
	G.setSize(num_nodes);

	cout << "\n\nGenerated Graph:";
	G.printGraph();

	cout << "\n\nRunning Dijkstra Algorithm on Generated Graph:\n";
	G.dijkstra(num_nodes);

	int hold = G.output();
	cout << "\n\nLightest Weight from Start to Finish: " << hold << "\n\n";
	return 0;
}


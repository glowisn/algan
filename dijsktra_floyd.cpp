/*
    2021-06-05
    21700663 정예찬 알고리듬분석 hw 6
    References: my hw5 
                https://modoocode.com/305

*/
#define max_node 30
#define max_char_size 100

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <string.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <iomanip>

using std::cout;
using std::endl;
using std::vector;

typedef struct node {
    int vertex;
    std::string weight;
}node;

void printMatrix(std::string array[][max_node], int width, int height, std::string cityn[]);
void dijkstra(int start, int height, int width, std::string mindis[][max_node], vector<node> vertex[]);

bool faster(std::string a, std::string b) { // return true if a is faster path(less number) than b
    if (a.compare("INF") == 0)
        return false;
    else if (b.compare("INF") == 0)
        return true;
    else {
        if (stoi(a) > stoi(b))
            return false;
        else
            return true;
    }
}

std::string plus(std::string a, std::string b) {
    if (a.compare("INF") == 0 || b.compare("INF") == 0)
        return "INF";
    else
        return std::to_string(stoi(a) + stoi(b));
}

int main() {


    std::ifstream fin("hw6_data.txt");
    if (!fin)
    {
        cout << "file open failed!" << endl;
        return 0;
    }

    cout << "file open success!" << endl;

    std::string mat[max_node][max_node];
    char t[max_char_size];
    std::string city[max_node];
    std::string line;


    cout << "Making matrix with file..." << endl;
    int ln = -2;
    int width = 0;
    while (!fin.eof())           //make matrix with file
    {
        int li = -1;
        ln++;
        getline(fin, line);
        if (ln == -1)
            continue;
        if (line.compare("") == 0)
            continue;
        strcpy_s(t, line.c_str());

        char* context = NULL;
        char* dis = strtok_s(t, "\t ", &context);
        city[ln] = dis;
        //cout << "city[" << ln << "] = " << city[ln] << endl;
        li++;

        while (dis != NULL)
        {
            dis = strtok_s(NULL, "\t ", &context);
            if (dis == NULL)
                continue;
            //printf("dis = %s , %d \n", dis, li);
            if (!strcmp(dis, "INF")) {
                mat[ln][li] = "INF";
            }
            else {
                mat[ln][li] = dis;
            }
            li++;
        }
        width = li;
    }

    int height = ln;
    printMatrix(mat, width, height, city);

    //make array of adjacnecy list
    vector<node> vertex[max_node];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (mat[i][j].compare("0") == 0)
                continue;
            if (mat[i][j].compare("INF") != 0) {
                node tmp;
                tmp.vertex = j;
                tmp.weight = mat[i][j];
                vertex[i].push_back(tmp);
            }
        }
    }

    //dijkstra
    clock_t startsecond = clock();
    std::string mindis[max_node][max_node];         // min distance of 1st max_node(city) to 2nd max_node(city)
    //mindis initialize
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mindis[i][j] = "INF";
        }
    }

    vector<node> copyver[max_node];                 //copy vector array because I made it pop
    for (int i = 0; i < height; i++) {
        std::copy(vertex, vertex + height, copyver);
        dijkstra(i, height, width, mindis, copyver);
    }

    clock_t endsecond = clock();
    cout << "It tooks " <<((double)endsecond - (double)startsecond) / 1000 << " seconds to compute shortest path between cities with Dijkstra's algorithm as follows:" << endl;
    printMatrix(mindis, width, height, city);

    //floyd
    startsecond = clock();
    //mindis initialize
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mindis[i][j] = mat[i][j];
        }
    }

    for (int k = 0; k < height; k++) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                std::string tmp = plus(mindis[i][k], mindis[k][j]);
                if (faster(tmp, mindis[i][j])) {
                    mindis[i][j] = tmp;
                }
            }
        }
    }



    endsecond = clock();
    cout << "It tooks " << ((double)endsecond - (double)startsecond) / 1000 << " seconds to compute shortest path between cities with Floyd's algorithm as follows:" << endl;
    printMatrix(mindis, width, height, city);


    return 0;
}

void printMatrix(std::string array[][max_node], int width, int height, std::string cityn[]) {
    cout.setf(std::ios::right);
    cout << std::setw(10) << " ";
    for (int i = 0; i < width; i++) {
        cout << std::setw(10) << cityn[i];
    }
    cout << endl;
    for (int i = 0; i < height; i++)
    {
        cout << std::setw(10) << cityn[i];
        for (int j = 0; j < width; j++)
        {
            cout << std::setw(10) << array[i][j];
        }
        cout << endl;
    }
    cout << endl << endl;
}

void dijkstra(int start, int height, int width, std::string mindis[][max_node], vector<node> vertex[]) {
    bool visited[max_node] = {false,};
    mindis[start][start] = "0";
    std::queue<int> q;
    q.push(start);
    while (!q.empty()) {
        int pi = q.front();
        q.pop();
        visited[pi] = true;
        while (!vertex[pi].empty()) {
            node tmp = vertex[pi].back();
            vertex[pi].pop_back();
            if (visited[tmp.vertex])
                continue;
            std::string plen = plus(tmp.weight, mindis[start][pi]);
            if (faster(plen, mindis[start][tmp.vertex])) {
                mindis[start][tmp.vertex] = plen;
            }
            q.push(tmp.vertex);
        }
    }
}


/*if (faster(mindis[start][tmp->vertex],tmp->weight))   // mindis[start][tmp->vertex]이 weight보다 작으면
continue; */
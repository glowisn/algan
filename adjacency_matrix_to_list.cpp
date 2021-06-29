/*
    2021-05-25
    21700663 정예찬 알고리듬분석 hw 5
    References: http://www.cplusplus.com/forum/beginner/236646/  //2D vector

*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>

#define max_node 20
#define max_char_size 100

using std::vector;
using std::cout;
using std::endl;

void printMatrix(int array[][max_node], int width, int height);
void printVector(vector<int> vec[max_node], int vertex);

int main() {
    std::ifstream fin("hw5_data.txt");
    if (!fin)
    {
        cout << "file open failed!" << endl;
        return 0;
    }

    cout << "file open success!" << endl;

    int mat1[max_node][max_node];
    char t[max_char_size];
    std::string line;


    cout << "Making matrix with file..." << endl;
    int i = 0, j = 0;
    int ln = 0;
    while (!fin.eof())           //make matrix with numbers in file
    {
        j = 0;
        ln++;
        getline(fin, line);
        //cout << line << endl;
        if (ln == 1)
            continue;
        strcpy_s(t, line.c_str());
        int li = 1;
        while (t[li] != '\0')
        {
            if (!isspace(t[li]))
            {
                mat1[i][j] = t[li] - '0';
                j++;
            }
            li++;
        }
        i++;
    }
    int width = j;
    int height = i;
    //printMatrix(mat1, width, height);

    //cout << endl << "\ttranpose" << endl;

    int mat2[max_node][max_node];       //tranpose mat1 make mat2
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {
            mat2[j][i] = mat1[i][j];
        }
    }
    int widthts = height;
    int heightts = width;

    //printMatrix(mat2, widthts, heightts);

    vector<int> vertex[max_node];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if(mat1[i][j] == 1)
                vertex[i].push_back(j);
        }
    }
    cout << endl << "adjacency list" << endl;
    printVector(vertex, height);

    vector<int> vertex2[max_node];
    for (int i = 0; i < heightts; i++) {
        for (int j = 0; j < widthts; j++) {
            if (mat2[i][j] == 1)
                vertex2[i].push_back(j);
        }
    }

    cout << endl << "tranposed adjacency list" << endl;
    printVector(vertex2, heightts);

    return 0;
}

void printMatrix(int array[][max_node],int width,int height) {
    for (int i = 0; i < height; i++)
    {
        cout << "\t";
        for (int j = 0; j < width; j++)
        {
            cout << array[i][j] << " ";
        }
        cout << endl;
    }
}

void printVector(vector<int> vec[max_node], int vertex) {
    char c = ' ';
    for (int i = 0; i < vertex; i++)
    {
        c = 65;
        cout << (char)(c + i) << " | ";
        for (int j = 0; j < vec[i].size() ; j++)
        {
            cout << " -> " << (char)(vec[i][j] + 65);
        }
        cout << endl;
    }
}
/*
    2021-05-06
    21700663 정예찬 알고리듬분석
    Reference:  LMS 강의 alg_knapsack_3
                https://en.cppreference.com/w/cpp/numeric/random        //random usage
                http://www.cplusplus.com/reference/cstdlib/qsort/       //qsort usage
                https://crazyj.tistory.com/152                          //벡터 구조체 필드 기준으로 최댓값 찾기

    clock_t start = clock();
    clock_t end = clock();
    cout << "elapsed time : " << (double)(end-start) << "ms" << endl;

    Not sure for B&B problem.
*/

#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

typedef struct Items
{
    int value;
    int weight;
    float benefit;
}Items;

struct Node {
    int value;
    int weight;
    float bound;
    int itemind;
};

Items *makeItems(int size);
float greedyAlgorithm(Items *tem, int size, int w);
int compare(const void *a, const void *b);
void freeItems(Items *tem);
int dynamicProgrammingAlgorithm(Items *tem, int size, int w);
void showItems(Items* tem, int num);
float boundexpect(Items *tem, int size, int index, int w);
bool promising(Node *node, int max_benefit, int bigW);
void branchNode(Items *tem, int size, std::vector<Node *> &vec, int bigW, int max_benefit);
void eraseUnpromising(std::vector<Node*>& vec, int bigW, int max_benefit);
int branchBoundAlgorithm(Items *tem, int size, int bigW);
void sortVecbound(std::vector<Node *> &vec);
int maxBBbenefit(std::vector<Node *> &vec);
std::random_device rd;
std::default_random_engine el(rd());
std::uniform_int_distribution<int> mkr100int(1, 100);
std::uniform_int_distribution<int> mkr400int(1, 400);

clock_t start_t = 0, end_t = 0;

int main() {
    int noI[8] = { 100, 1000, 2000, 4000, 6000, 8000, 10000, 12000 };
    bool keep = 1;
    Items* item;
    float fVB = 0.F;
    int iVB = 0;
    int i = 0;
    int bigW = 0;
    std::ofstream fout("output.txt");
    fout << "Number of Items | Processing time / Maximum benefit value" << std::endl;
    fout << "  No | Greedy | Dynamic | B. & B. " << std::endl;

    while (keep) {
        fout << noI[i] << " | ";
        item = makeItems(noI[i]);
        bigW = 40 * noI[i];

        fVB = greedyAlgorithm(item, noI[i], bigW);
        std::cout << fVB << std::endl;
        std::cout << "elapsed time : " << round((double)(end_t - start_t) * 100) / 100 << "ms" << std::endl;
        fout << " " << (double)(end_t - start_t) << "ms";
        fout << " / " << fVB << " | ";

        //for (int i = 0; i < noI[0]; i++)
        //    std::cout << "Item["<<i<<"] value: " << item[i].value << "  weight: "<< item[i].weight << std::endl;


        iVB = dynamicProgrammingAlgorithm(item,noI[i], bigW);
        std::cout << iVB << std::endl;
        std::cout << "elapsed time : " << round((double)(end_t - start_t) * 100) / 100 << "ms" << std::endl;
        fout << " " << round((double)(end_t - start_t) * 1000) / 1000 << "ms";
        fout << " / " << iVB << " | ";


        //for (int i = 0; i < noI[0]; i++)
        //    std::cout << "Item[" << i << "] value: " << item[i].value << "  weight: " << item[i].weight << std::endl;


        iVB = branchBoundAlgorithm(item, noI[i], bigW);
        std::cout << iVB << std::endl;
        std::cout << "elapsed time : " << round((double)(end_t - start_t) * 100) / 100 << "ms" << std::endl;
        fout << " " << round((double)(end_t - start_t) * 1000) / 1000 << "ms";
        fout << " / " << iVB << " | ";

        std::cout << std::endl;
        fout << std::endl;
        freeItems(item);
        i++;
        if(i == 8){
            keep = 0;
        }
    }

    fout.close();
    std::cout << "Bye!" << std::endl;
    return 0;
}

void showItems(Items* tem, int num = 10) {
    for (int i = 0; i < num; i++)
    {
        std::cout << "Item[" << i << "] value: " << tem[i].value << "  weight: " << tem[i].weight << "  benefit: " << tem[i].benefit << std::endl;
    }
}

Items* makeItems(int size = 100){       //allocate and make random Items array

    Items *tem = (Items *)malloc(sizeof(Items) * size);
    std::cout << "Making array of " << size << " items..." << std::endl;

    for (int i = 0; i < size; i++)
    {
        tem[i].value = mkr100int(el);
        tem[i].weight = mkr400int(el);
        tem[i].benefit = (float)tem[i].value / (float)tem[i].weight;
        //cout << "Item["<<i<<"] value: " << tem[i].value << "  weight: "<< tem[i].weight << std::endl;
    }

    //showItems(tem);

    return tem;
}


int compare(const void* a, const void* b){      //to use qsort, desending order
    Items* p = (Items*)a;
    Items* q = (Items*)b;
    return (p->benefit > q->benefit) ? -1 : (p->benefit < q->benefit);
}

void freeItems(Items* tem){
    free(tem);
}

float greedyAlgorithm(Items* tem, int size, int w)
{
    start_t = clock();
    float maxB = 0.F;                                           
    int maxw = w;
    qsort(tem, size, sizeof(Items), compare);

    //showItems(tem);

    //std::cout << "benes 1 : " << tem[1].benefit << " 2 : " << tem[2].benefit << " 3 : " << tem[3].benefit << std::endl;
    for (int i = 0; i < size; i++)
    {
        if (maxw == 0)
            break;
        if (maxw > tem[i].weight)
        {
            maxB += tem[i].value;
            maxw -= tem[i].weight;
        }
        else if (maxw < tem[i].weight)
        {
            maxB += tem[i].benefit * maxw;
            maxw = 0;
        }
    }

    end_t = clock();
    return maxB;
}

int dynamicProgrammingAlgorithm(Items *tem, int size, int w){
    start_t = clock();
    int maxB = 0;
    int **B = (int **)malloc(sizeof(int*) * (size + 1));                     //2 dimension array B[][]
    for(int i = 0; i <= size; i++){
        B[i] = (int *)malloc(sizeof(int) * (w + 1));
    }

    //std::cout << "malloc in DP" << std::endl;

    int maxw = 0;
    int i = 0;
    for(;maxw <= w; maxw++){
        B[0][maxw] = 0;
    }
    for(;i <= size; i++){
        B[i][0] = 0;
    }

    //std::cout << "DP algo start" << std::endl;

    int dbug = -1, dbug2 = -1, dbug3 = -1;;

    int indi = 0;
    for(i = 1; i <= size; i++){       //solution
        indi = i - 1;                  
        for(maxw = 1; maxw <= w; maxw++){
            dbug2 = tem[indi].weight;
            dbug2 = tem[0].weight;
            dbug3 = tem[indi].value;
            if(tem[indi].weight <= maxw){
                if(tem[indi].value + B[i - 1][maxw - tem[indi].weight] > B[i-1][maxw])
                    B[i][maxw] = B[i - 1][maxw - tem[indi].weight] + tem[indi].value;
                else
                    B[i][maxw] = B[i - 1][maxw];
            }else{
                B[i][maxw] = B[i - 1][maxw];
            }

            dbug = B[i][maxw];

        }
    }

    //std::cout << "DP algo end" << std::endl;

    maxB = B[size][w];


    for (int i = 0; i <= size; i++) {
        free(B[i]);
    }
    free(B);

    end_t = clock();
    return maxB;
}

int maxBBbenefit(std::vector<Node *> &vec){
    int max = 0;
    for(int i = 0 ; i < vec.size(); i++){
        if(vec[i]->value > max){
            max = vec[i]->value;
        }
    }
    return max;
}

void sortVecbound(std::vector<Node *> &vec){
    struct Node* max = (Node *)malloc(sizeof(Node));
    max->bound = 0;
    int maxind = -1;
    for(int i = 0 ; i < vec.size(); i++){
        if(vec[i]->bound >= max->bound){
            max->bound = vec[i]->bound;
            maxind = i;
        }
    }

    Node* tmp = vec[maxind];
    vec.erase(vec.begin() + maxind);
    vec.push_back(tmp);

    free(max);
}


float boundexpect(Items *tem, int size, int index, int w)
{
    float bound = 0;

    for (int i = index; i < size; i++)
    {
        if (w <= 0)
            break;
        if (w >= tem[i].weight)
        {
            bound += tem[i].value;
            w -= tem[i].weight;
        }
        else if (w < tem[i].weight)
        {
            bound += (float)w * tem[i].benefit; 
            w = 0;
        }
    }

    return bound;
}

bool promising(Node *node, int max_benefit, int bigW){
    return (node->bound > (float)max_benefit) && (node->weight <= bigW);
}

void eraseUnpromising(std::vector<Node*>& vec, int bigW, int max_benefit) {
    for (int i = vec.size() -1 ; i >= 0; i--) {
        if (!promising(vec[i], max_benefit, bigW)) {
            vec.erase(vec.begin() + i);
        }
    }
}

void branchNode(Items *tem, int size, std::vector<Node *> &vec, int bigW, int max_benefit)     //return NULL for usual, return Node* for solution
{
    struct Node *lchi = (Node*)malloc(sizeof(Node));
    struct Node *rchi = (Node*)malloc(sizeof(Node));

    struct Node *fa = vec.back();
    int childlevel = fa->itemind + 1;
    lchi->value = fa->value + tem[childlevel].value;
    rchi->value = fa->value;

    lchi->weight = fa->weight + tem[childlevel].weight;
    rchi->weight = fa->weight;

    lchi->bound = lchi->value + boundexpect(tem, size, childlevel + 1, bigW - lchi->weight);
    rchi->bound = rchi->value + boundexpect(tem, size, childlevel + 1, bigW - rchi->weight);

    lchi->itemind = fa->itemind + 1;
    rchi->itemind = fa->itemind + 1;

    //check non-promising
    //if double-nonpromising, return fa Node

    //only push promising Node
    vec.pop_back();

    eraseUnpromising(vec, bigW, max_benefit);

    bool lb = promising(lchi, max_benefit, bigW);
    if (lb)
        vec.push_back(lchi);
    else
        free(lchi);

    max_benefit = maxBBbenefit(vec);

    bool rb = promising(rchi, max_benefit, bigW);
    if (rb)
        vec.push_back(rchi);
    else
        free(rchi);


    if (!lb && !rb){
        vec.push_back(fa);
    }

    //vec.erase(vec.begin());
}

int branchBoundAlgorithm(Items *tem, int size, int bigW){
    start_t = clock();
    int max_benefit = 0;
    int dap = 0;
    qsort(tem, size, sizeof(Items), compare);

    std::vector<Node *> vec;
    struct Node *init = (Node*)malloc(sizeof(Node));
    init->itemind = -1;
    init->value = 0;
    init->weight = 0;
    init->bound = boundexpect(tem, size, 0, bigW);

    vec.push_back(init);
    Node *tmp;
    //tmp = *std::max_element(vec.begin(),vec.end(),compareBBbenefit);

    //sort and branch until Non-NULL comes out
    bool keep = 1;
    while(keep){
        branchNode(tem, size, vec, bigW, max_benefit);
        end_t = clock();
        if (((double)(end_t - start_t) / 60000) > 15) {
            break;
        }
        tmp = vec.back();
        sortVecbound(vec);
        if ((tmp->value == vec.back()->value)) {
            dap = tmp->value;
            break;
        }
    }
    
    free(init);
    end_t = clock();
    return dap;
}




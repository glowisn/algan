/*
 Name: heap_min_priority_queue.cpp
    write a program that implements min-priority queue using heap structure.


 Algorithm analysis
 HW 1
 21700663 Yechan Jung
 2021/03/21 creation
*/

// index start from 1
// name val at most 10 char, score float 0.0-100.0

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

#define MAX 1000

struct Info
{
    float score;
    string name;
};

void swap(Info* st, int i, int j);

int main(void)
{

    //input variables
    char c;
    Info student[MAX];
    int size = 0;
    string namei;
    float scorei;
    int curr, ind;

    do{
        cout << endl;
        cout << "*********** MENU ***********" << endl;
        cout << "I : Insert new element into queue." << endl;
        cout << "D : Delete element with smallest key from queue." << endl;
        cout << "C : Decrease key of element in queue." << endl;
        cout << "P : Print out all elements in queue." << endl;
        cout << "Q : Quit " << endl;
        cout << endl;

        cout << "Choose menu: ";
        
        c = getchar();
        while (getchar() != '\n'){} // initialize buffer
        
        switch (c) {
            case 'I': //Insert
                //cout << "Insert Op" << endl;
                cout << "Enter name of element: ";
                getline(cin, namei);
                cout << endl;

                cout << "Enter key value of element: ";
                scanf("%f",&scorei);
                getchar();
                
                if(scorei > 100.0 || scorei < 0.0){
                    cout << "Insertion Failed!" << endl;
                    cout << "score must be in 0.0 to 100.0" << endl;
                    break;
                }

                size++;
                student[size] = {scorei, namei};
                curr = size;
                //when parent node is bigger than inserted node
                while(student[curr/2].score > student[curr].score && curr != 1)
                {
                    swap(student, curr, curr/2);
                    curr = curr/2;
                }

                cout << "New element"
                     << " [" << student[curr].name << ", " << student[curr].score << "] "
                     << "is inserted" << endl;

                break;

            case 'D': //Delete
                //cout << "Delete Op" << endl;

                if(size == 0){
                    cout << "Deletion Failed!" << endl;
                    cout << "The heap is empty" << endl;
                    break;
                }

                if(size == 1){
                    cout << student[size].name << student[size].score << "is deleted" << endl;
                    size--;
                    break;
                }

                swap(student,1,size);
                size--;
                curr = 1;
                while(curr <= size/2 + 1){
                    if(student[curr*2].score < student[curr].score) // swap with left child
                    {
                        swap(student,curr*2,curr);
                        curr = curr *2;
                    }
                    else if ((size != curr * 2 ) && // do not execute when no right child
                    student[curr * 2 + 1].score < student[curr].score) //swap with right child
                    {
                        swap(student, curr * 2 + 1, curr);
                        curr = curr * 2 + 1;
                    }else{
                        break;
                    }
                }

                cout << "[" << student[size+1].name << ", " << student[size+1].score << "] "
                     << "is deleted" << endl;

                break;

            case 'C': //Decrease
                //cout << "Decrease OP" << endl;
                cout << "Enter index of element: ";
                scanf("%d", &ind);
                getchar();

                cout << "Enter new key value: ";
                scanf("%f", &scorei);
                getchar();

                if (scorei > 100.0 || scorei < 0.0)
                {
                    cout << "Decresion Failed!" << endl;
                    cout << "score must be in 0.0 to 100.0" << endl;
                    break;
                }

                if (student[ind].score < scorei) // New key must be smaller than previous
                {
                    cout << "Decresion Failed!" << endl;
                    cout << "New key must be smaller than previous" << endl;
                    break;
                }

                student[ind].score = scorei;
                curr = ind;
                //same as insertion
                while (student[curr / 2].score > student[curr].score && curr != 1)
                {
                    swap(student, curr, curr / 2);
                    curr = curr / 2;
                }

                break;

            case 'P': //Print Elements
                //cout << "Print Op" << endl;
                if(size == 0){
                    cout << "The heap is empty!" << endl;
                }

                for(int i = 1; i <= size; i++){
                    cout << "[" << student[i].name << ", " << student[i].score << "]" << endl;
                }
                break;
        }

    } while (c != 'Q'); // Quit when Q

    cout << "Thank you. Bye!" << endl;
}

//swap Info array element by 2 index
void swap(Info* st, int i, int j){

    Info temp = st[i];
    st[i]= st[j];
    st[j]= temp;

    return;
}
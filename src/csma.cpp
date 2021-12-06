#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int N, L, M, T;
int transT;
int transSlot = 0;
vector<int> RList;
vector<int> busy;

class Node {
    public:
        int ID;
        int R;
        int colCnt;
        int backOff;
        void setBackOff(int ticks);
        void collision(int ticks);
        Node(int id, int r);
};

Node::Node(int id, int r) {
    this->ID = id;
    this->R = r;
    this->colCnt = 0;
    setBackOff(0);
}

void Node::setBackOff(int ticks) {
    backOff = (ID + ticks) % RList[colCnt];
    if (backOff == 0) {
        busy.push_back(ID);
    }
}

void Node::collision(int ticks) {
    backOff = 0;
    if (++colCnt == M) {
        colCnt = 0;
    }
    setBackOff(ticks);
}

vector<Node> NodeList;

vector<int> splitStrToInt(string str, string token) {
    int pos = 0;
    vector<int> res;
    while ((pos = str.find(token)) != string::npos) {
        res.push_back(stoi(str.substr(0, pos)));
        str.erase(0, pos + token.length());
        //str = str.substr(pos + token.length()); //if perserve ori str
    }
    res.push_back(stoi(str));
    return res;
}

void initialize(char* fileName) {
    ifstream inputFile;
    inputFile.open(fileName, ios::in);
    string line;
    getline(inputFile, line);
    N = stoi(line.substr(2, line.size()));
    getline(inputFile, line);
    L = stoi(line.substr(2, line.size()));
    getline(inputFile, line);
    M = stoi(line.substr(2, line.size()));
    getline(inputFile, line);
    RList = splitStrToInt(line.substr(2, line.size()), " ");
    getline(inputFile, line);
    T = stoi(line.substr(2, line.size()));
    inputFile.close();
    for (int i = 0; i < N; i++) {
        NodeList.push_back(Node(i, RList[0]));
    }
}

void printInfo() {
    for (int i = 0; i < NodeList.size(); i++) {
        cout<<NodeList[i].backOff<<endl;
    }
}


void transmittion(int tick) {
    if (transT == 0) {
        NodeList[busy[0]].setBackOff(tick);
        transT = L;
        busy.erase(busy.begin());
    } else {
        NodeList[busy[0]].backOff = 0;
        transT--;
        transSlot++;
    }
}

int main(int argc, char** argv) {
    //printf("Number of arguments: %d", argc);
    if (argc != 2) {
        printf("Usage: ./csma input.txt\n");
        return -1;
    }

    initialize(argv[1]);
    transT = L;
    printInfo();
    for (int tick = 0; tick < T; tick++) {
        if (tick == 9171) {

        }
        // int s = busy.size();
        if (busy.size() == 1) {
            //transmittion
            transmittion(tick);
            
        } else if (busy.size() > 1) {
            //collision
            int sz = busy.size();
            for (int i = 0; i < sz; i++) {
                NodeList[busy[i]].collision(tick);
            }
            busy.erase(busy.begin(), busy.begin() + sz);
        } else { 
            for (int i = 0; i < NodeList.size(); i++) {
                if (NodeList[i].backOff == 1) {
                    busy.push_back(NodeList[i].ID);
                }
                NodeList[i].backOff--;
            }
            if ((busy.size()) == 1) {
                transmittion(tick);
            }
        }
        // cout<<"tick: "<<tick<<endl;
        // printInfo();
    }
    cout<<float(transSlot) / T<<endl;
    ofstream outfile;
    outfile.open("output.txt", ios::out | ios::trunc);
    outfile<<fixed<<setprecision(2);
    outfile<<float(transSlot) / T;
    return 0;
}


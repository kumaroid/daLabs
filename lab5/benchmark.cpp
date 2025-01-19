#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <chrono>

const char SENTINEL_D = '$';
const char SENTINEL_A = '@';

const int MAX_INT = 1e9;

int IS_PATT1 = 1;
int IS_PATT2 = 2;
int IS_PATT1N2 = 3;

class TSuffNode {
public:
    TSuffNode(int l, int len) {
        Left = l;
        SubstrLen = len;
    }

    int RightWall(int strSize) {
        return std::min(strSize, Left + SubstrLen);
    }

public:
    std::map<char, int> Transitions;

    int Left;
    int SubstrLen;

    int SuffLink = 0;
    int NodeType = 0;
}; // TSuffNode

class TSuffTree {
public:
    TSuffTree(const std::string & str);
    int TypeNodeAndFindRes(int currNode, int currDepth, const std::string & str);

private:
    int CreateNode(int l, int len);
    void CreateTRee();

    bool CanWeGo(int id, char c);
    int Go(int id, char c);
    void GoToEdge();

    void Extend(char c);

// solve field
public:
    std::string InputString;
    int MaxCommLen = 0;
    std::vector<int> LastIdxsCommSubstr;

private:
    std::vector<TSuffNode> Nodes;

    int ProcessedSize;
    int NodesCount;

    int ActiveNode = 0;
    int RemainingSuffs = 0;

    int Separ = InputString.find(SENTINEL_A);
};

int TSuffTree::CreateNode(int l, int len)
{
    Nodes.push_back(TSuffNode(l, len));
    ++NodesCount;

    return NodesCount;
}

bool TSuffTree::CanWeGo(int id, char c)
{
    return Nodes[id].Transitions.count(c);
}

int TSuffTree::Go(int id, char c)
{
    return Nodes[id].Transitions[c];
}

void TSuffTree::GoToEdge() 
{
    while (1) {
        char nextSymbInEdge = InputString[ProcessedSize - RemainingSuffs];
        if (!CanWeGo(ActiveNode, nextSymbInEdge)) {
            return;
        }
        int nextNode = Go(ActiveNode, nextSymbInEdge);
        if (Nodes[nextNode].SubstrLen < RemainingSuffs) {
            RemainingSuffs -= Nodes[nextNode].SubstrLen;
            ActiveNode = nextNode;
        } else {
            return;
        }
    }
}

void TSuffTree::Extend(char c) {
    ++ProcessedSize;
    ++RemainingSuffs;
    int last = 0;
    while (RemainingSuffs) {
        GoToEdge();
        char a = InputString[ProcessedSize - RemainingSuffs];

        if (!CanWeGo(ActiveNode, a)) {
            Nodes[ActiveNode].Transitions[a] = CreateNode(ProcessedSize - RemainingSuffs, MAX_INT);
            Nodes[last].SuffLink = ActiveNode;
            last = ActiveNode;
        } else {
            int nextNode = Go(ActiveNode, a);
            int goodLen = RemainingSuffs - 1;
            int edgeLeft = Nodes[nextNode].Left;
            char t = InputString[edgeLeft + goodLen];

            if (t != c) {
                int split = CreateNode(edgeLeft, goodLen);
                int newV = CreateNode(ProcessedSize - 1, MAX_INT);
                Nodes[split].Transitions[t] = nextNode;
                Nodes[split].Transitions[c] = newV;
                Nodes[ActiveNode].Transitions[a] = split;

                Nodes[nextNode].SubstrLen -= goodLen;
                Nodes[nextNode].Left += goodLen;

                Nodes[last].SuffLink = split;
                last = split;
            } else {
                Nodes[last].SuffLink = ActiveNode;
                return;
            }
        }

        if (ActiveNode == 0) {
            --RemainingSuffs;
        } else {
            ActiveNode = Nodes[ActiveNode].SuffLink;
        }
    }
}

void TSuffTree::CreateTRee() {
    ProcessedSize = 0;
    NodesCount = 0;

    ActiveNode = 0;
    RemainingSuffs = 0;

    for (char c: InputString) {
        Extend(c);
    }
}

TSuffTree::TSuffTree(const std::string &str) : InputString(str) {
    ProcessedSize = 0;
    NodesCount = 0;

    CreateNode(0, MAX_INT);
    CreateTRee();
}

int TSuffTree::TypeNodeAndFindRes(int currNode, int currDepth, const std::string & str){

    if (Nodes[currNode].Transitions.empty()){
        if (Nodes[currNode].Left > Separ) {
            Nodes[currNode].NodeType = IS_PATT2;
        } else {
            Nodes[currNode].NodeType = IS_PATT1;
        }
    }

    int result = Nodes[currNode].NodeType;  

    if (!Nodes[currNode].Transitions.empty()) {
        for (const auto& i : Nodes[currNode].Transitions) {
            int nextNode = i.second;
            int lenChildSubstr = Nodes[nextNode].RightWall(ProcessedSize) - Nodes[nextNode].Left;
            result |= TypeNodeAndFindRes(nextNode, currDepth + lenChildSubstr, str);  
        }
    }

    if (result == IS_PATT1N2){
        if (MaxCommLen == currDepth){
            LastIdxsCommSubstr.push_back(Nodes[currNode].RightWall(ProcessedSize));
        }

        else if (currDepth > MaxCommLen){
            MaxCommLen = currDepth;
            LastIdxsCommSubstr.resize(0);
            LastIdxsCommSubstr.push_back(Nodes[currNode].RightWall(ProcessedSize));
        }
    }

    Nodes[currNode].NodeType = result;  

    return result;
}

std::string RandomString(size_t size)
{
    srand(time(nullptr));
    std::string str;
    std::string tmp;
    for (size_t i = 0; i < size; ++i) {
        if (rand() % 15 == 0) {
            tmp = ' ';
        } else {
            tmp = (rand() % 26) + 'a';
        }

        str.append(tmp);
    }

    return str;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    auto start = std::chrono::high_resolution_clock::now();
    int halfSize = 1500000;
    std::string s = RandomString(halfSize) + SENTINEL_A + RandomString(halfSize) + SENTINEL_D;
    TSuffTree st(s);
    st.TypeNodeAndFindRes(0, 0, st.InputString);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Размер строки - " << 2 * halfSize << '\n';
    std::cout << "Время работы программы: " << duration.count() << " ms";
}
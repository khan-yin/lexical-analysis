#include<iostream>
#include<fstream>
#include<cstdio>
#include<vector>
#include<stack>
#include<algorithm>
#include<map>
#include<queue>
#include<string>
#include<map>
#include<set>
#define Node char //定义状态节点
using namespace std;

struct Edge         //存储一条状态转移规则
{
    Node startNode; //状态起点       
    char character; //接收的终结符   
    Node endNode;   //状态终点
};

class NFA
{
    public:
        vector<Edge> NFA_edgelist; //NFA存储的每一条状态转移
        set<char> route;//NFA中的非终结符
        vector<Node> NFA_nodelist;//NFA中所有的状态节点
    public:
        void read_NFA(string str);
        void testnodelist(set<Node> nodeset);
};

void NFA::read_NFA(string str)
{
    if(str=="\n")
        return;
    // cout<<str<<endl;
    NFA_nodelist.push_back(str[0]);
    if(str[0]=='Y')
        return;
    int i=2;
    int step=7;
    // Y - a - > 0   Y -  b  -   >  X
    // 2 3 4 5 6 7 8 9 10 11 12 13 14
    while(i<str.length())
    {
        // cout<<"i"<<" "<<str[i]<<endl;
        Edge edge;
        edge.startNode=str[i];
        edge.character=str[i+2];
        route.insert(str[i+2]);
        edge.endNode=str[i+5];
        NFA_edgelist.push_back(edge);
        i+=step;
    }
}

//主要用于测试NFA的读入节点是否正确
void NFA::testnodelist(set<Node> nodeset)
{
    cout<<"nodelist:"<<endl;
    for(auto node:nodeset)
    {
        cout<<node<<" ";
    }
    cout<<endl;
}
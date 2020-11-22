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
#define Node char
using namespace std;

struct Edge            
{
    Node startNode;        
    char character;    
    Node endNode;    
};



// struct DFA_Edge
// {
//     set<Node> startNode;        
//     char character;    
//     set<Node> endNode;  
// };


void read_NFA(string str,vector<Node>& NFA_nodelist,vector<Edge>& NFA_edgelist,set<char>& route)
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

set<Node> e_closure(set<Node> nodeset,vector<Edge>& NFA_edgelist)
{
    set<Node> newnodeset=nodeset;
    stack<Node> S;
    set<Node>::iterator si;
    vector<Edge>::iterator vi;
    for(si=newnodeset.begin();si!=newnodeset.end();si++)
        S.push(*si);
    Node t;
    while(!S.empty())
    {
        t=S.top();
        S.pop();
        for(vi=NFA_edgelist.begin(); vi!=NFA_edgelist.end();vi++)
        {
            if(t==(*vi).startNode&&(*vi).character=='~')
            {
                newnodeset.insert((*vi).endNode);
                S.push((*vi).endNode);
            }
        }
    }
    return newnodeset;
}

void testnodelist(set<Node> nodeset)
{
    cout<<"nodelist:"<<endl;
    for(set<Node>::iterator si=nodeset.begin();si!=nodeset.end();si++)
    {
        cout<<*si<<" ";
    }
    cout<<endl;
}

void for_statuslist(vector<Edge>& statuslist,Node first,Node end)
{
    for(vector<Edge>::iterator vi=statuslist.begin();vi!=statuslist.end();vi++)
    {
        if((*vi).startNode==first)
        {
            (*vi).startNode='X';
        } 
        else if((*vi).startNode==end)
        {
            (*vi).startNode='Y';
        }
        else
        {
            (*vi).startNode-=1;
        }

        if((*vi).endNode==first)
        {
            (*vi).endNode='X';
        } 
        else if((*vi).endNode==end)
        {
            (*vi).endNode='Y';
        }
        else
        {
            (*vi).endNode-=1;
        }
        cout<<" "<<(*vi).startNode<<"-"<<(*vi).character<<"->"<<(*vi).endNode;    
    }
    cout<<endl;
}

void testedgemap(map<Node,vector<Edge> > DFA_edgemap)
{
    int cnt=0;
    map<Node,vector<Edge> >::iterator iter_begin = DFA_edgemap.begin();
    map<Node,vector<Edge> >::iterator iter_end = DFA_edgemap.end();
    iter_end--;
    Node first=iter_begin->first;
    Node end=iter_end->first;
    cout<<"X";
    for_statuslist(iter_begin->second,first,end);
    cout<<"Y";
    for_statuslist(iter_end->second,first,end);
    for(map<Node,vector<Edge> >::iterator iter = ++iter_begin; iter != iter_end; iter++)  
    {
        cout<<iter->first-'1';
        for_statuslist(iter->second,first,end);
    }
    
}

set<Node> move(set<Node> nodeset,char ch, vector<Edge>& NFA_edgelist)
{
    set<Node>::iterator si;
    vector<Edge>::iterator vi;
    set<Node> newNodeset;
    for(si=nodeset.begin();si!=nodeset.end();si++)
    {
        for(vi=NFA_edgelist.begin(); vi!=NFA_edgelist.end();vi++)
        {
            if(*si==(*vi).startNode&&(*vi).character==ch)
            {
                newNodeset.insert((*vi).endNode);
            }
        }
    }
    return newNodeset;
}

//查找新结点的位置
int find_index(vector<set<Node> >list,set<Node> s)
{
    for(vector<set<Node> >::iterator i=list.begin();i!=list.end();i++)
    {
        if((*i)==s)
        {
            return i-list.begin();
        }
    }
}

int main()
{
    vector<Edge> NFA_edgelist; 
    set<char> route;
    vector<Node> NFA_nodelist;
    set<set<Node> >  DFA_nodelist;
    map<Node,vector<Edge> > DFA_edgemap;

    string filename="NFA.txt";
    ifstream infile(filename.c_str());
    string temp="";
    while(getline(infile,temp))
    {
        read_NFA(temp,NFA_nodelist,NFA_edgelist,route);
    }
    // cout<<"a"<<endl;
    //-std=c++11
    // for(auto node: NFA_nodelist)
    // {
    //     cout<<node;
    // }

    // for(auto edge: NFA_edgelist)
    // {
    //     cout<<edge.startNode<<"-"<<edge.character<<"-"<<">"<<edge.endNode<<endl;
    // }
    //vector<int>::iterator iter
    set<Node> initset;
    vector<set<Node> > sk;
    initset.insert(NFA_nodelist[0]);
    initset=e_closure(initset,NFA_edgelist);
    DFA_nodelist.insert(initset);
    sk.push_back(initset);
    int i=0;
    while(i<sk.size())
    {
        set<Node> t = sk[i];
        vector<Edge> edgeline;
        for(set<Node>::iterator it=route.begin();it!=route.end();it++)
        {
            if(*it=='~')
                continue;
            set<Node> newNodeset=e_closure(move(t,*it,NFA_edgelist),NFA_edgelist);
            Edge edge;
            edge.startNode=i+'0';
            edge.character=*it;
            if(newNodeset.size()==0)//空集合不算入新状态
                continue;
            if(DFA_nodelist.find(newNodeset)==DFA_nodelist.end())
            {
                DFA_nodelist.insert(newNodeset);
                edge.endNode=sk.size()+'0';
                sk.push_back(newNodeset);
            }
            else{
                edge.endNode=find_index(sk,newNodeset)+'0';
            }
            
            edgeline.push_back(edge);
        }
        
        DFA_edgemap.insert(pair<Node,vector<Edge> >(i+'0',edgeline));
        i++;
    }
    
    // cout<<DFA_nodelist.size()<<endl;
    testedgemap(DFA_edgemap);
    return 0;
}
#include"NFA.h"
set<Node> e_closure(set<Node> nodeset,vector<Edge>& NFA_edgelist);//求e_closure闭包
set<Node> move(set<Node> nodeset,char ch, vector<Edge>& NFA_edgelist);//move函数
int find_index(vector<set<Node> >list,set<Node> s);

class DFA
{
    public:
        set<set<Node> >  DFA_nodelist;//未化简的DFA中所有的状态
        map<Node,vector<Edge> > DFA_edgemap;//每个节点对应的状态转移规则
    public:
        void testedgemap();
        void print_statuslist(vector<Edge>& statuslist,Node first,Node end);
        void NFA2DFA(NFA& nfa);
};

//打印问题二输出结果，即所有节点的状态转移规则
void DFA::testedgemap()//测试
{
    int cnt=0;
    auto iter_begin = DFA_edgemap.begin();//初态X
    auto iter_end = DFA_edgemap.end();//end是最后一个元素的下一个，是空，所以需要--
    iter_end--;//终态Y
    Node first=iter_begin->first;
    Node end=iter_end->first;
    cout<<"X";
    print_statuslist(iter_begin->second,first,end);
    cout<<"Y";
    print_statuslist(iter_end->second,first,end);
    for(auto iter = ++iter_begin; iter != iter_end; iter++)  //从初态X的下一个开始遍历
    {
        cout<<iter->first-'1';
        print_statuslist(iter->second,first,end);
    }
}

//打印某一个节点的状态转移规则
void DFA::print_statuslist(vector<Edge>& statuslist,Node first,Node end)
{
    for(auto edge:statuslist)
    {
        if(edge.startNode==first)
        {
            edge.startNode='X';
        } 
        else if(edge.startNode==end)
        {
            edge.startNode='Y';
        }
        else
        {
            edge.startNode-=1;
        }

        if(edge.endNode==first)
        {
            edge.endNode='X';
        } 
        else if(edge.endNode==end)
        {
            edge.endNode='Y';
        }
        else
        {
            edge.endNode-=1;
        }
        cout<<" "<<edge.startNode<<"-"<<edge.character<<"->"<<edge.endNode;    
    }
    cout<<endl;
}

void DFA::NFA2DFA(NFA& nfa)
{
    vector<Edge> NFA_edgelist=nfa.NFA_edgelist;
    set<char> route=nfa.route;
    vector<Node> NFA_nodelist=nfa.NFA_nodelist;

    set<Node> initset;//初始闭包集合，即就是X的e_closure闭包
    vector<set<Node> > tempDFA_nodelist;
    //暂存DFA新状态集合，记录所有产生的状态和位置，模拟状态转移表的过程
    //主要用于查找edge对应endnode的正确位置，为后面的map做准备
    
    initset.insert(NFA_nodelist[0]);
    initset=e_closure(initset,NFA_edgelist);//求X的e_closure闭包

    DFA_nodelist.insert(initset);//DFA新增初始状态集合

    tempDFA_nodelist.push_back(initset);//把初始状态集合放入暂存的DFA新状态集合

    int i=0;//记录状态序号
    while(i<tempDFA_nodelist.size())
    {
        set<Node> temp = tempDFA_nodelist[i];
        vector<Edge> edgeline;
        for(auto ch:route)
        {
            if(ch=='~')//move函数处理的是非空终结符，遇到终结符则跳过
                continue;
            set<Node> newNodeset=e_closure(move(temp,ch,NFA_edgelist),NFA_edgelist);//用S'=e_closure(move(S),a)求出新闭包
            Edge edge;
            edge.startNode=i+'0';//序号转字符Node
            edge.character=ch;
            if(newNodeset.size()==0)//空集合不算入新状态
                continue;
            if(DFA_nodelist.find(newNodeset)==DFA_nodelist.end())
            {
                DFA_nodelist.insert(newNodeset);
                edge.endNode=tempDFA_nodelist.size()+'0';
                tempDFA_nodelist.push_back(newNodeset);
            }
            else{
                edge.endNode=find_index(tempDFA_nodelist,newNodeset)+'0';
            }
            
            edgeline.push_back(edge);
        }
        DFA_edgemap.insert(pair<Node,vector<Edge> >(i+'0',edgeline));//状态记录表新增一条当前node的转移规则
        i++;
    }
}

//求e_closure
set<Node> e_closure(set<Node> nodeset,vector<Edge>& NFA_edgelist)
{
    set<Node> newnodeset=nodeset;
    stack<Node> S;
    for(auto node:newnodeset)
        S.push(node);
    Node temp;
    while(!S.empty())
    {
        temp=S.top();
        S.pop();
        for(auto edge:NFA_edgelist)
        {
            if(temp==edge.startNode&&edge.character=='~')//特判,如果终结符为epsilon则加入这个新节点
            {
                newnodeset.insert(edge.endNode);
                S.push(edge.endNode);
            }
        }
    }
    return newnodeset;
}

//move函数
set<Node> move(set<Node> nodeset,char ch, vector<Edge>& NFA_edgelist)
{
    set<Node> newNodeset;//新的状态集合
    for(auto setItem:nodeset)
    {
        for(auto edge:NFA_edgelist)
        {
            if(setItem==edge.startNode&&edge.character==ch)
            {
                newNodeset.insert(edge.endNode);
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
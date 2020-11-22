#include"DFA.h"


int main()
{
    // cout<<"hello"<<endl;
    NFA nfa_automachine= NFA();
    DFA dfa_automachine= DFA();
    set<Node> initset;//初始闭包集合，即就是X的e_closure闭包
    vector<set<Node> > tempDFA_nodelist;
    //暂存DFA新状态集合，记录所有产生的状态和位置，模拟状态转移表的过程
    //主要用于查找edge对应endnode的正确位置，为后面的map做准备

    string filename="NFA.txt";//用于测试，以文件方式读入
    ifstream infile(filename.c_str());
    string temp="";
    while(getline(infile,temp))
    {
        nfa_automachine.read_NFA(temp);
    }
    dfa_automachine.NFA2DFA(nfa_automachine);

    dfa_automachine.testedgemap();//输出结果
    
    return 0;
}


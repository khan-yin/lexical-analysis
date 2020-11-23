#include<iostream>
#include<string>
#include<vector>

using namespace std;

struct status{
    char name; //状态名
    vector<char> accept; //可接受字符
    vector<char> change; //接收字符对应转换状态
};

vector<status> state_set; //初始状态集
vector<vector<status>> result_set; //化简状态集合

//读取数据行
void read(){
    while(true){
        string line;
        getline(std::cin,line);
        //cout << line << "%" << endl;
        if(line.length() != 0){
            status state;
            state.name = line[0]; //读取状态名

            for(int i = 1; i < line.length(); i++){
                if(line[i] == line[0] && line[i + 1] == '-'){
                    int j = i + 1;
                    state.accept.push_back(line[i + 2]); //读取可接受字符
                    while(j < line.length()){
                        if(line[j] == '>'){
                            state.change.push_back(line[j + 1]); //读取转换的状态
                            break;
                        }
                        j++;
                    }
                }
            }
            
            state_set.push_back(state); //加入到状态集
        }else{
            break;
        }
    }
}

//测试数据读取
void test(){
    string lines[] = {"X X-a->0 X-b->1","Y Y-a->0 Y-b->1","0 0-a->0 0-b->2",
                      "1 1-a->0 1-b->1","2 2-a->0 2-b->Y"};

    for(int k = 0; k < 5; k++){
        string line = lines[k];

        status state;
        state.name = line[0]; //读取状态名

        for(int i = 1; i < line.length(); i++){
            if(line[i] == ' '){
                state.accept.push_back(line[i + 3]); //读取可接受字符
                state.change.push_back(line[i + 6]); //读取转换的状态
            }
        }

        state_set.push_back(state); //加入到状态集
    }
}

//计算转换集
int changeTo(status state,int letter){
    char change = state.change.at(letter);

    for(int k = 0; k < result_set.size(); k++){
        vector<status> set = result_set.at(k);
        for(int i = 0; i < set.size(); i++){
            if(set.at(i).name == change){
                return k;
            }
        }
    }
    return -1;
}

//DFN化简
void simplify(){
    //初始状态
    vector<status> set; //非终态集
    vector<status> end_set; //终态集
    for(int i = 0; i < state_set.size(); i++){
        status state = state_set.at(i);
        if(state.name == 'Y')
            end_set.push_back(state);
        else
            set.push_back(state);
    }

    //将两个状态集加入
    result_set.push_back(set);
    result_set.push_back(end_set);

    //开始化简
    while(true){
        bool finish = true; //标记是否已全部分割完成
        vector<vector<status>> new_res; //存储每次得到的结果集
        int length = result_set.size();
        
        for(int i = 0; i < length; i++){
            vector<status> set = result_set.at(i);
            //统计转换集
            status first = set.at(0); //第一个状态作为标准
            vector<status> temp = set;
            vector<status> another; //存储分离出来的状态集
            for(int j = 0; j < first.change.size(); j++){
                int k = changeTo(first,j);
                int len = set.size();
                
                for(int t = 1; t < len; t++){
                    status s = set.at(t);
                    if(changeTo(s,j) != k){ //接收同一个字符后转换状态集不同
                        finish = false;
                        another.push_back(s); //加入分离的另一个状态集
                        temp.erase(temp.begin() + t); //原状态集删除此状态
                    }
                }
            }

            if(finish) //已全部分割完成，直接结束该方法
                return;

            new_res.push_back(temp);
            if(another.size() > 0)
                new_res.push_back(another);

        }
        result_set = new_res;
    }
}

int main(){
    //test();
    read();
    for(int i = 0; i < state_set.size(); i++){
        status state = state_set.at(i);
        cout << state.name << " ";
        for(int j = 0; j < state.accept.size(); j++){
            cout << state.name << "-" << state.accept.at(j) << "->" << state.change.at(j) << " ";
        }
        cout << endl;
    }

    //输出最终化简结果
    /*
    simplify();
    for(int i = 0; i < state_set.size(); i++){
        char name = state_set.at(i).name;
        for(int p = 0; p < result_set.size(); p++){
            vector<status> st = result_set.at(p);
            status s = st.at(0);
            if(s.name == name){
                cout << s.name << " ";
                for(int r = 0; r < s.accept.size(); r++){
                    cout << s.name << "-" << s.accept.at(r) << "->" << s.change.at(r) << " ";
                }
                cout << endl;
            }
        }
    }
    */
    return 0;
}
#ifndef DECODER_NEW_H
#define DECODER_NEW_H

#include<bits/stdc++.h>
using namespace std;
inline int getnum(string s)//将16进制的string转化为int
{
    int ans=0;
    s=s.substr(2,s.size()-2);
    for(auto it:s)
    {
        if(it>='0'&&it<='9') ans=ans*16+it-'0';
        else if(it>='a'&&it<='f') ans=ans*16+it-'a'+10;
        else
        {
            printf("Error:This string is not a hexadecimal representation string");
            exit(0);
        }
    }
    return ans;
}
struct Trie//字典树
{
    struct Node
    {   
        int son[2];
        unsigned char ch;
    };
    int root=0,now=0,siz;
    vector<Node> tre;
    vector<pair<string,unsigned char> > mp;
    void get_code_map(string code_txt)//创建映射表
    {
        std::ifstream fin(code_txt.c_str());
        if (!fin.is_open()) {
            printf("Error: The file cannot open\n");// 处理文件打开失败的情况，例如抛出异常或返回错误
            exit(0); // 或其他错误处理
        }

        int n;
        std::string str;
        fin >> str; // 原代码中的第一个读取，根据实际情况可能需要保留或调整
        siz = stoi(str);

        while (fin >> str) {
            unsigned char ch = getnum(str);
            fin >> str;
            int len = getnum(str);
            n = (len + 7) / 8;
            std::string ans;
            for (int j = 1; j <= n; j++) {
                fin >> str;
                int num = getnum(str);
                for (int k = 8; k >= 1; k--) {
                    ans += ((num >> (k - 1)) & 1) ? "1" : "0";
                }
            }
            mp.push_back({ans.substr(0, len), ch});
        }

        fin.close();
    }
    void insert(string s,unsigned char ch)//向字典树中加入字符串
    {
        int pos=root;
        for(auto it:s)
        {
            int p=it-'0';
            if(tre[pos].son[p]== -1)
            {
                tre.push_back({{-1,-1},0});
                tre[pos].son[p]=tre.size()-1;
            }
            pos=tre[pos].son[p];
        }
        tre[pos].ch=ch;
    }
    void init(string code_txt)//根据映射表建立字典树
    {
        get_code_map(code_txt);
        tre.push_back({{-1,-1},0});
        for(auto it:mp)
            insert(it.first,it.second);
    }
    pair<bool,unsigned char> find(int pos)//查询
    {
        // cout<<pos<<endl;
        if(tre[now].son[pos] == -1)
        {
            printf("Error : The node does not exist in the Trie tree\n");
            exit(0);
        }
        now=tre[now].son[pos];
        if(tre[now].son[0]==-1&&tre[now].son[0]==-1)
        {
            unsigned char ch=tre[now].ch; now=root;
            return {true,ch};
        }
        return {false,0};
    }
};
class HFM_decoder
{
    public:
    string decode(string hfm_file,string code_txt)//解码
    {
        Trie T; T.init(code_txt);//预处理code.txt
        std::ifstream file(hfm_file.c_str(), std::ios::binary);  // 以二进制模式打开文件
        if (!file) {
            cerr << "Error: can't open this file to read" << endl;
            exit(0);
        }

        string ans,t;
        string content;
        unsigned char byte;

        while (file.read(reinterpret_cast<char*>(&byte), 1)) {  // 逐字节读取
            std::string bits = std::bitset<8>(byte).to_string();  // 转换为8位二进制字符串
            content += bits;  // 追加到字符串
            int pos=0; T.now=T.root;
            for(int i=0;i<content.size();i++)
            {
                unsigned char it=content[i];
                auto ret=T.find(it-'0');
                if(ans.size()==T.siz)
                {
                    content=content.substr(pos,content.size()-pos);
                    goto write_end;//结束解码过程
                }
                if(ret.first)
                {
                    ans+=ret.second;
                    pos=i+1;
                }
            }
            content=content.substr(pos,content.size()-pos);
        }
        
        write_end:
        file.close();
        
        bool flag=true;
        for(auto it:content)
            if(it=='1')
                flag=false;
        if(!flag)//判断文件不合法的情况
        {
            printf("Error:The.hfm file or code.txt is incorrect\n");
            exit(0);
        }
        return ans;
    }
};

#endif
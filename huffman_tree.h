#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include<bits/stdc++.h>
#include <queue>
#include <unordered_map>
#include"minheap.h"
#include"out_in_hex.h"
#define ull unsigned long long
using namespace std;
struct Tree_node
{
    int l,r,dat;
    unsigned char ch;
};
class Huffman_tree
{
    public:
    vector<Tree_node> tre;//存储哈夫曼树的节点
    unordered_map<unsigned char,string> enc;//编码信息映射表
    int root,wpl;

    //arr存储一个二元组数组表示频度表，每个元素表示(频度，字符)
    void build(vector<pair<int,unsigned char> > arr)//构建哈夫曼树
    {
        // priority_queue<pair<pair<int,unsigned char>,int> ,vector<pair<pair<int,unsigned char>,int> >,greater<> > q;
        MinHeap q;
        for(auto it : arr)
            tre.push_back({-1,-1,it.first,it.second});
        for(int i=0;i<tre.size();i++)
            q.push({{tre[i].dat,tre[i].ch},i});
        while(!q.empty())
        {
            pair<pair<int,unsigned char>,int> x=q.top(); q.pop();
            if(q.empty())
            {
                root=x.second;
                break;
            }
            pair<pair<int,unsigned char>,int> y=q.top(); q.pop();
            Tree_node it={x.second,y.second,x.first.first+y.first.first,max(x.first.second,y.first.second)};
            tre.push_back(it);
            q.push({{it.dat,it.ch},(int)tre.size()-1});
        }
    }

    int get_wpl()//求wpl
    {
        wpl=0;
        for(auto it:tre)
            if(it.l!=-1)
                wpl+=it.dat;
        return wpl;
    }

    //x表示递归的节编号，str记录当前节点到根结点的编码串
    void dfs(int x,string str)//递归求每个字符的编码
    {
        if(tre[x].l==-1 && tre[x].r==-1)
        {
            enc.insert({tre[x].ch,str});
            return ;
        }
        dfs(tre[x].l,str+"0");
        dfs(tre[x].r,str+"1");
    }
    
    //arr存储一个二元组数组表示频度表，每个元素表示(频度，字符)
    void print_arr(vector<pair<int,unsigned char> > arr)//输出频率表
    {
        printf("\n************频率表**********\nbyte freq\n");
        for(auto it:arr)
            cout<<num_to_16(it.second)<<' '<<it.first<<endl;
    }
    //s为.txt文本中读取的文本串
    void init(string s)//初始化哈夫曼树
    {
        unordered_map<unsigned char,int> mp;
        for(auto it:s)
        {
            if(mp.find(it) == mp.end())
                mp.insert({it,1});
            else mp[it]++;
        }
        vector<pair<int,unsigned char> > arr;
        for(auto it:mp)
            arr.push_back({it.second,it.first});
        print_arr(arr);
        build(arr);
        wpl=get_wpl();
        dfs(root,"");
    }
    //s为.txt文本中读取的文本串，返回编码后的串
    string encode(string s) // 编码
    {
        init(s);
        string binaryString,ans;
        for(auto it:s)
            binaryString+=enc.find(it)->second;
        for (size_t i = 0; i < binaryString.length(); i += 8) {
            std::string byteString = binaryString.substr(i, 8);
            if (byteString.length() < 8) {
                // 如果剩余的位数不足8位，可以填充0或者抛出异常
                byteString.append(8 - byteString.length(), '0');
            }
            unsigned char byte = 0;
            for (size_t j = 0; j < 8; ++j) {
                if (byteString[j] == '1') {
                    byte |= (1 << (7 - j));
                }
            }
            ans.push_back(byte);
        }
        return ans;
    }
    vector<pair<unsigned char,string> > get_table()//得到编码表
    {
        vector<pair<unsigned char,string> > ans;
        for(auto it:enc)
            ans.push_back({it.first,it.second});
        sort(ans.begin(),ans.end());
        return ans;
    }
};

#endif
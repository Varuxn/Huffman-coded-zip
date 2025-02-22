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
    // unordered_map<string,unsigned char> dec;//解码信息映射表
    int root,wpl;
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
            // cout<<(int)x.first.second<<' '<<(int)y.first.second<<' '<<(int)max(x.first.second,y.first.second)<<endl;
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
    void dfs(int x,string str)//递归求每个字符的编码
    {
        if(tre[x].l==-1 && tre[x].r==-1)
        {
            enc.insert({tre[x].ch,str});
            // dec.insert({str,tre[x].ch});
            return ;
        }
        // cout<<tre[x].l<<' '<<tre[x].r<<endl;
        dfs(tre[x].l,str+"0");
        dfs(tre[x].r,str+"1");
    }
    void print_arr(vector<pair<int,unsigned char> > arr)//输出频率表
    {
        printf("\n************频率表**********\nbyte freq\n");
        for(auto it:arr)
            cout<<num_to_16(it.second)<<' '<<it.first<<endl;
    }
    void init(string s)//初始化哈夫曼树
    {
        unordered_map<unsigned char,int> mp;
        for(auto it:s)
        {
            if(mp.find(it) == mp.end())
                mp.insert({it,1});
            else mp[it]++;
            // if(it=='\n') cout<<"Test"<<endl;
            // cout<<it;
        }
        vector<pair<int,unsigned char> > arr;
        for(auto it:mp)
            arr.push_back({it.second,it.first});
        print_arr(arr);
        build(arr);
        wpl=get_wpl();
        dfs(root,"");
        // for(auto it:enc)
        // {
        //     bool flag=false;
        //     for(auto ch:it.second)
        //         if(ch=='1')
        //             flag=true;
        //     if(!flag)
        //     {
        //         enc[it.first]="0";
        //         continue;
        //     }
        //     while(enc[it.first][0]=='0') enc[it.first]=enc[it.first].substr(1,enc[it.first].size());
        //     // cout<<it.second<<endl;
        // }
    }
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
        // cout<<s.size()<<' '<<binaryString.size()<<' '<<ans.size()<<endl;
        return ans;
    }
    vector<pair<unsigned char,string> > get_table()//得到编码表
    {
        vector<pair<unsigned char,string> > ans;
        for(auto it:enc)
        {
            ans.push_back({it.first,it.second});
            // cout<<it.second<<endl;
        }
        sort(ans.begin(),ans.end());
        // for(auto it:ans)
        //     cout<<it.first<<' '<<(int)it.first<<' '<<it.second<<endl;
        return ans;
    }
    // int decode(string x) // 解码
    // {
    //     if(dec.find(x)==dec.end())
    //     {
    //         printf("Decode Error: Not exist\n");
    //         return -1;
    //     }
    //     return dec.find(x)->second;
    // }
};

#endif
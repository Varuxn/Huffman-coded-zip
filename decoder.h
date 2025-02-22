#ifndef DECODER_H
#define DECODER_H


#include<bits/stdc++.h>
using namespace std;
#define ull unsigned long long
const ull base=13331ull;
inline int getnum(string s)
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
ull get_hash(string s)
{
    ull has=0;
    for(auto it:s)
        has=has*base+it;
    return has;
}
class HFM_decoder
{
    public:
    int n,siz;
    unordered_map<ull,unsigned char> mp;
    string decode(string hfm_file,string code_txt)
    {
        string content;
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
            mp.insert({get_hash(ans.substr(0, len)),ch});
        }

        fin.close();

        unsigned char byte;
        std::ifstream file(hfm_file.c_str(), std::ios::binary);  // 以二进制模式打开文件
        if (!file) {
            cerr << "Error: can't open this file to read" << endl;
            exit(0);
        }
        ull has=0;
        string ans;
        while (file.read(reinterpret_cast<char*>(&byte), 1)) {  // 逐字节读取
            std::string bits = std::bitset<8>(byte).to_string();  // 转换为8位二进制字符串
            content = bits;  // 追加到字符串
            int pos=0;
            for(int i=0;i<content.size();i++)
            {
                unsigned char it=content[i];
                has=has*base+it;
                if(mp.find(has)!=mp.end())
                {
                    ans+=mp.find(has)->second;
                    has=0; pos=i+1;
                }
                if(ans.size()==siz)
                    goto write_end;//结束解码过程
            }
        }
        write_end:
        return ans;
    }
};

#endif
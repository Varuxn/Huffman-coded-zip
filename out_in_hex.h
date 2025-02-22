#ifndef OUT_IN_HEX_H
#define OUT_IN_HEX_H

#include <bits/stdc++.h>
using namespace std;

char d[20]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
string num_to_16(int num)//将数字转化为16进制表示的string
{
    string ans="0x";
    // if(num/16>15) cout<<num<<endl;
    ans+=d[num/16];
    ans+=d[num%16];
    // cout<<ans<<' '<<num/16<<' '<<num%16<<endl;
    return ans;
}
string str_to_16(string s)//将01字符串变化为16进制表示的字符串
{
    string ans;
    // cout<<s.size()<<endl;
    ans=num_to_16(s.size())+" ";
    // if(ans=="0x43 ") cout<<s<<endl;
    // cout<<ans<<' '<<s<<endl;
    while(s.size()%8!=0) s+="0";
    for(int i=0;i<s.size();i+=8)
    {
        int num=0;
        for(int j=i;j<=i+7;j++)
            num=(num<<1)|(s[j]-'0');
        // if(num<0) cout<<num<<endl;
        ans+=num_to_16(num)+" ";
    }
    ans.pop_back();
    return ans;
}

string get_last_16(string s)//获得一个字符串的最后16位并且按照16进制表示。
{
    string ans;
    if(s.size()<16)
    {
        printf("Error:The file is too small to output the last 16 bits\n");
        exit(0);
    }
    for(int i=s.size()-16;i<s.size();i++)
        ans+=num_to_16((unsigned char)s[i])+" ";
    // for(int i=s.size()-16;i<s.size();i++)
    //     cout<<(int)s[i]<<' ';
    // cout<<endl;
    return ans;
}

#endif  // OUT_IN_HEX_H
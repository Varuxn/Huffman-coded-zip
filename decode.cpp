#include<bits/stdc++.h>
#include"huffman_tree.h"
#include"hash.h"
#include"file.h"
#include"out_in_hex.h"
// #include"decoder.h"
#include"decoder_new.h"
using namespace std;
std::mt19937_64 rnd(std::chrono::steady_clock().now().time_since_epoch().count());
const string code="code.txt";
const string config_file="config.txt";
string enc_flag,user_flag;
string file_hfm,file_j;
string de_content;
string inid,outid,inname,outname;
HFM_decoder decoder;
int encipher,keyyyy;
unsigned long long jyh;
vector<unsigned char> de_map;
void kwd_out()
{
    std::ifstream kwd_file(config_file.c_str());
    if (!kwd_file) {
        std::cerr << "Error : cannot read "<< config_file << std::endl;
        exit(0);
    }
    int tot_num;
    kwd_file>>file_hfm>>file_j;
    kwd_file>>enc_flag>>user_flag;
    kwd_file>>encipher>>keyyyy;
    kwd_file>>jyh>>tot_num;
    for(int i=1;i<=tot_num;i++)
    {
        int x; kwd_file>>x;
        de_map.push_back(x);
    }
    kwd_file.close();
}

void decode()//解码过程
{
    //记录开始时间节点
    printf("If you need to decrypt the key, enter an integer between 0 and 255, otherwise enter 0 : ");
    int keyyyy; cin>>keyyyy;
    
    auto start_time = std::chrono::high_resolution_clock::now();

    de_content=decoder.decode(file_hfm,code);

    //********************************
    if(enc_flag=="y")
    {
        for(int i=0;i<de_content.size();i++)//映射解密
            de_content[i]=de_map[(unsigned char)de_content[i]];
        for(int i=0;i<de_content.size();i++)//偏移量解密
            de_content[i]-=encipher;
        for(int i=0;i<de_content.size();i++)//异或解密
            de_content[i]^=keyyyy;

    }
    //********************************

    unsigned long long check_num=0;
    for(auto it:de_content) check_num=check_num*1331ull+it;
    cout<<"Check sum is: "<<check_num<<endl;
    if(check_num!=jyh)
    {
        cout<<"The checksum is inconsistent and the file may be corrupted\n";
        exit(0);
    }
    //记录结束时间节点
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    string ori_content;
    if(user_flag=="y")//根据需求检验收发人信息
    {
        int pre=0,pos=0;
        for(int i=pre;i<de_content.size();i++)if(de_content[i]==','){pos=i;break;}
        inid=de_content.substr(pre,pos-pre); pre=pos+1;
        for(int i=pre;i<de_content.size();i++)if(de_content[i]=='\n'){pos=i;break;}
        inname=de_content.substr(pre,pos-pre); pre=pos+1;
        for(int i=pre;i<de_content.size();i++)if(de_content[i]==','){pos=i;break;}
        outid=de_content.substr(pre,pos-pre); pre=pos+1;
        for(int i=pre;i<de_content.size();i++)if(de_content[i]=='\n'){pos=i;break;}
        outname=de_content.substr(pre,pos-pre); pre=pos+1;
        ori_content=de_content.substr(pre,de_content.size()-pre);
        string x,y;
        printf("Please input the receiver ID : ");  cin>>x;
        printf("Please input the receiver name : "); cin>>y;
        if(x!=outid||y!=outname)
        {
            printf("Error : Unable to extract, the recipient's name or information is incorrect\n");
            return ;
        }
        printf("Identity check successful\n");
    }
    
    file_write(file_j, de_content);

    printf("The last 16 bits of the decoded file are : \n");
    cout<<get_last_16(de_content)<<endl;
    printf("The Hash value of the file after decoding is : ");
    cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(de_content.c_str(),de_content.size()) << endl;
    std::cout << std::dec;
    if(user_flag=="y")
    { 
        printf("The Hash value of the decoded file without sender and receiver information is : ");
        cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(ori_content.c_str(),ori_content.size()) << endl;
        std::cout << std::dec;
    }

    // 输出时间
    printf("The decoding process takes %lld ms\n",(long long)duration_time.count());

}
int main()
{
    decode();
    return 0;
}
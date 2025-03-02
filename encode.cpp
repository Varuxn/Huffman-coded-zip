#include<bits/stdc++.h>
#include"huffman_tree.h"
#include"hash.h"
#include"file.h"
#include"out_in_hex.h"
using namespace std;
std::mt19937_64 rnd(std::chrono::steady_clock().now().time_since_epoch().count());
const string code="code.txt";
const string config_file="config.txt";
vector<unsigned char> en_map;
string enc_flag,user_flag;
string file,file_hfm,file_j;
string content,en_content;
string inid,outid,inname,outname;
Huffman_tree coder;
int encipher,keyyyy;
unsigned long long jyh;
void kwd_in()
{
    std::ofstream kwd_file(config_file.c_str());
    if (!kwd_file) {
        std::cerr << "Error : cannot write "<< config_file << std::endl;
        exit(0);
    }
    kwd_file<<file_hfm<<' '<<file_j<<endl;
    kwd_file<<enc_flag<<' '<<user_flag<<endl;
    kwd_file<<encipher<<' '<<keyyyy<<endl;
    kwd_file<<jyh<<endl<<en_map.size()<<endl;
    for(auto it:en_map) kwd_file<<it<<' ';
    kwd_file.close();
}
//siz为圆txt文件的字节数，s的每一个元素为为表示(字符，哈夫曼01编码)的二元组，表示编码表
void get_code_txt(int siz,vector<pair<unsigned char,string> > s)//获得code.txt的内容，并且写到文件里
{
    string ans;
    ans=to_string(siz)+"\n";
    for(auto it:s)
    {
        unsigned char ch=it.first;
        string str=num_to_16((unsigned char) ch)+" "+str_to_16(it.second)+"\n";
        ans+=str;
    }
    file_write(code,ans);
}
//s使用二元组表示编码表
void print_code_txt(vector<pair<unsigned char,string> > s)//输出编码表
{
    printf("\n************编码表**********\nbyte length code\n");
    for(auto it:s) cout<<num_to_16((unsigned char) it.first)+" "+str_to_16(it.second)+"\n";
}
void encode()//编码过程
{
    content=file_read(file);
    string ori_content=content;
    if(user_flag=="y")//根据需要加入收发人信息
        content=inid+","+inname+"\n"+outid+","+outname+"\n"+content;

    printf("Do you want to encrypt files ?(y/n)\n");
    cin>>enc_flag;
    while(enc_flag!="y"&&enc_flag!="n")
    {
        printf("The input character is incorrect. Please enter y or n :");
        cin>>enc_flag;
    }
    printf("If you need key encryption, enter an integer between 0 and 255, otherwise enter 0 : ");
    int keyyyy; cin>>keyyyy;
    //********************************

    if(enc_flag=="y")
    {
        encipher=0x55;
        for(int i=0;i<content.size();i++)//异或加密
            content[i]^=keyyyy;
        for(int i=0;i<content.size();i++)//偏移量加密
            content[i]+=encipher;
        for(int i=0;i<content.size();i++)//映射加密
            content[i]=en_map[(unsigned char)content[i]];
    }

    //*******************************

    en_content = coder.encode(content);
    vector<pair<unsigned char,string> > code_txt_content = coder.get_table();
    print_code_txt(code_txt_content);

    for(auto it:content) jyh=jyh*1331ull+it;
    cout<<"Check sum is: "<<jyh<<endl;
    printf("The last 16 bytes of the file before encoding are(加密过) : \n");
    cout<<get_last_16(content)<<endl;
    printf("The Hash value of the file before encoding is : ");
    cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(content.c_str(),content.size()) << endl;
    std::cout << std::dec;
    if(user_flag=="y")
    {
        printf("The Hash value of the encoded file without sender and receiver information is : ");
        cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(ori_content.c_str(),ori_content.size()) << endl;
        std::cout << std::dec;
    }

    printf("The WPL of Huffman tree is : %d\n",coder.wpl);
    printf("The last 16 bytes of the file after encoding are : \n");
    cout<<get_last_16(en_content)<<endl;
    printf("The Hash of the file after encoding is : ");
    cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(en_content.c_str(),en_content.size()) << endl;
    std::cout << std::dec;

    printf("Compressed file size : %d\n",(int)en_content.size());
    if(user_flag=="y")
    {
        printf("SENDER INFO:"); cout<<inid<<','<<inname<<endl;
        printf("RECEIVER INFO:"); cout<<outid<<','<<outname<<endl;
    }

    file_write(file_hfm,en_content);
    get_code_txt(content.size(),code_txt_content);

    printf("The file compression rate is %.2lf %%\n",8.0*content.size()/(1.0*en_content.size()));
}

void init()//初始化文件名称等
{
    for(int i=0;i<=255;i++) en_map.push_back(i);
    shuffle(en_map.begin(),en_map.end(),rnd);

    printf("Please input the filename : ");
    cin>>file;
    while(file.substr(file.size()-4,4)!=".txt")
    {
        printf("Error: The file must end in \".txt\"\n");
        printf("Please input the filename again : ");
        cin>>file;
    }
    
    file_hfm = file.substr(0,file.size()-4)+".hfm";
    file_j = file.substr(0,file.size()-4)+"_j.txt";

    printf("Do you want to store sender and receiver information?(y/n)\n");
    cin>>user_flag;
    while(user_flag!="y"&&user_flag!="n")
    {
        printf("The input character is incorrect. Please enter y or n :");
        cin>>user_flag;
    }

    if(user_flag == "y")
    {
        printf("Please input the sender ID : "); cin>>inid;
        printf("Please input the sender name : "); cin>>inname;
        printf("Please input the receiver ID : "); cin>>outid;
        printf("Please input the receiver name : "); cin>>outname;
    }
}
int main()
{
    init();
    encode();
    return 0;
}
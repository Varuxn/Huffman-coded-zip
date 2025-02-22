#include<bits/stdc++.h>
#include"huffman_tree.h"
#include"hash.h"
#include"file.h"
#include"out_in_hex.h"
// #include"decoder.h"
#include"decoder_new.h"
using namespace std;
std::mt19937_64 rnd(std::chrono::steady_clock().now().time_since_epoch().count());
string file,inid,outid,inname,outname;
string file_hfm,file_j,code="code.txt";

string hfm_content,de_content;

string content,en_content;
Huffman_tree coder;
string enc_flag,user_flag;
int encipher;
unsigned long long jyh;
void get_code_txt(int siz,vector<pair<unsigned char,string> > s)//获得code.txt的内容，并且写到文件里
{
    string ans;
    ans=to_string(siz)+"\n";
    for(auto it:s)
    {
        unsigned char ch=it.first;
        string str=num_to_16((unsigned char) ch)+" "+str_to_16(it.second)+"\n";
        ans+=str; //cout<<str;
    }
    file_write(code,ans);
}
void print_code_txt(vector<pair<unsigned char,string> > s)//输出编码表
{
    printf("\n************编码表**********\nbyte length code\n");
    for(auto it:s) cout<<num_to_16((unsigned char) it.first)+" "+str_to_16(it.second)+"\n";
}
unsigned char en_map[500],de_map[500];
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
    cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(content.c_str(),strlen(content.c_str())) << endl;
    std::cout << std::dec;
    if(user_flag=="y")
    {
        printf("The Hash value of the encoded file without sender and receiver information is : ");
        cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(ori_content.c_str(),strlen(ori_content.c_str())) << endl;
        std::cout << std::dec;
    }

    printf("The WPL of Huffman tree is : %d\n",coder.wpl);
    printf("The last 16 bytes of the file after encoding are : \n");
    cout<<get_last_16(en_content)<<endl;
    printf("The Hash of the file after encoding is : ");
    cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(en_content.c_str(),strlen(en_content.c_str())) << endl;
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
void decode()//解码过程
{
    //记录开始时间节点
    printf("If you need to decrypt the key, enter an integer between 0 and 255, otherwise enter 0 : ");
    int keyyyy; cin>>keyyyy;
    
    auto start_time = std::chrono::high_resolution_clock::now();

    HFM_decoder decoder;
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
    for(auto it:content) check_num=check_num*1331ull+it;
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
    cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(de_content.c_str(),strlen(de_content.c_str())) << endl;
    std::cout << std::dec;
    if(user_flag=="y")
    { 
        printf("The Hash value of the decoded file without sender and receiver information is : ");
        cout<<"0x"<< std::hex << (uint64_t)fnv1a_64(ori_content.c_str(),strlen(ori_content.c_str())) << endl;
        std::cout << std::dec;
    }

    // 输出时间
    printf("The decoding process takes %lld ms\n",(long long)duration_time.count());

}
void init()//初始化文件名称等
{
    for(int i=0;i<=255;i++) en_map[i]=i;
    shuffle(en_map+0,en_map+256,rnd);
    for(int i=0;i<=255;i++) de_map[en_map[i]]=i;
    // for(int i=0;i<=255;i++)
    //     cout<<(int)en_map[de_map[i]]<<' '<<endl;

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
    decode();
    return 0;
}
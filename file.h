#ifndef FILE_H
#define FILE_H

#include<bits/stdc++.h>
using namespace std;
//name表示文件名/路径
string file_read(string name)//读入文件并且以string返回
{
    FILE* file = fopen(name.c_str(), "r");
    // 检查文件是否成功打开
    if (file == nullptr) {
        cerr << "Error: can't open this file to read" << endl;
        exit(0);
    }
    // 移动文件指针到文件末尾以获取文件大小
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);  // 获取文件大小
    rewind(file);  // 将文件指针重置到文件开始位置
    string content;
    content.resize(fileSize);

    // 读取文件内容到字符串中
    size_t bytesRead = fread(&content[0], 1, fileSize, file); // 直接操作 std::string 的内部数据
    content.resize(bytesRead);
    fclose(file);
    return content;
}
//name表示文件名/路径，content表示写入文件的内容
void file_write(string name,string content)
{
    // 打开文件（写入模式，若文件不存在则创建文件）
    FILE* file = fopen(name.c_str(), "w");
    // 检查文件是否成功打开
    if (file == nullptr) {
        cerr << "Error: can't open this file to write" << endl;
        exit(0);
    }

    //写入文件
    fwrite(content.c_str(), sizeof(char), content.size(), file);

    // 关闭文件
    fclose(file);
}
//name表示文件名/路径，返回文件的大小
int file_size(string name)//查看某一个文件的大小
{
    ifstream file(name, std::ios::binary); // 以二进制模式打开文件

    if (!file) {
        cerr << "Error: can't open the" << name <<endl;
        return -1;
    }

    // 移动到文件末尾
    file.seekg(0, std::ios::end);
    // 获取当前文件指针的位置，即文件大小
    streamsize size = file.tellg();
    file.close(); // 关闭文件
    return size;
}

#endif
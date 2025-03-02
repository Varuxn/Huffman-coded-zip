# 基于哈夫曼编码的文本文件压缩解压缩系统

## 项目概述

本系统是一个基于哈夫曼编码的文本文件压缩解压工具，支持加密传输和完整性校验功能。系统包含以下核心特性：
- 基于哈夫曼编码的高效压缩算法
- 三重数据加密保护（异或加密+偏移加密+随机映射）
- FNV-1a 64位哈希校验
- 收发人身份验证机制
- 压缩过程可视化（编码表/压缩率/WPL等指标）
- 支持两种解码实现（树状搜索/哈希表）

![系统架构图](https://raw.githubusercontent.com/Varuxn/pictures-bed/master/img%E7%B3%BB%E7%BB%9F%E8%AE%BE%E8%AE%A1.png)

## 文件结构

```
├── include/
│   ├── hash.h          # FNV哈希算法实现
│   ├── file.h          # 文件读写接口
│   ├── minheap.h       # 最小堆实现
│   ├── out_in_hex.h    # 十六进制转换工具
│   ├── huffman_tree.h  # 哈夫曼树构建与编码
│   ├── decoder_new.h   # 树状解码器（高效实现）
│   └── decoder.h       # 哈希表解码器（STL实现）
├── src/
│   ├── main.cpp        # 主程序（完整流程）
│   ├── encode.cpp      # 独立压缩模块
│   └── decode.cpp      # 独立解压模块
```

## 用户手册

### 编译指南

```bash
g++ -std=c++11 -O3 main.cpp -o huffman_compressor
```

### 使用流程

1. **启动程序**
   ```bash
   ./huffman_compressor
   ```

2. **输入文件**
   ```
   Please input the filename : example.txt
   ```

3. **收发人信息（可选）**
   ```
   Do you want to store sender and receiver information?(y/n)
   y
   Please input the sender ID : 001
   Please input the sender name : Alice
   Please input the receiver ID : 002
   Please input the receiver name : Bob
   ```

4. **加密设置**
   ```
   Do you want to encrypt files ?(y/n)
   y
   If you need key encryption, enter an integer between 0 and 255 : 123
   ```

5. **压缩过程**
   - 自动生成编码表（code.txt）
   - 显示压缩前后哈希校验值
   - 输出压缩率等关键指标

6. **解压过程**
   ```
   If you need to decrypt the key, enter an integer between 0 and 255 : 123
   Please input the receiver ID : 002
   Please input the receiver name : Bob
   ```

### 生成文件说明

- `.hfm`：压缩后的二进制文件
- `_j.txt`：解压恢复的文本文件
- `code.txt`：包含以下信息的编码表：
  ```
  文件原始大小
  字符十六进制 对应编码（十六进制表示）
  ```

### 特性说明

1. **多层加密体系**
   - 异或加密：`content[i] ^= key`
   - 偏移加密：`content[i] += 0x55`
   - 随机映射：使用预生成的置换表

2. **完整性验证**
   - 1331进制校验和
   - 文件首尾16字节对比
   - FNV-1a 64位哈希校验

3. **性能优化**
   - 小根堆构建哈夫曼树（O(n)复杂度）
   - 两种解码实现可选（树形遍历 vs 哈希表）
   - 压缩率实时计算（原始大小/压缩后大小）

4. **身份验证**
   - 收发人ID/姓名双重验证
   - 信息头分离存储
   - 独立哈希校验原始内容

## 技术指标

- 支持文件格式：`.txt`文本文件
- 最大密钥值：255
- 压缩头信息：152字节（含加密参数）
- 时间统计精度：毫秒级

## 注意事项

1. 必须保留`code.txt`用于解码
2. 加密解密需使用相同密钥
3. 接收人信息验证失败将终止解压
4. 文件损坏会触发校验失败警告

## 运行示意截图

![](https://raw.githubusercontent.com/Varuxn/pictures-bed/master/img%E4%BA%A4%E4%BA%921.png)
![](https://raw.githubusercontent.com/Varuxn/pictures-bed/master/img%E4%BA%A4%E4%BA%922.png)
![](https://raw.githubusercontent.com/Varuxn/pictures-bed/master/img%E4%BA%A4%E4%BA%923.png)

## 性能测试

测试文件 | 原始大小 | 压缩后大小 | 压缩率 | 解压时间
--------|---------|-----------|-------|---------
test1.txt | 2.25MB | 1.67MB | 74.19% | 464ms
test2.txt | 1.16B | 814B | 67.95%  | 4ms
test3.txt | 2.34KB | 1.65KB | 70.51%  | 4ms

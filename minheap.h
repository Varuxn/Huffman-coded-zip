#ifndef MINHEAP_H
#define MINHEAP_H

#include<bits/stdc++.h>
using namespace std;
class MinHeap {
private:
    vector< pair<pair<int,unsigned char>,int> > heap; // 用动态数组存储堆

    //pair存储的是<<词频，哈夫曼树节点对应字符单字节的值>,哈夫曼树上的编号>
    //因此排序当碰到词频数量相同的情况会根据单字节的值进行排序

    // 向上调整堆
    void siftUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2; // 父节点索引
            if (heap[index].first < heap[parentIndex].first) {
                swap(heap[index], heap[parentIndex]); // 如果当前节点大于父节点，交换
                index = parentIndex; // 继续向上调整
            } else {
                break; // 如果当前节点小于等于父节点，调整结束
            }
        }
    }

    // 向下调整堆
    void siftDown(int index) {
        int n = heap.size();
        while (true) {
            int leftChild = 2 * index + 1; // 左子节点索引
            int rightChild = 2 * index + 2; // 右子节点索引
            int largest = index; // 假设当前节点是最小值

            // 找到当前节点、左子节点、右子节点中的最小值
            if (leftChild < n && heap[leftChild].first < heap[largest].first) {
                largest = leftChild;
            }
            if (rightChild < n && heap[rightChild].first < heap[largest].first) {
                largest = rightChild;
            }

            // 如果最小值不是当前节点，交换并继续向下调整
            if (largest != index) {
                swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break; // 如果当前节点已经是最小值，调整结束
            }
        }
    }

public:
    // 插入数据
    void push(pair<pair<int,unsigned char>,int> value) {
        heap.push_back(value); // 将新数据插入堆的末尾
        siftUp(heap.size() - 1); // 向上调整堆
    }

    bool empty() {
        return heap.empty();
    }

    // 获取最小值
    pair<pair<int,unsigned char>,int> top() {
        if (heap.empty()) {
            throw out_of_range("Heap is empty"); // 如果堆为空，抛出异常
        }
        return heap[0]; // 返回堆顶元素
    }

    // 删除最小值
    void pop() {
        if (heap.empty()) {
            throw out_of_range("Heap is empty"); // 如果堆为空，抛出异常
        }
        swap(heap[0], heap[heap.size() - 1]); // 将堆顶元素与最后一个元素交换
        heap.pop_back(); // 删除最后一个元素
        siftDown(0); // 从堆顶开始向下调整堆
    }

};

#endif
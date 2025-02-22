#include<bits/stdc++.h>
using namespace std;

void heapify(vector<int> s,int n,int i)
{
    int largest = i; // 初始化最大值为当前节点
    int left = 2 * i + 1; // 左子节点
    int right = 2 * i + 2; // 右子节点

    // 如果左子节点大于当前最大值，则更新最大值
    if (left < n && s[left] > s[largest])
        largest = left;

    // 如果右子节点大于当前最大值，则更新最大值
    if (right < n && s[right] > s[largest])
        largest = right;

    // 如果最大值不是当前节点，则交换并继续调整堆
    if (largest != i) {
        swap(s[i], s[largest]);
        heapify(s, n, largest);
    }
}

vector<int> heap_sort(vector<int> s)
{
    int n=s.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(s ,n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(s[0], s[i]);
        heapify(s, i, 0);
    }
    return s;
}
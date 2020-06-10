#include<iostream>
#include<vector>
#include<time.h>
#include<stdlib.h>
using namespace std;
/* 各种排序算法。
可在leetcode 912进行测试。
leetcode 912: https://leetcode-cn.com/problems/sort-an-array/
*/
void swap(int& a, int& b){
	int temp = a;
	a = b;
	b = temp;
}

// 基本思想：相邻元素两两比较，最大的下沉。
void bubbleSort(vector<int>& sequence){
	bool changed = false;
	for(int i = sequence.size() - 1; i > 0; i--){
		for(int j = 0; j < i; j++){
			if(sequence[j] > sequence[j+1]){
				swap(sequence[j], sequence[j+1]);
				changed = true;
			}
		}
		if(changed == false) break;
	}
}

// 基本思想：抓扑克牌并按大小顺序插在合适的位置。
void insertSort(vector<int>& sequence){
	for(int i = 1; i < sequence.size(); i++){
		int temp = sequence[i];
		int j = i;
		while(j > 0 && sequence[j - 1] > temp){
			sequence[j] = sequence[j - 1];
			j--;
		}
		sequence[j] = temp;
	}
}

/* 四重循环的希尔排序
	实际上过于暴力且冗余："分组"是不必要的。
	设当前增量为i, 只需从索引为i的元素开始遍历到数组末尾。再设本次遍历索引变量为j，
	那么j位置的元素就要与j-1*i, j-2*i, ..., j-m*i位置的元素比较并插入。
*/
void shellSort_lp4(vector<int>& sequence){
	for(int i = sequence.size() / 2; i > 0; i /= 2){	// 增量
		for(int j = 0; j < i; j++){		// "分组"，共i组(i=当前增量数)。实际不必要。
			for(int k = 1; j + k * i < sequence.size(); k++){
				int temp = sequence[j+k*i];
				int m = j + k * i;
				while(m > j && sequence[m-i] > temp){
					sequence[m] = sequence[m-i];
					m -= i;
				}
				sequence[m] = temp;
			}
		}
	}
}

// 反复在leetcode 912写几次，关注边界条件。
void shellSort(vector<int>& sequence){
	for(int i = sequence.size() / 2; i > 0; i /= 2){	// 增量
		for(int j = i; j < sequence.size(); j++){	// 从j=i位置开始遍历，直到数组尾端
			int temp = sequence[j];
			int k = j;		// temp都要与j-i, j-2i, ...位置的元素比较。k始终指向带比较元素的后一个元素。
			while(k >= i && sequence[k-i] > temp){		// k >= i是个容易错的点(边界)
				sequence[k] = sequence[k-i];	// 元素后移
				k -= i;
			}
			sequence[k] = temp;
		}
	}
	/* 可以对比原版插排
	for(int i = 1; i < sequence.size(); i++){
		int temp = sequence[i];
		int j = i;
		while(j > 0 && sequence[j - 1] > temp){		// j始终指向带比较元素的后一个元素
			sequence[j] = sequence[j - 1];
			j--;
		}
		sequence[j] = temp;
	}
	*/
}

void selectSort(vector<int>& sequence){
	for(int i = 0; i < sequence.size(); i++){
		// 从i开始遍历，找到最小元
		int min_pos = i, min = sequence[i];
		for(int j = i + 1; j < sequence.size(); j++){
			if(sequence[j] < min){
				min = sequence[j];
				min_pos = j;
			}
		}
		swap(sequence[i], sequence[min_pos]);
	}
}


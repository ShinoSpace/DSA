#include<iostream>
#include<vector>
using namespace std;

class MergeSort{
	private:
		void swap(int& a, int& b){
			int temp = a;
			a = b;
			b = temp;
		}
		void merge(vector<int>& s, vector<int>& temp,int s1_start, int s2_start, int s2_end);
		void mergeSort(vector<int>& s, vector<int>& temp,int left, int right);	// 递归版本
	public:
		void mergeSort(vector<int>& s);	// API
};

void MergeSort::merge(vector<int>& s, vector<int>& temp, int s1_start, int s2_start, int s2_end){
	/* 目的：将有序的左侧和右侧合并为有序的序列
		整体要干两件事：
		1. 遍历左右子列，合并到临时数组上。
		2. 完全有序的临时数组拷贝回原部分有序数组。
		作为参数，传左右边界比传边界加长度更舒服 */
	// 左侧序列[s1_left, s2_left-1]. 右侧序列[s2_left, s2_left + length - 1]
	int tmp_ptr = s1_start;		// 临时数组指针
	int s1_end = s2_start - 1;	// 左子列边界
	int total_length = s2_end - s1_start + 1;	// 总长度，拷贝时使用
	while(s1_start <= s1_end && s2_start <= s2_end){
		if(s[s1_start] < s[s2_start]){
			temp[tmp_ptr] = s[s1_start];
			tmp_ptr++;
			s1_start++;
		}
		else{
			temp[tmp_ptr] = s[s2_start];
			tmp_ptr++;
			s2_start++;
		}
	}
	// 左右子列至少有一个遍历完成，没遍历完的直接接到临时数组合并完的部分的后面。
	while(s1_start <= s1_end){
		temp[tmp_ptr] = s[s1_start];
		tmp_ptr++;
		s1_start++;
	}
	while(s2_start <= s2_end){
		temp[tmp_ptr] = s[s2_start];
		tmp_ptr++;
		s2_start++;
	}
	// 合并完成，拷贝回原来的数组
	for(int i = 0; i < total_length; i++){
		tmp_ptr--;
		s[tmp_ptr] = temp[tmp_ptr];
	}
}

void MergeSort::mergeSort(vector<int>& s, vector<int>& temp, int left, int right){
	int mid = (left + right) >> 1;
	if(left < right){
		mergeSort(s, temp, left, mid);
		mergeSort(s, temp, mid+1, right);
		merge(s, temp, left, mid+1, right);
	}
}

void MergeSort::mergeSort(vector<int>& s){
	vector<int> temp(s.size());
	if(temp.size() == s.size())
		mergeSort(s, temp, 0, s.size()-1);
	else
		printf("Not enough space");
}

/*
int main(){
	MergeSort ms;
	vector<int> seq({10, 12, 5, 8, 1, 15, 20, 9});
	ms.mergeSort(seq);
	for(int i = 0; i < seq.size(); i++){
		printf("%d ", seq[i]);
	}
	return 0;
}
*/
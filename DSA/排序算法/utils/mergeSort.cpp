#include<iostream>
#include<vector>
using namespace std;

class MergeSort{
	private:
		void merge(vector<int> &s, vector<int> &temp, int l_start, int r_start, int r_end);
		void mergePass(vector<int> &s, vector<int> &temp, int length);
	public:
		void mergeSort(vector<int> &s);		// API
};

void MergeSort::merge(vector<int> &s, vector<int> &temp, int l_start, int r_start, int r_end){
	/* 与递归版本不同的是，这个版本的不会进行反向拷贝回原数组 */
	int l_end = r_start - 1;
	int tptr = l_start;
	while(l_start <= l_end && r_start <= r_end){
		if(s[l_start] < s[r_start]){
			temp[tptr] = s[l_start];
			tptr++;
			l_start++;
		}
		else{
			temp[tptr] = s[r_start];
			tptr++;
			r_start++;
		}
	}
	while(l_start <= l_end){
		temp[tptr] = s[l_start];
		tptr++;
		l_start++;
	}
	while(r_start <= r_end){
		temp[tptr] = s[r_start];
		tptr++;
		l_start++;
	}
	// 不往原数组拷贝了
}

void MergeSort::mergePass(vector<int> &s, vector<int> &temp, int length){
	int i = 0;
	while(i <= s.size() - 2*length){
		merge(s, temp, i, i+length, i+2*length-1);
		i += 2*length;
	}
	// 针对末端情况处理
	if(i + length < s.size()){
		// 末端有左右两个子列，长度为length + left_length, left_length < length
		merge(s, temp, i, i+length, s.size()-1);
	}
	else{
		while(i < s.size()){
			temp[i] = s[i];
			i++;
		}
	}
}

void MergeSort::mergeSort(vector<int> &s){
	vector<int> temp(s.size());
	for(int i = 1; i < s.size(); i *= 2){
		/* 起始每个子列长度为1，一组包含两个子列，长度为2
			终止边界条件是i < s.size() 还是 i <= s.size() / 2?
			简单画一个总长为5的序列看看归并终止的条件 */
		mergePass(s, temp, i);	// 待排序列s向temp归并
		i *= 2;
		mergePass(temp, s, i);	// 反向，temp向s归并
	}
}
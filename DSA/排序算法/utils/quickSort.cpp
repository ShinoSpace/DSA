#include<vector>
#include<iostream>
#include<time.h>
#include<stdlib.h>
using namespace std;

class QuickSort{
	private:
		int median3(vector<int>& sequence, int left, int right);		// 中位数pivot
		int randSelect(vector<int>& sequence, int left, int right);		// 随机选择pivot
		void quickSort(vector<int>& sequence, int left, int right);		// 中位数版
		void randQuickSort(vector<int>& sequence, int left, int right);	// 随机数版
		void swap(int& a, int& b){
			int temp = a;
			a = b;
			b = temp;
		}
	public:
		void quickSort(vector<int>& sequence);	// API
};

int QuickSort::median3(vector<int>& sequence, int left, int right){
	/* 三个数的中位数作为pivot, 做三件事：
		1. 找到首尾和中间位置mid = (left + right) / 2的中位数。
		2. 三个位置的元素升序/降序(视任务需要)排列。
		3. 中位数与right-1位置的元素交换，并返回中位数作为pivot。
		比较+交换找中位数的问题：
		实际上是三个数做比较排序，使最终sequence[left] <= sequence[mid] <= sequence[right]
		因此这个比较排序的顺序不能乱：先是left位置的元素与mid和right位置的元素分别比较和交换，
		使left位置元素最小，然后mid和right再比较和交换，以完成排序任务。
	*/
	int mid = (left + right) >> 1;
	if(sequence[left] > sequence[mid]) swap(sequence[left], sequence[mid]);
	if(sequence[left] > sequence[right]) swap(sequence[left], sequence[right]);
	if(sequence[mid] > sequence[right]) swap(sequence[mid], sequence[right]);
	swap(sequence[mid], sequence[right-1]);
	return sequence[right-1];
}

int QuickSort::randSelect(vector<int>& sequence, int left, int right){
	/* 生成一个范围在[left, right]之间的随机数作为pivot的索引。
		将这个随机数换到当前数组尾部。
	*/
	srand((unsigned)time(0));
	int pivot_idx = left + rand() % (right - left + 1);		// 范围在[left, right]内的随机数
	swap(sequence[pivot_idx], sequence[right]);
	return sequence[right];
}

void QuickSort::quickSort(vector<int>& sequence, int left, int right){
	/* 三件事：
		1. 选主元pivot。
		2. 调整数组，分为两部分，左边比主元小，右边比主元大(通过交换来完成)。
		3. 调整完成后pivot换到合适的位置。
		额外的工作：
		当数组长度较小的时候，用简单排序。一方面可以大幅度降低递归深度节省内存，
		另一方面，递归到较短数组时，较短数组实际上基本有序，用简单排序算法非常快。
		快排是一个非常细致的算法，稍不注意，算法就会非常低效甚至出错。
		碰到的坑：（代码中可能出问题的地方以Qx注释）
		Q1: 选主元pivot的方法有很多种，常用的有中位数和随机选择法。
		中位数法(gcc内部排序的实现方法)：
			一般用首尾+序列中心三个位置的元素查找中位数。实际上是进行三个数的比较排序。
			排序的比较不能乱来，详见median3函数。
		随机选择法：
			TODO!
		Q2.x: 这是非常坑的一个问题！
			如果Q2.1~Q2.4所标注的几句话变为这样：
			low = left + 1;									// Q2.1m
			high = right - 2;							  // Q2.2m
			while(sequence[low] < pivot) low++;		// Q2.3m
			while(sequence[high] > pivot) high--;	// Q2.4m
			逻辑上似乎没什么问题，但实际上有两个严重的缺陷：
			1. Q2.3m和Q2.4m所在的外层循环体完整执行一次后，
				low和high位置的元素已经交换到了他们合适的位置。
				下一次循环中，Q2.3和Q2.4进行的第一次比较是多余的。
				(因为此时low和high位置的元素必然分别小于和大于主元pivot)。
			2. 如果仅仅是一次比较的多余似乎不会造成问题，但考虑待排序列：
				{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
				显然主元是1。如果按Q2.3m和Q2.4m的做法，low和high位置的元素始终等于pivot，
				Q2.3m和Q2.4m的循环永远不会执行！这样low和high始终原地踏步，
				外层循环终止条件Q2.5永远达不到。死循环！
			综上所述，警惕处理和判断逻辑重叠导致的多余判断，很有可能造成极端输入情况下程序崩溃或是bug。
			一个头一个尾，这是典型的双指针对撞法。
			实际上还有另一种思路，一个指针i从left+1扫到right-2，
			另一个指针j始终指向小于pivot的子序列的尾部。扫的时候把小的往前放，大的不管。
			这样小于pivot的都跑到左边了，大于pivot的自然都在右边，
			最后交换right-1和j位置的元素交换即可。
		Q3: 这种median3的方法必须使用cutoff，且cutoff必须大于2。
			原因是：如果只有两个元素待排，此情况下快排的high指针在Q2.4操作时会越界！
		综上，算法的边界情况：
			1. 数组中所有元素都是一个数。
			2. 只有2个元素的数组。
	*/
	int cutoff = 200, pivot, low, high;		// Q3
	if(right - left + 1 > cutoff){
		// 规模够大，快排
		pivot = median3(sequence, left, right);		// Q1
		low = left;		//Q2.1
		high = right - 1;	//Q2.2
		while(true){
			while(sequence[++low] < pivot);		// Q2.3
			while(sequence[--high] > pivot);	// Q2.4
			if(low < high) swap(sequence[low], sequence[high]);		// Q2.5
			else break;
		}
		swap(sequence[right-1], sequence[low]);
		quickSort(sequence, left, low - 1);
		quickSort(sequence, low + 1, right);
	}
	else{
		// 规模较小，简单排序。这里用直接插排
		for(int i = left + 1; i <= right; i++){
			int j = i;
			int temp = sequence[i];
			while(j > 0 && sequence[j - 1] > temp){
				sequence[j] = sequence[j-1];
				j--;
			}
			sequence[j] = temp;
		}
	}
}

void QuickSort::randQuickSort(vector<int>& sequence, int left, int right){
	/* 为了保持代码规范，需要对median3版本的双指针对撞逻辑小心地修改。
		这里又有容易出问题的地方：
		Q1: 分析序列{5, 1, 1, 2, 0, 0}的排序, 第一次主元选2。
	*/
	int cutoff, pivot, low, high;
	cutoff = 200;
	if(right - left + 1 > cutoff){		// 待排序列够长，快排！
		pivot = randSelect(sequence, left, right);		// 执行后，主元pivot位于当前序列尾部。
		low = left;
		high = right - 1;
		while(true){
			while(low <= high && sequence[low] < pivot)	// Q1: 这又有个坑，low<=high而不能low<high
				low++;
			while(low < high && sequence[high] > pivot)
				high--;
			if(low < high){
				swap(sequence[low], sequence[high]);
				/* 下一次循环不必再比较low和high位置的元素与pivot的大小。但其实这是次要的！
				最重要的是，在诸如{1,1,1,1,1}这样的特殊情况下，这个逻辑使low和high能够正常移动，
				避免二者原地踏步，永远达不到跳出循环的条件而彻底变为死循环。
				*/
				low++; 
				high--;
			}
			else
				break;
		}
		// 当交换好的时候[0, low)是小于pivot的部分，[high, right-1]是大于pivot的部分
		swap(sequence[right], sequence[low]);
		randQuickSort(sequence, left, low - 1);
		randQuickSort(sequence, low + 1, right);
	}
	else{
		// 规模较小，简单排序。这里用直接插排
		for(int i = left + 1; i <= right; i++){
			int j = i;
			int temp = sequence[i];
			while(j > 0 && sequence[j - 1] > temp){
				sequence[j] = sequence[j-1];
				j--;
			}
			sequence[j] = temp;
		}
	}		
}

void QuickSort::quickSort(vector<int>& sequence){
	quickSort(sequence, 0, sequence.size() - 1);
}
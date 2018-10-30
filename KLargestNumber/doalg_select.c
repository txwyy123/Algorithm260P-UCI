#include <stdlib.h>
#include <math.h>
#define MAX_S 30

void swap(int x, int y, int *a){
	int tmp = a[x];
	a[x] = a[y];
	a[y] = tmp;
}

void sample(int num_s, int l, int r, int *a, int *b){
	int s = r-l+1;
	for (int i=0; i<num_s; i++){
		b[i] = l + (i+1)*s/num_s - 1;
	}
	for(int i=1;i<num_s;i++){
		if(COMPARE(a[b[0]], a[b[i]]) == 2){
			swap(i, 0, b);
		}
	}
}

int partition(int l, int r, int *index_list){
	int i = l-1;
	int num_s =  2+(r-i)/769; //(log(r-i)/log(2));
	if(r-i < 3){
		for(int j=l; j<r; j++){
			if(COMPARE(index_list[j], index_list[r]) == 1){
				i++;
				swap(i, j, index_list);
			}
		}
		swap(i+1, r, index_list);
		return i+1;
	}else{
		int b[MAX_S];
		sample(num_s, l, r, index_list, b);
		swap(b[0], r, index_list);
		for(int j=l; j<r; j++){
			int flag = 1;
			for(int p=0; p<num_s-1; p++){
				if(j==b[p]){
					flag = 0;
					break;
				}
			}
			if(flag == 1 && COMPARE(index_list[j], index_list[r]) == 1){
				i++;
				swap(i, j, index_list);
			}
		}
		swap(i+1, r, index_list);
		return i+1;
	}
	
}

int random_partition(int l, int r, int *index_list){
	int i = l-1;
	swap((l+r)/2, r, index_list);
	for(int j=l; j<r; j++){
		if(COMPARE(index_list[j], index_list[r]) == 1){
			i++;
			swap(i, j, index_list);
		}
	}
	swap(i+1, r, index_list);
	return i+1;
}

void merge(int l, int m, int r, int *array){
    int i = l;
    int j = m+1;
    int tmp[r-l+1];
    int k = 0;
    while(i <= m && j <= r)
        tmp[k++] = (COMPARE(array[i], array[j]) == 1) ? array[i++] : array[j++];
    while(i <= m)
        tmp[k++] = array[i++];
    while(j <= r)
        tmp[k++] = array[j++];
    for(i = l, k = 0; i <= r; k++)
        array[i++] = tmp[k];
}

void mergeSort(int l, int r, int *array){
    if(r-l < 1)
        return;

    int m = (l+r)/2;
    mergeSort(l, m, array);
    mergeSort(m+1, r, array);
    merge(l, m, r, array);
}

int select(int k, int l, int r, int *index_list, int *Best){
	int p = partition(l, r, index_list);
	if(p == k-1){
		for(int j=l; j<=p; j++){
			Best[j] = index_list[j];
		}
		return p;
	} else if(p > k-1){
		return select(k, l, p-1, index_list, Best);
	} else {
		for(int j=l; j<=p; j++){
			Best[j] = index_list[j];
		}
		return select(k, p+1, r, index_list, Best);
	}
}

void quicksort(int l, int r, int *a){
	if(l < r){
		int q = random_partition(l, r, a);
		quicksort(l, q-1, a);
		quicksort(q+1, r, a);
	}
}

int doalg(int n, int k, int *Best){
	int index_list[n];
	for(int i = 0; i < n; i++) index_list[i] = i+1;
	int p = select(k, 0, n-1, index_list, Best);
	mergeSort(0, k-1, Best);
	if(p==k-1){
		return 1;
	}
	return 0;
}
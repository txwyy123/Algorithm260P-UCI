#include <math.h>
#include <stdlib.h>
int max_tree[13][3334];
int flag1[3334];

void swap(int x, int y, int *a){
	int tmp = a[x];
	a[x] = a[y];
	a[y] = tmp;
}

int three_max(int x, int y, int z, int *a){
	int largest = x;
	int f = rand()%2;
	if(f==1){
		swap(z,y,a);
	}
	int x1 = 0;
	int x2 = 0;
	if(COMPARE(a[y], a[largest])==1){
		largest=y;
		x1=1;
	}
	if(COMPARE(a[z], a[largest])==1){
		largest=z;
		x2=1;
	}
	if(x1==1 && x2==1){
		flag1[x/3] = 1;
	}
	if(x1==0 && x2==1){
		flag1[x/3] = 2;
	}
	return largest;
}

int two_max(int x, int y, int *a){
	int largest = x;
	if(COMPARE(a[y], a[largest])==1){
		largest=y;
	}
	return largest;
}

void build_tree(int *a, int n){
	// the first layer made of triples
	for(int i=0;i<n;i+=3){
		int tmp = i;
		if(i+2<n){
			tmp = three_max(i, i+1, i+2, a);
		} else if(i+1<n){
			tmp = two_max(i, i+1, a);
		}
		if(tmp != i){
			swap(i, tmp, a);
		}
		max_tree[0][i/3] = a[i];	
	}
	
	// the second layer to the last layer
	int m = 3334;
	for(int i=0;i<=11;i++){
		for(int j=0; j<m; j+=2){
			if(j+1<m){
				max_tree[i+1][j/2] = (COMPARE(max_tree[i][j], max_tree[i][j+1])==1) ? max_tree[i][j]:max_tree[i][j+1];
			} else{
				max_tree[i+1][j/2] = max_tree[i][j];
			}
		}
		m = (m%2==0)?m/2:m/2+1;
	}
}

void modify_tree(int top, int *index_list){
	// the first layer made of triples
	int x = 3*((top-1)/3);
	index_list[x] = -1;
	if(x+1>=10000){
		max_tree[0][x/3] = -1;
	} else if(index_list[x+1] == -1){
		if(index_list[x+2] == -1){
			max_tree[0][x/3] = -1;
		} else{
			max_tree[0][x/3] = index_list[x+2];
			swap(x,x+2,index_list);
		}
	} else if(index_list[x+2] == -1){
		max_tree[0][x/3] = index_list[x+1];
		swap(x,x+1,index_list);
	} else{
		if(flag1[x/3]==1){
			max_tree[0][x/3] = index_list[x+1];
			swap(x, x+1, index_list);
		}else if(flag1[x/3]==2){
			max_tree[0][x/3] = index_list[x+2];
			swap(x, x+2, index_list);
		}else{
			int largest = two_max(x+1, x+2, index_list);
			max_tree[0][x/3] = index_list[largest];
			swap(x, largest, index_list);
		}
	}

	// the second layer to the last layer
	int p = x/3;
	int m = 3334;
	for(int i=0;i<=11;i++){
		int left = 2*(p/2);
		int right = left+1;
		p = left/2;
		if(right >= m){
			max_tree[i+1][p] = (max_tree[i][left]==-1)?-1:max_tree[i][left];
		} else if(max_tree[i][left]==-1){
			max_tree[i+1][p] = (max_tree[i][right]==-1)?-1:max_tree[i][right];
		} else if(max_tree[i][right]==-1){
			max_tree[i+1][p] = max_tree[i][left];
		} else{
			max_tree[i+1][p] = (COMPARE(max_tree[i][left], max_tree[i][right])==1) ? max_tree[i][left]:max_tree[i][right];
		}
		m = (m%2==0)?m/2:m/2+1;
	}
}

int doalg(int n, int k, int *Best){
	int index_list[n];
	for(int i = 0; i < n; i++)
		index_list[i] = i+1;
	for(int i=0;i<3334;i++)
		flag1[i] = 0;
	// buid the tree
	build_tree(index_list, n);
	// fetch 40 largest number's from the tree
	for(int i = 0; i < k-1; i++){
		Best[i] = max_tree[12][0];
		modify_tree(Best[i],index_list);
	}
	Best[k-1] = max_tree[12][0];
	return 1;
}

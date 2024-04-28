#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
using namespace std;

vector<int> arr;

void selection_sort() {
    int len = arr.size();
    int i, j, temp, least;

    for(i=0; i<len; i++) {
        least = i;
        for(j = i+1; j<len; j++) {
            if(arr[j]<arr[least]) least = j;
        }
        temp = arr[i];
        arr[i] = arr[least];
        arr[least] = temp;
    }
}


int partition(int p, int r) {
    int x = arr[r];
    int i = p-1, j, temp;

    for (j=p; j<=r-1; j++) {
        if(arr[j]<=x) {
            i = i+1;
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    temp = arr[i+1];
    arr[i+1] = arr[r];
    arr[r] = temp;

    return i+1;   
}


void quick_sort(int p, int r) {
    if(p<r) {
        int q = partition(p, r);
        quick_sort(p, q-1);
        quick_sort(q+1, r);
    }
}


void merge(int left, int mid, int right) {

    int l_len = mid - left + 1; 
    int r_len = right - mid;
    
    int i=0;
    int j=0;
    int k=left;
    
    vector<int> L_arr(l_len);
    vector<int> R_arr(r_len);

    for(int i=0; i<l_len; i++) {
        L_arr[i] = arr[i+left];
    }
    for(int i=0; i<r_len; i++) {
        R_arr[i] = arr[i+mid+1];
    }
    
    while(i<l_len && j<r_len) {
        if (L_arr[i] < R_arr[j]) arr[k++] = L_arr[i++];
        else arr[k++] = R_arr[j++];
    }

    while(i<l_len) arr[k++] = L_arr[i++];
    while(j<r_len) arr[k++] = R_arr[j++];
}


void merge_sort(int l, int r) {

    if (l>=r) return;

    int m = (l+r)/2;
    merge_sort(l, m);
    merge_sort(m+1, r);
    merge(l, m, r);
}



void my_sort(int p, int r) {
    if (p>=r) return ;
   
    
    if (r - p + 1 <= 15){
        for (int i = p + 1; i <= r; i++){
            int k = arr[i];
            int j = i - 1;
            while(j >= p && arr[j] > k){
                arr[j + 1] = arr[j];
                j = j - 1;
            }
        arr[j + 1] = k;
        }
    }

    else{
        int x = arr[(p + r) / 2];
        int i = p-1, j, temp;
    
        temp = arr[r];
        arr[r] = arr[(p + r) / 2];
        arr[(p + r) / 2] = temp;

        for (j=p ; j<r; j++){
            if (arr[j]<=x){
                i = i+1;
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        temp = arr[i+1];
        arr[i] = arr[r];
        arr[r] = temp;

        my_sort(p, i - 1);
        my_sort(i + 1, r);
    }

}
   


int main(int argc, char*argv[]) {

    char* file_name = argv[1];
    FILE *fp=fopen(file_name, "r");

    int len, n;
    fscanf(fp, "%d", &len);

    for(int i=0; i<len; i++){
      fscanf(fp, "%d", &n);
      arr.push_back(n);
    }


    clock_t st, fin;
    int al_num = (int)argv[2][0] - 48;
    st = clock();
    if(al_num==1) selection_sort();
    else if(al_num==2) quick_sort(0, len-1);
    else if(al_num==3) merge_sort(0, len-1);
    else if(al_num==4) my_sort(0, len-1);
    fin = clock();


    char res[24] = "result_";
    res[7] = argv[2][0];
    res[8] = '_';
    double time = fin-st;
    for(int i=9; i<24; i++) res[i] = file_name[i-9];


    fp = fopen(res, "w");
    fprintf(fp, "%s\n", file_name);
    fprintf(fp, "%d\n", al_num);
    fprintf(fp, "%d\n", len);
    fprintf(fp, "%.6f\n", time/CLOCKS_PER_SEC);


    int i = 0;
    while(i<len) {
       fprintf(fp, "%d ", arr[i]);
       i++; 
    }

    fclose(fp);
    return 0;
    

}

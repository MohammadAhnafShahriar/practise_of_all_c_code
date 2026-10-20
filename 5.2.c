
#include<stdio.h>
void swap(int *x,int *y){
    int temp;
    temp=*x;
    *x=*y;
    *y=temp;

}

int main (){
    int *left,*right;
    int arr[9]= {34,23,3,42,1,2,7,9,87};
    printf("the previous array is   ");
    for(int i = 0;i<9;i++){
        printf(" %d  ",*(arr+i));
    }
    left = arr;
    right = (arr+ 8);
    while(left < right){
    swap(left,right);
    left = left +1;
    right = right -1;
    }
    for(int i = 0;i<9;i++){
    printf(" %d  ",*(arr+i));
    }
    
    return 0;
}
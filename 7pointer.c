#include<stdio.h>
#include<stdlib.h>
int main(){
    int N;
    printf("enter the number of elements\n");
    scanf("%d",&N);
    int *arr=(int *)malloc(N*sizeof(int));
    if(arr == NULL){
        printf("Memory Allocation Failed !");
        return 1;

    }
    printf("give the inputs here\n");
    for(int i=0;i<N;i++){
        scanf("%d",(arr+i));
    }
    printf("here are the inputs  ");
    for(int j=0;j<N;j++){
        printf("%d  ",*(arr+j));
    }
    free(arr);
    arr=NULL;
    return 0;
}
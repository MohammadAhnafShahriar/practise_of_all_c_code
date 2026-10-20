
#include<stdio.h>


int main (){
    int arr[]= {34,23,34,3,42,1,2,7,9,87};
    int i,sum=0;
    for (i=0;i<9;i++){
        sum=sum+*(arr + i);
    }
    printf("the summation is %d  \n",sum);
    
    
    return 0;
}
#include<stdio.h>
void swap(int *x,int *y){
    int temp;
    temp=*x;
    *x=*y;
    *y=temp;

}
int main (){
    int a=7,b=9;
    
    
    printf("the values are %d  %d\n",a,b);
    swap(&a,&b);
    printf(" after swap \n the values are %d  %d\n",a,b);
    
    return 0;
}
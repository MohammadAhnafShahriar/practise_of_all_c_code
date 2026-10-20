
#include<stdio.h>
void get_min_max(int a, int b, int *min, int *max) {
    if (a>b){
         *max = a;
         *min = b;
    }
    else{
         *max = b;
         *min = a;
    }
}

int main (){
    int k=7,m=9;
    int low , high;

    int *max,*min;
    max= &high;
    min = &low;
    get_min_max(k,m,min,max);
    printf("the values are %d  %d\n",k,m);
    
    printf(" the highest\n %d",high);
    printf(" the lowest\n %d",low);
    return 0;
}
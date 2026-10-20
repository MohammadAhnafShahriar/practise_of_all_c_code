#include<stdio.h>


int main(){
    int m,n,i;
    printf("write the number ");
    scanf("%d",&n);
    printf(" the table is here \n");
    for(i=1;i<=10;i++){
        m=n*i;
      printf(" %d * %d  = %d\n",n,i,m);
      
    
}
printf(" the value has %d digits",m);
    return 0;
}
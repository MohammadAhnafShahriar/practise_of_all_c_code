#include<stdio.h>


int main(){
    int m=0,n,i;
    printf("write the number ");
    scanf("%d",&n);
    for(i=1;1;i++){
      n=n/10;
      m++
      if(n/10==0){
        break;
      }
    
}
printf(" the value has %d digits",m+1);
    return 0;
}
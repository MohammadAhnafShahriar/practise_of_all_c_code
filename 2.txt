#include<stdio.h>


int main(){
    int m,n,i;
    printf("write the limit ");
    scanf("%d",&n);
    for(i=1;i<=n;i++){
    printf("write the value ");
    scanf("%d",&m);
  if(m%2==0){
    printf("even");
  }
  
  else{
    printf("odd");
  }
}
    return 0;
}
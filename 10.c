#include<stdio.h>


int main(){
    int m=0,n,i,j;
    printf("write the number of rows ");
    scanf("%d",&n);
    
    for(i=n;i>0;i--){
        for(j=1;j<=(n-i+1);j++){
            if((i+j)%2==0){
                printf("1");
            }
            else{
                printf("0");
            }
        }


        printf("\n");
}

    return 0;
}
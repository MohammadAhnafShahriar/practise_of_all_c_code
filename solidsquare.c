#include<stdio.h>


int main(){
    int m=0,n,i,j,k;
    printf("write the number of rows ");
    scanf("%d",&n);
    
    for(i=n;i>0;i--){
            if(i==1 || i==n){
                for(k=1;k<=n;k++){
            
            
            printf("*");
            }
        }
            
        for(j=1;j<=n;j++){
            if(j==1 || j==n){
            printf("*");
            }
            else{
                printf(" ");
            }
        }


        printf("\n");
}

    return 0;
}
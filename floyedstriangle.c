#include<stdio.h>


int main(){
    int m=0,n,i,j;
    printf("write the number of rows ");
    scanf("%d",&n);
    
    for(i=n;i>0;i--){
        for(j=1;j<=(n-i+1);j++){
            m=m+1;
            printf("%d  ",m);
        }


        printf("\n");
}

    return 0;
}
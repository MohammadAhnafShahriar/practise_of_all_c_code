#include<stdio.h>


int main(){
    int m,n,i,j,k,l,p,q;
    printf("write the number of rows ");
    scanf("%d",&n);
    
    for(i=1;i<=n;i++){
        for(k=1;k<=(n-i);k++){
            printf(" ");
        }




        for(j=1;j<=(2*i-1);j++){
            printf("*");
            
        }



        printf("\n");
}
            for(l = n - 1; l >= 1; l--){

                    for(q = n - 1; q>0 ; q-- ){
                        printf(" ");

                    for(p=(2*l-1);p>0;p--){
                         printf("*");
                    }

                printf("\n");
            }
        }
    return 0;
}
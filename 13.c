#include<stdio.h>
#include<stdlib.h>

int main(){
    int M,N ,k,count=1;
    printf("Enter the number of rows");
    scanf("%d",&N);
    
    printf("Enter the number of columns");
    scanf("%d",&M);
    int **matrix=(int**)malloc(N*sizeof(int *));
    for(int i =0;i<N;i++){
         *(matrix +i)=(int*)malloc(M*sizeof(int));
    }
   for( k=0;k<N;k++){
    printf("\n Enter the values of %d row \n",k+1);
    for(int j=0;j<M;j++){
        scanf("%d",(*(matrix+k)+j)) ;
        
    }
}
   
   for(int k=0;k<N;k++){
    for(int j=0;j<M;j++){
        printf("%d  ",*(*(matrix+k)+j));
    }
    printf("\n");
}   
    for(int i =0;i<N;i++){
       free (*(matrix+i));
    }
    free(matrix);
    matrix=NULL;
    return 0;
}
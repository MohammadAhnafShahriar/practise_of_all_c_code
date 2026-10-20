#include<stdio.h>
#include<stdlib.h>

int main(){
    int count=1;
    int **matrix=(int**)malloc(3*sizeof(int *));
    for(int i =0;i<3;i++){
         *(matrix +i)=(int*)malloc(3*sizeof(int));
    }
   for(int k=0;k<3;k++){
    for(int j=0;j<3;j++){
        *(*(matrix+k)+j) = count;
        count++;
    }
}
   
   for(int k=0;k<3;k++){
    for(int j=0;j<3;j++){
        printf("%d  ",*(*(matrix+k)+j));
    }
    printf("\n");
}   
    for(int i =0;i<3;i++){
       free (*(matrix+i));
    }
    free(matrix);
    matrix=NULL;
    return 0;
}
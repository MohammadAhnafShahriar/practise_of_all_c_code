#include<stdio.h>
#include<stdlib.h>

int main(){
   
    char **words=(char**)malloc(3*sizeof(char *));
    for(int i =0;i<3;i++){
         *(words +i)=(char*)malloc(50*sizeof(char));
    }
  
    for(int k=0;k<3;k++){
        scanf("%s", *(words+k));
        
    }
printf("\n oigulai abar repeat kortese\n");
   
   for(int k=0;k<3;k++){
    
        printf("%s  ",*(words+k));
    
    printf("\n");
}   
    for(int i =0;i<3;i++){
       free (*(words+i));
    }
    free(words);
    words=NULL;
    return 0;
}
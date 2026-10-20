
#include<stdio.h>

int my_strlen(const char *str){
    int count=0;
    while(*str !='\0'){
        count++;
        str++;
    }
    return count;
}
int main (){
    char *gunbo;
    int wordcount;
   char name[]="janinabhai aisob";
    gunbo=name;
    wordcount = my_strlen(gunbo);
    printf(" the word count is %d ",wordcount);
    
    return 0;
}
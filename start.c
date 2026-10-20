#include<stdio.h>
float gpa(int a){
    if(a>=80)   return 4.00;
    else if(a>=75)  return 3.75;
    else if(a>=70)  return 3.50;
    else if(a>=65)  return 3.25;
    else if(a>=60)  return 3.00;
    else if(a>=55)  return 2.75;
    else if(a>=50)  return 2.50;
    else if(a>=45)  return 2.25;
    else if(a>=40)  return 2.00;
    else   return 0.00;
}
int main(){
    int m;
    float result;
    printf("enter the number :- 4");

    scanf("%d",&m);
    result= gpa(m);
    printf("%f",result);
    return 0;
}
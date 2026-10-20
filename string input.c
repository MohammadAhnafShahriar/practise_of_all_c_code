#include <stdio.h>

int main() {
    char str[50];

    // 1. Warn the user (Good User Experience)
    printf("Enter a string (MAXIMUM 49 characters): ");
    
    // 2. Enforce the rule in code (Good Security)
    scanf("%49s", str); 

    printf("You entered: %s\n", str);
    return 0;
}
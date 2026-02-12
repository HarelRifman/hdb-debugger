#include <stdio.h>

void function_to_break() {
    printf("Inside function_to_break!\n");
    fflush(stdout);
    printf("You hit the breakpoint successfully!\n");
    fflush(stdout);
}

int main() {
    printf("Program starting...\n");
    fflush(stdout);
    
    for (int i = 0; i < 3; i++) {
        printf("Loop iteration: %d\n", i);
        fflush(stdout);    
        function_to_break();
    }
    
    printf("Program ending...\n");
    fflush(stdout);
    return 0;
}

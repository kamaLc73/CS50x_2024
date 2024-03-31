#include <stdio.h>
#include <cs50.h>

int main(void) {
    int height = get_int("Height : ");
    while(height<=0){
        height = get_int("Height: ");
    }
    for (int row = 1; row <= height; row++) {
        for (int space = 0; space < height - row; space++) {
            printf(" ");
        }
        for (int hash = 0; hash < row; hash++) {
            printf("#");
        }
        printf("\n");
    }
    return 0;
}

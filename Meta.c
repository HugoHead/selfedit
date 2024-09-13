#include <stdio.h>
#include <stdlib.h>

// Function that will be modified
int some_function() {
    return 0; // This is the part that will be modified
}

// Check the function result
int test_code() {
    int result = some_function();
    if (result == 0) {
        printf("Success: Code modification worked!\n");
        return 0;
    } else {
        printf("Failure: Code modification failed.\n");
        return 1;
    }
}

// Modify the source code and recompile
void modify_self() {
    FILE *fp = fopen("Meta.c", "r+");
    if (fp == NULL) {
        printf("Error: Couldn't open file for modification.\n");
        exit(1);
    }

    // Read through the file and find the function to modify
    char line[256];
    long pos;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "return 0;")) {
            pos = ftell(fp);  // Get the current position in the file
            fseek(fp, pos - strlen("return 0;\n"), SEEK_SET);  // Move the file pointer to the 'return 0;'
            fputs("    return 1;\n", fp);  // Replace with 'return 1;'
            break;
        } else if (strstr(line, "return 1;")) {
            pos = ftell(fp);
            fseek(fp, pos - strlen("return 1;\n"), SEEK_SET);
            fputs("    return 0;\n", fp);  // Replace with 'return 0;'
            break;
        }
    }

    fclose(fp);

    // Recompile the program after modifying
    printf("Recompiling...\n");
    system("gcc -o meta Meta.c");

    //printf("Restarting the modified program...\n");
    //system("./meta");  // Run the modified version
}

int main() {
    // Run the test
    int exit_code = test_code();

    if (exit_code == 0) {
        printf("No modification needed, terminating.\n");
        return 0;
    }

    // If the test fails, modify the code
    printf("Modifying the code...\n");
    modify_self();

    return 0;
}

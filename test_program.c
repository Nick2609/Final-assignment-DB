#include "storage_mgr.h"
#include <stdio.h>

int main() {
    // Test createPageFile function
    RC rc = createPageFile("testfile.bin");  // Create a new file
    if (rc == RC_OK) {
        printf("File created successfully!\n");
    } else {
        printf("Failed to create file. Error code: %d\n", rc);
    }

    return 0;  // Indicate the program ran successfully
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"
char *testName;  


#define TESTPF "test_pagefile.bin"

static void testCreateOpenClose(void);
static void testSinglePageContent(void);

int main(void) {
    testName = "";

    initStorageSystem(); 

    testCreateOpenClose();
    testSinglePageContent();

    return 0;
}

void testCreateOpenClose(void) {
    SM_FileHandle fh;
    testName = "Test Create, Open, and Close File";

    TEST_CHECK(createStorageFile(TESTPF));  

    TEST_CHECK(loadStorageFile(TESTPF, &fh));
    ASSERT_TRUE(strcmp(fh.fileName, TESTPF) == 0, "File name matches");
    ASSERT_TRUE(fh.totalNumPages == 1, "Expect 1 page in new file");
    ASSERT_TRUE(fh.currentPageIndex == 0, "New file's current page should be 0");

    TEST_CHECK(closeStorageFile(&fh)); 
    TEST_CHECK(removeStorageFile(TESTPF));  

    ASSERT_TRUE(loadStorageFile(TESTPF, &fh) != RC_OK, "Opening non-existing file should return error");

    TEST_DONE();
}

void testSinglePageContent(void) {
    SM_FileHandle fh;
    SM_PageHandle ph = (SM_PageHandle)malloc(PAGE_SIZE);
    int i;

    testName = "Test Single Page Content";

    TEST_CHECK(createStorageFile(TESTPF));
    TEST_CHECK(loadStorageFile(TESTPF, &fh));

    printf("Created and opened file successfully.\n");

    TEST_CHECK(fetchFirstPage(&fh, ph));  

    for (i = 0; i < PAGE_SIZE; i++)
        ASSERT_TRUE(ph[i] == 0, "First page should be empty");

    printf("Verified first block is empty.\n");

    for (i = 0; i < PAGE_SIZE; i++)
        ph[i] = (i % 10) + '0';

    TEST_CHECK(storePage(&fh, 0, ph));  
    printf("Wrote to first block successfully.\n");

    TEST_CHECK(fetchFirstPage(&fh, ph));
    for (i = 0; i < PAGE_SIZE; i++)
        ASSERT_TRUE(ph[i] == (i % 10) + '0', "Data read matches expected content");

    printf("Read first block successfully.\n");

    TEST_CHECK(closeStorageFile(&fh));
    TEST_CHECK(removeStorageFile(TESTPF));

    free(ph);

    TEST_DONE();
}

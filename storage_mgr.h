#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

#include "dberror.h"

typedef struct SM_FileHandle {
    char *fileName;
    int totalNumPages;
    int currentPageIndex;
    void *filePointer;
} SM_FileHandle;

typedef char* SM_PageHandle;

/* 🔹 Core Storage Operations */
void initStorageSystem(void);
RC createStorageFile(const char *path);
RC loadStorageFile(const char *path, SM_FileHandle *handle);
RC closeStorageFile(SM_FileHandle *handle);
RC removeStorageFile(const char *path);

/* 🔹 Page Read Operations */
RC fetchPage(int index, SM_FileHandle *handle, SM_PageHandle buffer);
int getCurrentPage(SM_FileHandle *handle);
RC fetchFirstPage(SM_FileHandle *handle, SM_PageHandle buffer);
RC fetchPreviousPage(SM_FileHandle *handle, SM_PageHandle buffer);
RC fetchCurrentPage(SM_FileHandle *handle, SM_PageHandle buffer);
RC fetchNextPage(SM_FileHandle *handle, SM_PageHandle buffer);
RC fetchLastPage(SM_FileHandle *handle, SM_PageHandle buffer);

/* 🔹 Page Write Operations */
RC storePage(SM_FileHandle *handle, int index, SM_PageHandle buffer);
RC storeCurrentPage(SM_FileHandle *handle, SM_PageHandle buffer);
RC appendBlankPage(SM_FileHandle *handle);
RC ensureStorageCapacity(int numPages, SM_FileHandle *handle);

#endif

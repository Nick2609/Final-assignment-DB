#include "storage_mgr.h"
#include "dberror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4096

FILE *getFileHandle(const char *path, const char *mode) {
    return fopen(path, mode);
}

void initStorageSystem(void) {}

RC createStorageFile(const char *path) {
    FILE *fp = getFileHandle(path, "wb+");
    if (!fp) return RC_FILE_NOT_FOUND;

    void *buffer = calloc(1, PAGE_SIZE);
    if (!buffer) {
        fclose(fp);
        return RC_MEMORY_ALLOCATION_FAILED;
    }

    size_t written = fwrite(buffer, PAGE_SIZE, 1, fp);
    free(buffer);
    fclose(fp);

    return written < 1 ? RC_WRITE_FAILED : RC_OK;
}

RC loadStorageFile(const char *path, SM_FileHandle *handle) {
    FILE *fp = getFileHandle(path, "rb+");
    if (!fp) return RC_FILE_NOT_FOUND;

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    handle->fileName = strdup(path);
    handle->totalNumPages = fileSize / PAGE_SIZE;
    handle->currentPageIndex = 0;
    handle->filePointer = fp;

    return RC_OK;
}

RC closeStorageFile(SM_FileHandle *handle) {
    if (!handle || !handle->filePointer) return RC_FILE_NOT_FOUND;
    fclose((FILE *)handle->filePointer);
    handle->filePointer = NULL;
    return RC_OK;
}

RC removeStorageFile(const char *path) {
    return remove(path) == 0 ? RC_OK : RC_FILE_NOT_FOUND;
}

RC fetchPage(int index, SM_FileHandle *handle, SM_PageHandle buffer) {
    FILE *fp = (FILE *)handle->filePointer;
    if (!fp || index < 0 || index >= handle->totalNumPages) return RC_READ_NON_EXISTING_PAGE;

    fseek(fp, (long)index * PAGE_SIZE, SEEK_SET);
    return fread(buffer, 1, PAGE_SIZE, fp) != PAGE_SIZE ? RC_READ_NON_EXISTING_PAGE : (handle->currentPageIndex = index, RC_OK);
}

int getCurrentPage(SM_FileHandle *handle) {
    return handle->currentPageIndex;
}

RC fetchFirstPage(SM_FileHandle *handle, SM_PageHandle buffer) {
    return fetchPage(0, handle, buffer);
}

RC fetchPreviousPage(SM_FileHandle *handle, SM_PageHandle buffer) {
    return handle->currentPageIndex > 0 ? fetchPage(handle->currentPageIndex - 1, handle, buffer) : RC_READ_NON_EXISTING_PAGE;
}

RC fetchCurrentPage(SM_FileHandle *handle, SM_PageHandle buffer) {
    return fetchPage(handle->currentPageIndex, handle, buffer);
}

RC fetchNextPage(SM_FileHandle *handle, SM_PageHandle buffer) {
    return handle->currentPageIndex + 1 < handle->totalNumPages ? fetchPage(handle->currentPageIndex + 1, handle, buffer) : RC_READ_NON_EXISTING_PAGE;
}

RC fetchLastPage(SM_FileHandle *handle, SM_PageHandle buffer) {
    return fetchPage(handle->totalNumPages - 1, handle, buffer);
}

RC storePage(SM_FileHandle *handle, int index, SM_PageHandle buffer) {
    FILE *fp = (FILE *)handle->filePointer;
    if (!fp || index < 0 || index >= handle->totalNumPages) return RC_WRITE_FAILED;

    fseek(fp, (long)index * PAGE_SIZE, SEEK_SET);
    return fwrite(buffer, PAGE_SIZE, 1, fp) < 1 ? RC_WRITE_FAILED : RC_OK;
}

RC storeCurrentPage(SM_FileHandle *handle, SM_PageHandle buffer) {
    return storePage(handle, handle->currentPageIndex, buffer);
}

RC appendBlankPage(SM_FileHandle *handle) {
    FILE *fp = (FILE *)handle->filePointer;
    if (!fp) return RC_FILE_NOT_FOUND;

    void *buffer = calloc(1, PAGE_SIZE);
    if (!buffer) return RC_MEMORY_ALLOCATION_FAILED;

    fseek(fp, 0, SEEK_END);
    size_t written = fwrite(buffer, PAGE_SIZE, 1, fp);
    free(buffer);

    if (written < 1) return RC_WRITE_FAILED;

    handle->totalNumPages++;
    return RC_OK;
}

RC ensureStorageCapacity(int numPages, SM_FileHandle *handle) {
    while (handle->totalNumPages < numPages) {
        RC status = appendBlankPage(handle);
        if (status != RC_OK) return status;
    }
    return RC_OK;
}

#include<stdio.h>
#include<stdlib.h>
#include "buffer_mgr.h"
#include <string.h>
#include "storage_mgr.h"

int bufferSize = 0;

int lastIndex = 0;

int writeCount = 0;

int hitCount = 0;

int clockPtr = 0;

int lfuPtr = 0;


int leastLFUPos = 0;
int leastLFUCount = 0;
int leastHitCount = 0;
int leastHitPos = 0;

bool checkBufferPool(BM_BufferPool *const bufferPool)
{
    if(bufferPool == NULL) {
        return false;
    } else{
        return true;
    }
}

bool checkPageInfo(PageInfo *page)
{
    if(page == NULL) {
        return false;
    } else{
        return true;
    }
}

bool checkPageHandle(BM_PageHandle *const pageHandle)
{
    if(pageHandle == NULL) {
        return false;
    } else{
        return true;
    }
}

bool checkPageNum(int pageNum)
{
    if(pageNum < 0) {
        return false;
    } else{
        return true;
    }
}

void writeDirtyPage(char *pageFile,SM_FileHandle* fileHandle,int pageNum,SM_PageHandle data)
{
    openPageFile(pageFile, fileHandle);
    writeBlock(pageNum, fileHandle, data);
}

void fillNewPage(PageInfo* pageInfo,PageInfo *page,int headPos){
    pageInfo[headPos].data = page->data;
    pageInfo[headPos].pageNum = page->pageNum;
    pageInfo[headPos].dirtyBit = page->dirtyBit;
    pageInfo[headPos].fixCount = page->fixCount;
    pageInfo[headPos].hitNum = page->hitNum;
    pageInfo[headPos].refNum = page->refNum;
}

void fillNewPageForClock(PageInfo* pageInfo,PageInfo *page,int clockPtr){
    pageInfo[clockPtr].data = page->data;
    pageInfo[clockPtr].pageNum = page->pageNum;
    pageInfo[clockPtr].dirtyBit = page->dirtyBit;
    pageInfo[clockPtr].fixCount = page->fixCount;
    pageInfo[clockPtr].hitNum = page->hitNum;
}

void processDirtyPage(BM_BufferPool *const bufferPool,PageInfo* pageInfo,SM_FileHandle* fileHandle){
    if(pageInfo->dirtyBit) {
        writeDirtyPage(bufferPool->pageFile, fileHandle, pageInfo->pageNum, pageInfo->data);
        writeCount++;
    }
}

void processDirtyPageForLFU(BM_BufferPool *const bufferPool,PageInfo* pageInfo,SM_FileHandle* fileHandle,int leastLFUPos){
    if(pageInfo[leastLFUPos].dirtyBit) {
        writeDirtyPage(bufferPool->pageFile, fileHandle, pageInfo->pageNum, pageInfo->data);
        writeCount++;
    }
}
void processDirtyPageForLRU(BM_BufferPool *const bufferPool,PageInfo* pageInfo,SM_FileHandle* fileHandle,int leastHitPos){
    if(pageInfo[leastHitPos].dirtyBit) {
        writeDirtyPage(bufferPool->pageFile, fileHandle, pageInfo->pageNum, pageInfo->data);
        writeCount++;
    }
}









void findFirstZeroFixCountForLFU(PageInfo *pageInfo) {

    int i;
    for (i = 0; i < bufferSize; i++) {
        PageInfo leastLFUElement = pageInfo[i];
        if (leastLFUElement.fixCount == 0) {
            leastLFUPos = i;
            leastLFUCount = leastLFUElement.refNum;
            break;
        }
    }
}

void findFirstZeroFixCountForLRU(PageInfo *pageInfo) {

    int i;
    for (i = 0; i < bufferSize; i++) {
        PageInfo leastLRUElement = pageInfo[i];
        if (leastLRUElement.fixCount == 0) {
            leastHitPos = i;
            leastHitCount = leastLRUElement.hitNum;
            break;
        }
    }
}
void findMinRefCount(PageInfo *pageInfo) {
    int j ;
    for ( j= 0; j < bufferSize; j++) {
        PageInfo leastLFUElement = pageInfo[j];
        if(leastLFUElement.refNum < leastLFUCount && leastLFUElement.fixCount == 0){
            leastLFUPos = j;
            leastLFUCount = leastLFUElement.refNum;
        }
    }
}

void findMinHitCount(PageInfo *pageInfo) {
    int j ;
    for ( j= 0; j < bufferSize; j++) {
        PageInfo leastLRUElement = pageInfo[j];
        if(leastLRUElement.hitNum < leastHitCount ){
            leastHitPos = j;
            leastHitCount = leastLRUElement.hitNum;
        }
    }
}

extern void FIFOStrategy(BM_BufferPool *const bufferPool,  PageInfo *page)
{

    if(!checkBufferPool(bufferPool) || !checkPageInfo(page)){
        return;
    }

    int count  = bufferSize;
    PageInfo * pageInfoList =  (PageInfo *)(*bufferPool).mgmtData;
    int headPos = lastIndex % bufferSize ;
    while (count-- > 0) {
        PageInfo pi = pageInfoList[headPos];
        if(pi.fixCount == 0) {
            SM_FileHandle fh;
            processDirtyPage(bufferPool,&pi,&fh);

            fillNewPage(pageInfoList,page,headPos);
            break;
        } else{
            headPos = (headPos++) % bufferSize == 0 ? 0 : headPos;
        }
    }
}

extern void LFUStrategy(BM_BufferPool *const bufferPool, PageInfo *page)
{

    if(!checkBufferPool(bufferPool) || !checkPageInfo(page)){
        return;
    }


    PageInfo *pageInfo = (PageInfo *)(*bufferPool).mgmtData;

    findFirstZeroFixCountForLFU(pageInfo);

    findMinRefCount(pageInfo);
    SM_FileHandle fh;
    processDirtyPageForLFU(bufferPool,pageInfo,&fh,leastLFUPos);

    fillNewPage(pageInfo,page,leastLFUPos);
}



extern void LRUStrategy(BM_BufferPool *const bufferPool, PageInfo *page)
{
    if(!checkBufferPool(bufferPool) || !checkPageInfo(page)){
        return;
    }

    PageInfo *pageInfo = (PageInfo *)(*bufferPool).mgmtData;

    findFirstZeroFixCountForLRU(pageInfo);
    findMinHitCount(pageInfo);

    SM_FileHandle fh;
    processDirtyPageForLRU(bufferPool,pageInfo,&fh,leastHitPos);

    fillNewPage(pageInfo,page,leastHitPos);

}



extern void CLOCKStrategy(BM_BufferPool *const bufferPool,PageInfo *page)
{
    if(!checkBufferPool(bufferPool) || !checkPageInfo(page)){
        return;
    }

    PageInfo *pageInfo = (PageInfo *)bufferPool->mgmtData;
    for (; ;) {
        clockPtr = (clockPtr % bufferSize == 0) ? 0 : clockPtr;
        PageInfo* pi = &pageInfo[clockPtr];
        if( pageInfo[clockPtr].hitNum == 0 ) {
            SM_FileHandle fh;
            processDirtyPage(bufferPool,pi,&fh);
            fillNewPageForClock(pageInfo,page,clockPtr);
            clockPtr++;
            break;
        }else{
            pi->hitNum = 0;
            clockPtr++;
        }
    }
}



extern RC initBufferPool(BM_BufferPool *const bufferPool, const char *const pageFileName,
                         const int numPages, ReplacementStrategy strategy,
                         void *stratData)
{
    SM_FileHandle fHandle;
    if (openPageFile ((char *)pageFileName, &fHandle) != RC_OK){
        return RC_FILE_NOT_FOUND;
    }
    PageInfo *pageInfo = calloc(1,sizeof(PageInfo) * numPages);


    bufferSize = numPages;
    int i=0;
    while(i<bufferSize)
    {
        pageInfo[i].data = NULL;
        pageInfo[i].pageNum = -1;
        pageInfo[i].dirtyBit = 0;
        pageInfo[i].fixCount = 0;
        pageInfo[i].hitNum = 0;
        pageInfo[i].refNum = 0;
        i++;
    }
    writeCount =0;
    clockPtr = 0;
    lfuPtr = 0;
    (*bufferPool).mgmtData= pageInfo;
    (*bufferPool).pageFile = (char *)pageFileName;
    (*bufferPool).numPages = numPages;
    (*bufferPool).strategy = strategy;
    closePageFile(&fHandle);
    return RC_OK;
}

//

extern RC shutdownBufferPool(BM_BufferPool *const bufferPool)
{
    PageInfo *pageInfo = (PageInfo *)(*bufferPool).mgmtData;
    RC value;
    if((value = forceFlushPool(bufferPool)) != RC_OK){
        return value;
    }
    int i=0;
    while(i<bufferSize)
    {
        if(pageInfo[i].fixCount != 0)
            return RC_PINNED_NOT_OUT;
        i++;
    }
   if(pageInfo !=NULL){
       free(pageInfo);
   }

    (*bufferPool).numPages = 0;
    (*bufferPool).mgmtData = NULL;
    (*bufferPool).pageFile = NULL;
    return RC_OK;
}



extern RC forceFlushPool(BM_BufferPool *const bufferPool)
{
    SM_FileHandle fHandle;

    if (openPageFile ((char *)((*bufferPool).pageFile), &fHandle) != RC_OK){
        return RC_FILE_NOT_FOUND;
    }
    PageInfo *pageInfo = (PageInfo *)(*bufferPool).mgmtData;

    int i=0;
    while(i < bufferSize)
    {
        if(pageInfo[i].fixCount == 0 && pageInfo[i].dirtyBit == 1)
        {
            SM_FileHandle fHandle;
            openPageFile((*bufferPool).pageFile, &fHandle);
            writeBlock(pageInfo[i].pageNum, &fHandle, pageInfo[i].data);
            pageInfo[i].dirtyBit = 0;;
            writeCount=writeCount+1;
        }
        i++;
    }
    closePageFile(&fHandle);
    return RC_OK;
}





extern RC markDirty (BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle)
{
    int numPages = bufferPool->numPages;
    PageInfo *pageInfo = (PageInfo*) bufferPool->mgmtData;
    if(pageInfo == NULL){
        return RC_RM_UNKOWN_DATATYPE;
    }
    for (int i = 0; i < numPages; i++){
        if (pageInfo[i].pageNum == pageHandle->pageNum){
            pageInfo[i].dirtyBit = true;
            break;
        }
    }
    return RC_OK;
}



extern RC unpinPage (BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle)
{
    PageInfo *pageInfo = (PageInfo *)(*bufferPool).mgmtData;
    int i=0;
    while(i < bufferSize)
    {
        if(pageInfo[i].pageNum == (*pageHandle).pageNum)
        {
            pageInfo[i].fixCount = pageInfo[i].fixCount - 1;
            break;
        }
        i++;
    }
    return RC_OK;
}



extern RC forcePage (BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle)
{
    FILE* fp = fopen(bufferPool->pageFile, "rb+");
    if(fp == NULL){
        return RC_FILE_NOT_FOUND;
    }

    if(fseek(fp, (pageHandle->pageNum)*PAGE_SIZE, SEEK_SET) != 0){
        return RC_RM_UNKOWN_DATATYPE;
    }
    fwrite(pageHandle->data, PAGE_SIZE, 1, fp);
    writeCount ++;
    fclose(fp);
    PageInfo *pageInfo = (PageInfo*) bufferPool->mgmtData;
    for (int i = 0; i < bufferPool->numPages; i++){
        if (pageInfo[i].pageNum == pageHandle->pageNum){
            pageInfo[i].dirtyBit = true;
            break;
        }
    }
    return RC_OK;
}




extern RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page,
                   const PageNumber pageNum){
    PageInfo *pageInfo = (PageInfo *)bm->mgmtData;

    if(pageInfo[0].pageNum != -1){
        int i = 0;
        bool isFull = true;

       while (i < bufferSize)
        {
             if(pageInfo[i].pageNum == -1) {
                SM_FileHandle fh;
                openPageFile(bm->pageFile, &fh);
                pageInfo[i].data = (SM_PageHandle) calloc(1,PAGE_SIZE);
                readBlock(pageNum, &fh, pageInfo[i].data);
                pageInfo[i].pageNum = pageNum;
                pageInfo[i].fixCount = 1;
                pageInfo[i].refNum = 0;
                lastIndex++;
                hitCount++;

                if(bm->strategy == RS_LRU)
                    pageInfo[i].hitNum = hitCount;
                else if(bm->strategy == RS_CLOCK)
                    pageInfo[i].hitNum = 1;

                page->pageNum = pageNum;
                page->data = pageInfo[i].data;

                isFull = false;
                break;
            }else if (pageInfo[i].pageNum != -1){
                if(pageInfo[i].pageNum == pageNum)
                {
                    pageInfo[i].fixCount++;
                    isFull = false;
                    hitCount++;

                    if(bm->strategy == RS_LRU)
                        pageInfo[i].hitNum = hitCount;
                    else if(bm->strategy == RS_CLOCK)
                        pageInfo[i].hitNum = 1;
                    else if(bm->strategy == RS_LFU)
                        pageInfo[i].refNum++;

                    page->pageNum = pageNum;
                    page->data = pageInfo[i].data;

                    clockPtr++;
                    break;
                }
            }
            i++;
        }

        if(isFull == true)
        {
            PageInfo *newPage = (PageInfo *) calloc(1,sizeof(PageInfo));

            SM_FileHandle fh;
            openPageFile(bm->pageFile, &fh);
            newPage->data = (SM_PageHandle) calloc(1,PAGE_SIZE);
            readBlock(pageNum, &fh, newPage->data);
            newPage->pageNum = pageNum;
            newPage->dirtyBit = 0;
            newPage->fixCount = 1;
            newPage->refNum = 0;
            lastIndex++;
            hitCount++;

            if(bm->strategy == RS_LRU)
                newPage->hitNum = hitCount;
            else if(bm->strategy == RS_CLOCK)
                newPage->hitNum = 1;

            page->pageNum = pageNum;
            page->data = newPage->data;

            switch(bm->strategy)
            {
                case RS_FIFO:
                    FIFOStrategy(bm, newPage);

                    break;

                case RS_LRU:
                    LRUStrategy(bm, newPage);

                    break;

                case RS_CLOCK: // Using CLOCK algorithm
                    CLOCKStrategy(bm, newPage);

                    break;

                case RS_LFU: // Using LFU algorithm
                    LFUStrategy(bm, newPage);

                    break;

                case RS_LRU_K:
                    printf("\n LRU-k algorithm not implemented");
                    break;

                default:
                    printf("\nAlgorithm Not Implemented\n");
                    break;
            }

            free(newPage);
        }


        return RC_OK;
    } else if(pageInfo[0].pageNum == -1){
        // Reading page from disk and initializing page frame's content in the buffer pool
        SM_FileHandle fh;
        openPageFile(bm->pageFile, &fh);
        pageInfo[0].data = (SM_PageHandle) calloc(1,PAGE_SIZE);
        ensureCapacity(pageNum,&fh);
        readBlock(pageNum, &fh, pageInfo[0].data);
        pageInfo[0].pageNum = pageNum;
        pageInfo[0].fixCount++;
        lastIndex = hitCount = 0;
        pageInfo[0].hitNum = hitCount;
        pageInfo[0].refNum = 0;
        page->pageNum = pageNum;
        page->data = pageInfo[0].data;
        return RC_OK;
    }

}



// ***** STATISTICS FUNCTIONS ***** //


extern PageNumber *getFrameContents (BM_BufferPool *const bufferPool)
{
    PageNumber* pageArray = (PageNumber*)malloc(bufferSize * sizeof(PageNumber));
    if(pageArray == NULL){
        return pageArray;
    }
    PageInfo *pageInfo = (PageInfo*) bufferPool->mgmtData;

    for (int i = 0; i < bufferSize; i++) {
        if (pageInfo[i].pageNum == -1) {
            pageArray[i] = NO_PAGE;
        } else {
            pageArray[i] = pageInfo[i].pageNum;
        }
    }
    return pageArray;
}


extern bool *getDirtyFlags (BM_BufferPool *const bufferPool){
    bool* flagArray = (bool*)malloc(bufferPool->numPages * sizeof(bool));
    if(flagArray == NULL){
        return flagArray;
    }
    PageInfo *pageInfo = (PageInfo*) bufferPool->mgmtData;

    for (int i = 0; i < bufferPool->numPages; i++) {
        flagArray[i] = pageInfo[i].dirtyBit;
    }
    return flagArray;
}

extern int *getFixCounts (BM_BufferPool *const bufferPool)
{
    int* fixCountsArray = (int*)malloc(bufferPool->numPages * sizeof(int));
    if(fixCountsArray == NULL){
        return fixCountsArray;
    }
    PageInfo *pageInfo = (PageInfo*) bufferPool->mgmtData;

    for (int i = 0; i < bufferPool->numPages; i++) {
        fixCountsArray[i] = pageInfo[i].fixCount;
    }
    return fixCountsArray;
}

extern int getNumReadIO (BM_BufferPool *const bm)
{
    return (lastIndex + 1);
}

extern int getNumWriteIO (BM_BufferPool *const bm)
{
    return writeCount;
}
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<math.h>

#include "storage_mgr.h"

FILE *filePoint;

extern void initStorageManager (void) {
    // Initialising file pointer i.e. storage manager.
    filePoint = NULL;
}

//extern RC createfilePoint (char *fileName) {
//    // Opening file stream in read & write mode. 'w+' mode creates an empty file for both reading and writing.
//    filePoint = fopen(fileName, "w+");
//
//    // Checking if file was successfully opened.
//    if(filePoint == NULL) {
//        return RC_FILE_NOT_FOUND;
//    } else {
//        // Creating an empty page in memory.
//        SM_PageHandle emptyPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));
//
//        // Writing empty page to file.
//        if(fwrite(emptyPage, sizeof(char), PAGE_SIZE,filePoint) < PAGE_SIZE)
//            printf("write failed \n");
//        else
//            printf("write succeeded \n");
//
//        // Closing file stream so that all the buffers are flushed.
//        fclose(filePoint);
//
//        // De-allocating the memory previously allocated to 'emptyPage'.
//        // This is optional but always better to do for proper memory management.
//        free(emptyPage);
//
//        return RC_OK;
//    }
//}

RC createPageFile(char *fileName) {
	RC ReturnValue;
    filePoint = fopen(fileName,"w+"); //Create an empty file with read/write mode, if a file with the same name already exists its contents are discarded and the file is treated as a new empty file.
    char fillPage[PAGE_SIZE] = {1};//Initialize the file size with the array 1.
	char* space = (char *)calloc(PAGE_SIZE,sizeof(char)); //Fill this single page with ’\0’ bytes.
    if(filePoint==0)
        ReturnValue=RC_FILE_NOT_FOUND;
    else(fwrite(space, sizeof(char), PAGE_SIZE, filePoint));//Writing the allocated memory block into the file
        ReturnValue = RC_OK;
    free(space);                                 //Freeing the space after writing in
    fclose(filePoint);                                 //Once it's done, close the file
    return ReturnValue;

}

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle) {

    RC ReturnValue;
    int length;
    filePoint = fopen(fileName, "r+");		//File pointer as filename in read only mode
    if (filePoint == NULL)
        ReturnValue=RC_FILE_NOT_FOUND;
    else
    {
        fseek(filePoint, 0, SEEK_END);//fseek sets the file position of the fp point from the start to the EOF.
        int lastone = ftell(filePoint); 	//ftell returns the current file position of the given fp(EOF), which is the last position.
        length = lastone ; //length of the page file
        (*fHandle).fileName = fileName; //Set the fileName to the file handler
        (*fHandle).totalNumPages= length/PAGE_SIZE;//Set the the total Number of Pages of the file to the file handler
        (*fHandle).curPagePos = 0;//Set the current Page Position of the file to the file handler
        (*fHandle).mgmtInfo = filePoint;//Set the mgmtInfo of the file to the file handler
        rewind(filePoint); //Set the file position to the beginning of the file
        fclose(filePoint);
        ReturnValue = RC_OK;
    }
    return ReturnValue;
}

extern RC closePageFile (SM_FileHandle *fHandle) {

    filePoint = NULL;
    return RC_OK;
}


extern RC destroyPageFile (char *fileName) {
    int removeValue;
    removeValue=remove(fileName);
    if (removeValue != 0)//deletes the given filename so that it is no longer accessible
        return RC_FILE_NOT_FOUND;
    else
        return RC_OK;
}

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {

    int offset;
    int retCode;
    if(!fHandle) return RC_FILE_HANDLE_NOT_INIT;


    if(pageNum < 0 || pageNum >= fHandle->totalNumPages) return RC_READ_NON_EXISTING_PAGE;

    filePoint = fopen(fHandle->fileName, "r");
    if(filePoint == NULL){
        return RC_FILE_NOT_FOUND;
    }

    offset = pageNum * PAGE_SIZE;
    retCode = fseek(filePoint,offset,SEEK_SET);
    if(retCode == 0) {
         fread(memPage,sizeof (char),PAGE_SIZE,filePoint);
    }else {
        return RC_READ_NON_EXISTING_PAGE;
    }
    fHandle->curPagePos = offset;
    fclose(filePoint);
    return RC_OK;
}

extern int getBlockPos (SM_FileHandle *fHandle) {
    if(!fHandle) return RC_FILE_HANDLE_NOT_INIT;
//     checkFileHandleValid(fHandle);
    return fHandle->curPagePos;
}

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {

    return readBlock(0,fHandle,memPage);
}

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if(!fHandle) return RC_FILE_HANDLE_NOT_INIT;
    int currentPageNum = fHandle->curPagePos / PAGE_SIZE;

    return readBlock(currentPageNum-1,fHandle,memPage);

}

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if(!fHandle) return RC_FILE_HANDLE_NOT_INIT;
    int currentPageNum = fHandle->curPagePos / PAGE_SIZE;

    return readBlock(currentPageNum,fHandle,memPage);
}

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){

    if(!fHandle) return RC_FILE_HANDLE_NOT_INIT;
//     checkFileHandleValid(fHandle);
    int currentPageNum = fHandle->curPagePos / PAGE_SIZE;

    return readBlock(currentPageNum+1,fHandle,memPage);
}

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    if(!fHandle) return RC_FILE_HANDLE_NOT_INIT;
    int lastPageNum = fHandle->totalNumPages -1;
    return readBlock(lastPageNum,fHandle,memPage);
}

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    ensureCapacity (pageNum, fHandle); //Ensure that the capacity is sufficient
    filePoint = fopen(fHandle->fileName,"rb+"); //Opens the file, or returns if the file fails to open
    if(filePoint == NULL){
        return RC_FILE_NOT_FOUND;
    }

    if(fseek(filePoint,pageNum * PAGE_SIZE, SEEK_SET) != 0){ //Determine the size of the open file and whether it meets the requirements
        fclose(filePoint);
        if(memPage != NULL) {
            free(memPage);
            memPage = NULL;
        }
        return RC_READ_NON_EXISTING_PAGE;
    } else if (fwrite(memPage, PAGE_SIZE, 1, filePoint) != 1){ //Write data to a file and determine the return value
        fclose(filePoint);
        if(memPage != NULL) {
            free(memPage);
            memPage = NULL;
        }
        return RC_WRITE_FAILED;
    } else {
        fHandle->curPagePos=pageNum;	//Saves the current page location
        fclose(filePoint);
        if(memPage != NULL) {
            free(memPage);
            memPage = NULL;
        }

        return RC_OK;
    }
}



extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if(fHandle == NULL){ //Error code is returned when fHandle is empty
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fHandle->curPagePos < 0){ //If curPagePos < 0 is empty, page does not exist. An error code is displayed
        return RC_WRITE_FAILED;
    }
    return writeBlock(fHandle->curPagePos, fHandle, memPage); //write block
}


extern RC appendEmptyBlock (SM_FileHandle *fHandle) {
    if(fHandle == NULL){//Error code is returned when fHandle is empty
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fHandle->fileName == NULL){ //Error code is returned when fileName is empty
        return RC_FILE_HANDLE_NOT_INIT;
    }

    RC rv = RC_WRITE_FAILED;
    char* allocData = (char *)calloc(1, PAGE_SIZE); //Apply for the corresponding memory
    if(allocData != NULL){
        FILE* fp=fopen(fHandle->fileName,"ab+");//Opens the file, or returns if the file fails to open
        if(fp != NULL){
            if(fwrite(allocData, PAGE_SIZE, 1, fp) != 1){ //Writes data to a block,And determine the return value
                rv = RC_WRITE_FAILED;
            } else {
                fHandle -> totalNumPages += 1; //TotalNumPages increments 1 on success
                rv = RC_OK;
            }
            fclose(fp);
        }
        free(allocData); //Release the previously allocated memory
    }
    return  rv;
}

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle) {
    if(fHandle == NULL){//Error code is returned when fHandle is empty
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fHandle->fileName == NULL){//Error code is returned when fileName is empty
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fHandle->totalNumPages >= numberOfPages){//Determine whether the current totalNumPages is greater than the required numberOfPages
        return RC_OK;
    }

    long capacitySize= (numberOfPages - fHandle->totalNumPages) * PAGE_SIZE; //Calculate the size of the request
    char* allocDataPtr = (char *)calloc(1,capacitySize); //Apply for the corresponding memory

    if(allocDataPtr == NULL){ //Check whether data application is successful
        return RC_IM_NO_MORE_ENTRIES;
    }

    FILE* fp=fopen(fHandle->fileName,"ab+");//Opens the file, or returns if the file fails to open
    if(fp == NULL){
        return RC_FILE_NOT_FOUND;
    }
    size_t write_result = fwrite(allocDataPtr, capacitySize, 1, fp);//Writes data to a block,And determine the return value
    if(0 == write_result)   {//Write success
        fclose(fp);
        free(allocDataPtr);
        return RC_WRITE_FAILED;
    } else {//write failure
        fHandle -> totalNumPages = numberOfPages;
        fclose(fp);
        free(allocDataPtr);
        return RC_OK;
    }
}
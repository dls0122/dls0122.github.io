#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record_mgr.h"
#include "storage_mgr.h"
#include "buffer_mgr.h"


typedef struct ScanManager ScanManager;
bool hasAlreadInitBM = 0;
static RC attrOffset (Schema *schema, int attrNum, int *result);


int startWith(const char * str1, char *str2)
{
    if(str1 == NULL || str2 == NULL)
        return -1;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if((len1 < len2) ||  (len1 == 0 || len2 == 0))
        return -1;
    char *p = str2;
    int i = 0;
    while(*p != '\0')
    {
        if(*p != str1[i])
            return 0;
        p++;
        i++;
    }
    return 1;
}

/**
 *
 * @param mgmtData
 * @return
 */
RC initRecordManager (void *mgmtData) {

    tableManager = (TableManager*)malloc(sizeof(TableManager) );
    drm = (DeleteRecordManager *)malloc(sizeof (DeleteRecordManager));
    drm->slot = -2;
    drm->page = -2;
    scanManager = (ScanManager*) malloc(sizeof (ScanManager));
    scanManager->currentID.slot = 0;
    scanManager->currentID.page = 1;
    BM_BufferPool *bufferPool = ( BM_BufferPool *)malloc(sizeof ( BM_BufferPool));
            RecordManager*  recordManager = malloc(sizeof(RecordManager));
            recordManager->recordSize = 0;
            recordManager->totalRecord = 0;
            recordManager->status = 0;
            recordManager->freePageManager = (FreePageManager *) malloc(sizeof(FreePageManager));
            recordManager->freePageManager->freePage = 1;
            recordManager->freePageManager->freeSlot = 0;
            recordManager->records = (Record **) malloc(sizeof(Record *) * 500);
            recordManager->tableData = (RM_TableData *) malloc(sizeof(RM_TableData));
            recordManager->bufferPool = bufferPool;
            recordManager->deleteRecordManager = drm;
            tableManager->tableInfoList[0] = recordManager;
//    free(scanManager);
    initStorageManager();
    return RC_OK;
}

RC shutdownRecordManager (){
    for (int i = 0; i < 1; i++) {
        free(tableManager->tableInfoList[i]->tableData);
        free(tableManager->tableInfoList[i]->freePageManager);
        free(tableManager->tableInfoList[i]->records);
        free(tableManager->tableInfoList[i]->deleteRecordManager);
        if(tableManager->tableInfoList[i]->bufferPool != NULL){
            free(tableManager->tableInfoList[i]->bufferPool);
            tableManager->tableInfoList[i]->bufferPool = NULL;
        }
        free(tableManager->tableInfoList[i]);
    }
//    free(scanManager->scanHandle);
   free(scanManager);
    free(tableManager);
    return RC_OK;
}

RC createTable (char *tableName, Schema *schema) {

     RecordManager *rm = tableManager->tableInfoList[0];
     SM_FileHandle  fileHandle;
     char* pageHandle;

    initBufferPool(rm->bufferPool,tableName,MAX_NUM_PAGES,RS_LRU,NULL);
    char* md = malloc(sizeof (char) * PAGE_SIZE);
    memset(md,0,PAGE_SIZE);
    pageHandle = md;
    //1.freePage
    *(int*)pageHandle = 1;
    pageHandle+=sizeof (int);
    //2.freeSlot
    *(int*)pageHandle = 0;
    pageHandle+=sizeof (int);
    //3.recordSize
    *(int*)pageHandle = 0;
    pageHandle+=sizeof (int);
    //4.totalCount
    *(int*)pageHandle = 0;
    pageHandle+=sizeof (int);
    //5. numAttr
    *(int*)pageHandle = schema->numAttr;
    pageHandle+=sizeof (int);
    //6.keySize
    *(int*)pageHandle = schema->keySize;
    pageHandle+=sizeof (int);
    //7.keyAttrs
    *(int*)pageHandle = *schema->keyAttrs;
    pageHandle+=sizeof (int);
    //8.attrName,dataType,typeLength
    for (int i = 0; i <schema->numAttr ; ++i) {
        strncpy(pageHandle,schema->attrNames[i],20);
        pageHandle+=20;
        *(int*)pageHandle = schema->dataTypes[i];
        pageHandle+=sizeof (int);
        *(int*)pageHandle = schema->typeLength[i];
        pageHandle+=sizeof (int);
    }

    if(createPageFile(tableName)!=RC_OK)
    {
           return RC_ERROR;
    }
    if(openPageFile(tableName,&fileHandle)!=RC_OK)
    {
        return RC_ERROR;
    }

    if(writeBlock(0,&fileHandle,md)!=RC_OK)
    {
        return RC_ERROR;
    }

    if(closePageFile(&fileHandle)!=RC_OK)
    {
        return RC_ERROR;
    }
    return RC_OK;
}

int getNumTuples (RM_TableData *tableData){
    return tableManager->tableInfoList[0]->totalRecord;
}
RC openTable (RM_TableData *tableData, char *tableName){


      RC rc = RC_OK;
        RecordManager* rm = tableManager->tableInfoList[0];

        rm->bufferPool->mgmtData = tableName;
    initBufferPool(rm->bufferPool,tableName,MAX_NUM_PAGES,RS_LRU,NULL);
                BM_PageHandle * pageHandle = (BM_PageHandle*) malloc(sizeof (BM_PageHandle));
//                pageHandle->data = (char*)malloc(sizeof (char) * PAGE_SIZE);
                rm->pageHandle = pageHandle;
                rc = pinPage(rm->bufferPool,rm->pageHandle,0);
                if(rc != RC_OK) {
                    return RC_PINNED_PAGES_FAIL;
                }

                char* page = rm->pageHandle->data;
                Schema * schema = malloc(sizeof (Schema));
                rm->freePageManager->freePage = *(int*)page;
                page+=sizeof (int);
                rm->freePageManager->freeSlot = *(int*)page;
                page+=sizeof (int);

                rm->recordSize = *(int*)page;
                page+=sizeof (int);
                rm->totalRecord = *(int*)page;
                page+=sizeof (int);
                schema->numAttr = *(int*)page;
                int numAttr = *(int*)page;
                page+=sizeof (int);
                schema->keySize = *(int*)page;
                page+=sizeof (int);
                schema->keyAttrs = (int*)malloc(sizeof (int));
                schema->attrNames = (char** )malloc(sizeof (char*)*(schema->numAttr));
                schema->dataTypes = (int*) malloc(sizeof (int) * (schema->numAttr));
                schema->typeLength = (int*) malloc(sizeof (int) * (schema->numAttr));
                *schema->keyAttrs = *(int*)page;
                page+=sizeof (int);
                for(int i=0;i<schema->numAttr;i++)
                {
                    schema->attrNames[i] = malloc(sizeof(char) * 20);
                    strncpy(schema->attrNames[i],page,20);
                    page+=20;
                    schema->dataTypes[i] = *(int*)page;
                    page+=sizeof (int);
                    schema->typeLength[i] = *(int*)page;
                    page+=sizeof (int);
                }
                rm->tableData->schema = schema;
                rm->tableData->name = tableName;
                tableData =  rm->tableData;
                rc = unpinPage(rm->bufferPool,rm->pageHandle);
                if(rc != RC_OK) {
                    return RC_UNPINNED_PAGES_FAIL;
                }
    return RC_OK;

}

RC closeTable (RM_TableData *tableData){
        RecordManager* rm = NULL;
        rm = tableManager->tableInfoList[0];
        tableData = rm->tableData;
        char* ptr = ((PageInfo*)rm->bufferPool->mgmtData)[0].data;
        //1.freePage
        *(int*)ptr = rm->freePageManager->freePage;
        ptr+=sizeof (int);
        //2.freeSlot
        *(int*)ptr = rm->freePageManager->freeSlot;;
        ptr+=sizeof (int);
        //3.recordSize
        *(int*)ptr = getRecordSize(rm->tableData->schema);
        ptr+=sizeof (int);
        //4.totalCount
        *(int*)ptr = rm->totalRecord;
        ptr+=sizeof (int);
        rm->pageHandle->pageNum = 0;
        markDirty(rm->bufferPool,rm->pageHandle);
        forceFlushPool(rm->bufferPool);
        shutdownBufferPool(rm->bufferPool);
        free(rm->pageHandle);
        freeSchema(rm->tableData->schema);
        return RC_OK;

}

RC deleteTable (char *tableName){
    RC rc = destroyPageFile(tableName);
    if(rc != RC_OK)
    {
        return RC_RM_REMOVE_TABLE_FAIL;
    }
    return RC_OK;
}
int nextPage()
{
    RecordManager*  rm = tableManager->tableInfoList[0];
        return rm->freePageManager->freePage+1;

}

int nextSlot()
{
    RecordManager*  rm = tableManager->tableInfoList[0];

  if(rm->freePageManager->freeSlot >= ((PAGE_SIZE / rm->recordSize) -1))
  {
      rm->freePageManager->freePage = nextPage();
      rm->freePageManager->freeSlot = 0;
      return rm->freePageManager->freeSlot;
  }
   if(drm->slot != -2)
   {
       rm->freePageManager->freeSlot = drm->slot;
       return rm->freePageManager->freeSlot;
   }
   return  ++rm->freePageManager->freeSlot;
}
RC insertRecord (RM_TableData *tableData, Record *record){
    BM_BufferPool* bufferPool;
    BM_PageHandle*  pageHandle;
    //pin
//    pageHandle = (BM_PageHandle*) malloc(sizeof (BM_PageHandle));
//    pageHandle->data = (char*)malloc(sizeof (char) * PAGE_SIZE);

    RecordManager*  rm;

          rm = tableManager->tableInfoList[0];
    pageHandle = rm->pageHandle;
    rm->recordSize = getRecordSize(rm->tableData->schema);
    rm->freePageManager->freeSlot = nextSlot();
    rm->totalRecord++;
            tableData =  rm->tableData ;
            bufferPool = rm->bufferPool;
            char* page =  NULL;
            RC rc = pinPage(bufferPool,pageHandle, rm->freePageManager->freePage);
            if(rc != RC_OK) {
                return RC_PINNED_PAGES_FAIL;
            }
            int recordSize = getRecordSize(rm->tableData->schema);
            page= pageHandle->data;
            int offset = rm->freePageManager->freeSlot * recordSize;
            char* rdata = record->data;
    Schema  *schema = rm->tableData->schema;
            for (int i = 0; i < schema->numAttr; ++i) {
                if(schema->dataTypes[i] == DT_INT)
                {
                    memcpy(page+offset,rdata,sizeof (int));
                    page+=sizeof (int);
                    rdata+=sizeof (int);
                }else if(schema->dataTypes[i] == DT_STRING){
                    strncpy(page+offset,rdata,schema->typeLength[i]);
                    page+=schema->typeLength[i];
                    rdata+=schema->typeLength[i];
                }
            }

            rc =  markDirty(bufferPool,pageHandle);
            if(rc != RC_OK) {
                return RC_MAKE_DIRTY_FAIL;
            }

            record->id.page = rm->freePageManager->freePage;
            record->id.slot = rm->freePageManager->freeSlot;

            *rm->records = record;

            rc = unpinPage(bufferPool,pageHandle);
            if(rc != RC_OK) {
                return RC_UNPINNED_PAGES_FAIL;
            }
//            free(pageHandle->data);
//    free(pageHandle);
    return RC_OK;
}

RC deleteRecord (RM_TableData *tableData, RID id){
    //pin
//    BM_BufferPool* bufferPool;
//    BM_PageHandle*  pageHandle;
    RecordManager * rm;
        rm = tableManager->tableInfoList[0];
//        char* page ;
//
//            bufferPool = (rm)->bufferPool;
//            pageHandle = (rm)->pageHandle;
//
//            RC rc = pinPage(bufferPool,pageHandle, id.page);
//            if(rc != RC_OK) {
//                return RC_PINNED_PAGES_FAIL;
//            }
//            int recordSize = getRecordSize(rm->tableData->schema);
//            page= pageHandle->data;
//            page+=(id.slot * recordSize);
//
//            memset(page,'@',recordSize);
//
//            rc = markDirty(bufferPool,pageHandle);
//            if(rc != RC_OK){
//                return RC_MAKE_DIRTY_FAIL;
//            }
//            rc = unpinPage(bufferPool,pageHandle);
//            if(rc != RC_OK) {
//                return RC_UNPINNED_PAGES_FAIL;
//            }
         rm->deleteRecordManager->slot = id.slot;
    rm->deleteRecordManager->page = id.page;
            rm->totalRecord--;
    tableData->mgmtData = &rm;
    return RC_OK;
}

RC updateRecord (RM_TableData *tableData, Record *record){
    //pin
    BM_BufferPool* bufferPool;
    BM_PageHandle*  pageHandle;
    RecordManager * rm;
    for (int i = 0; i < 1; ++i) {
        rm = tableManager->tableInfoList[i];
        char* page ;
        char *rData;

            bufferPool = (rm)->bufferPool;
            pageHandle = (rm)->pageHandle;

            RC rc = pinPage(bufferPool,pageHandle, record->id.page);
            if(rc != RC_OK) {
                return RC_PINNED_PAGES_FAIL;
            }
            int recordSize = getRecordSize(rm->tableData->schema);
            page= pageHandle->data;
            page+=(record->id.slot * recordSize);

            memcpy(page,record->data,recordSize);

            rc = markDirty(bufferPool,pageHandle);
            if(rc != RC_OK){
                return RC_MAKE_DIRTY_FAIL;
            }
            rc = unpinPage(bufferPool,pageHandle);
            if(rc != RC_OK) {
                return RC_UNPINNED_PAGES_FAIL;
            }


    }
    tableData->mgmtData = &rm;
    return RC_OK;
}

RC getRecord (RM_TableData *tableData, RID id, Record *record){

    //pin
    BM_BufferPool* bufferPool;
    BM_PageHandle*  pageHandle;
    RecordManager * rm;
    char* page ;
    char *rData;
          rm = tableManager->tableInfoList[0];
    if(rm->deleteRecordManager->slot == id.slot)
    {
        return RC_RM_RECORD_IS_NOT_EXISTS;
    }
            bufferPool = (rm)->bufferPool;
            pageHandle = (rm)->pageHandle;

            RC rc = pinPage(bufferPool,pageHandle, id.page);
            if(rc != RC_OK) {
                return RC_PINNED_PAGES_FAIL;
            }
            record->id.slot = id.slot;
            record->id.page = id.page;
            int recordSize = getRecordSize(rm->tableData->schema);
            page= pageHandle->data;
            page+=(record->id.slot * recordSize);
            rData = record->data;
            memcpy(rData,page,recordSize);
            rc = unpinPage(bufferPool,pageHandle);
            if(rc != RC_OK) {
                return RC_UNPINNED_PAGES_FAIL;
            }

    tableData->mgmtData = &rm;
    return RC_OK;

}

RC startScan (RM_TableData *rel, RM_ScanHandle *scan, Expr *cond){

    RID currentID ;
    currentID.slot = 0;
    currentID.page = 1;
    scanManager->currentID = currentID;
    scanManager->scnCt = 0;
//    scanManager->scanHandle =
scanManager->scanHandle = scan;
    scanManager->scanHandle->rel = tableManager->tableInfoList[0]->tableData;
    scanManager->cond = cond;
    return RC_OK;
}
RC next (RM_ScanHandle *scan, Record *record){
    scan  = scanManager->scanHandle ;
    int totalRecords = getNumTuples(scan->rel);
    RecordManager * rm = tableManager->tableInfoList[0];
    Value * value ;
    int scnCt = scanManager->scnCt;
    scanManager->scnCt++;
    while (scnCt <= totalRecords)
    {

        if(getRecord(scan->rel,scanManager->currentID,record) != RC_OK)
        {
            return RC_RM_NO_MORE_TUPLES;
        }

            scnCt ++;

            evalExpr(record,rm->tableData->schema,scanManager->cond,&value);
            if(value->v.boolV == TRUE)
            {
                record->id.slot = scanManager->currentID.slot;
                record->id.page = scanManager->currentID.page;
                if(scanManager->currentID.slot == ((PAGE_SIZE / rm->recordSize)-1))
                {
                    scanManager->currentID.page ++;
                    scanManager->currentID.slot = 0;
                } else{
                    scanManager->currentID.slot +=1 ;
                }
                    freeExpr(value);
                return RC_OK;
            }

                if (scanManager->currentID.slot == ((PAGE_SIZE / rm->recordSize) - 1)) {
                    scanManager->currentID.page ++ ;
                    scanManager->currentID.slot = 0;
                } else {
                    scanManager->currentID.slot ++;
                }


        freeExpr(value);

    }
    closeScan(scan);
    return RC_RM_NO_MORE_TUPLES;
}



RC closeScan (RM_ScanHandle *scanHandle){
    scanManager->currentID.slot = 0;
    scanManager->currentID.page = 1;
    scanManager->scnCt = 0;
    return RC_OK;
}


int getRecordSize (Schema *schema){
    int size = 0;
     int numAttr = schema->numAttr;
    for(int i=0;i<numAttr;i++) {
                if (schema->dataTypes[i] == DT_STRING) {
                    size += schema->typeLength[i];
                } else if (schema->dataTypes[i] == DT_FLOAT) {
                size += sizeof (float );
                } else if (schema->dataTypes[i] == DT_BOOL) {
                    size += sizeof (short );
                } else if(schema->dataTypes[i] == DT_INT) {
                    size += sizeof (int );
                }
    }
    return size;
}

Schema *createSchema (int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys){
    Schema *schema = (Schema*)malloc(sizeof (Schema));
    if(schema == NULL) return RC_RM_MALLOC_ALLOCATE_FAIL;

            schema->dataTypes = dataTypes;
            schema->typeLength = typeLength;
            schema->attrNames = attrNames;
            schema->keyAttrs = keys;
            schema->keySize = keySize;
            schema->numAttr = numAttr;
    return schema;
}

RC freeSchema (Schema *schema){
    if(schema != NULL)
    {
        free(schema->keyAttrs);
        for (int i = 0; i < schema->numAttr; ++i) {
            free(schema->attrNames[i]);
        }
        free(schema->attrNames);
        free(schema->typeLength);
        free(schema->dataTypes);
    }
    free(schema);
    return RC_OK;
}
RC
attrOffset (Schema *schema, int attrNum, int *result)
{
    int offset = 0;
    int attrPos = 0;

    for(attrPos = 0; attrPos < attrNum; attrPos++)
        switch (schema->dataTypes[attrPos])
        {
            case DT_STRING:
                offset += schema->typeLength[attrPos];
                break;
            case DT_INT:
                offset += sizeof(int);
                break;
            case DT_FLOAT:
                offset += sizeof(float);
                break;
            case DT_BOOL:
                offset += sizeof(bool);
                break;
        }

    *result = offset;
    return RC_OK;
}


RC createRecord (Record **record, Schema *schema){
     Record* r = (struct Record*)malloc(sizeof (Record));
     int recordSize = getRecordSize(schema);
     r->data = (char*)malloc(sizeof (char) * recordSize);
    r->id.page = -1;
    r->id.slot = -1;
     *record = r;
    return RC_OK;
}



RC freeRecord (Record *record){
    if(record != NULL) {
        free(record->data);
    }
    free(record);
    return RC_OK;
}
char * attrPtr =NULL;
RC getAttr (Record *record, Schema *schema, int attrNum, Value **value){

    int position = 0;
    attrOffset(schema,attrNum,&position);
    attrPtr = (char*)(record->data + position);

    DataType dt = schema->dataTypes[attrNum];
    if(dt == DT_STRING){
        int length = schema->typeLength[attrNum];
            (*value) = (Value *) malloc(sizeof(Value));
			(*value)->dt = DT_STRING;
			(*value)->v.stringV = (char *) malloc(length +1);
        memcpy((*value)->v.stringV,attrPtr, length);
        (*value)->v.stringV[length] = '\0';
    }else if(dt == DT_INT){
        MAKE_VALUE(*value,dt, atoi(attrPtr));
        memcpy(&(*value)->v.intV,attrPtr, sizeof(int));
    }else if(dt == DT_BOOL){
        MAKE_VALUE(*value,dt, (bool )atoi(attrPtr));
        memcpy(&(*value)->v.boolV,attrPtr, sizeof(bool));
    }else if(dt == DT_FLOAT){
        MAKE_VALUE(*value,dt, (float )atoi(attrPtr));
        memcpy(&(*value)->v.floatV,attrPtr, sizeof(float ));
    }
    return RC_OK;
}

RC setAttr(Record *record, Schema *schema, int attrNum, Value *value){
//    record->data = malloc(sizeof getRecordSize(schema));
    int position = 0;
    attrOffset(schema,attrNum,&position);
    char * attrPtr = record ;
//     attrPtr = attrPtr + position;
    DataType dt = schema->dataTypes[attrNum];
    if(dt == DT_STRING){
        int len = schema->typeLength[attrNum];
//        strncpy(attrPtr, value->v.stringV, len);
        memcpy(((Record*)attrPtr)->data+position, value->v.stringV, len);
        value->v.stringV[len] = '\0';
    }else if(dt == DT_INT){
        memcpy(((Record*)attrPtr)->data+position,&value->v.intV, sizeof(int));
    }else if(dt == DT_BOOL){
        memcpy(((Record*)attrPtr)->data+position,&value->v.boolV, sizeof(bool));
    }else if(dt == DT_FLOAT){
        memcpy(((Record*)attrPtr)->data+position,&value->v.floatV, sizeof(float ));
    }
    return RC_OK;
}

RC updateScan (RM_TableData *tableData,Record *record, Record *updated, RM_ScanHandle *scan){

    RecordManager* rm = tableManager->tableInfoList[0];
    for (int i = 0; i < rm->totalRecord; ++i) {
        if(!next(scan,record))
        {
            updated->id.slot = record->id.slot;
            updated->id.page = record->id.page;
            updateRecord(tableData,updated);
            return RC_OK;
        }
    }
    return RC_OK;
}

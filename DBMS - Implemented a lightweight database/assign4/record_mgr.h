#ifndef RECORD_MGR_H
#define RECORD_MGR_H

#include "dberror.h"
#include "expr.h"
#include "tables.h"
#include "buffer_mgr.h"
#define MAX_RECORD_NUM_PER_PAGE  4096
#define MAX_NUM_PAGES 100
// Bookkeeping for scans
typedef struct FreePageManager
{
    int freePage;
    int freeSlot;
}FreePageManager;

typedef struct DeleteRecordManager
{
    int page;
    int slot;
}DeleteRecordManager;
typedef struct ScanManager
{
    int scnCt;
    RID currentID;
    RM_ScanHandle*  scanHandle;
    Expr * cond;
}ScanManager;
typedef struct RecordManager
{
    BM_BufferPool* bufferPool;
    BM_PageHandle* pageHandle;
    Record** records;
    RM_TableData* tableData;
    int recordSize;
    int totalRecord;
    FreePageManager* freePageManager;
    DeleteRecordManager * deleteRecordManager;
    int status;
} RecordManager;




typedef struct TableManager
{
   RecordManager* tableInfoList[1] ;
}TableManager;


TableManager* tableManager;
DeleteRecordManager* drm;
ScanManager* scanManager;
// table and manager
extern RC initRecordManager (void *mgmtData);
extern RC shutdownRecordManager ();
extern RC createTable (char *name, Schema *schema);
extern RC openTable (RM_TableData *rel, char *name);
extern RC closeTable (RM_TableData *rel);
extern RC deleteTable (char *name);
extern int getNumTuples (RM_TableData *rel);
extern RC updateScan (RM_TableData *rel,Record *record, Record *updated, RM_ScanHandle *scan);
// handling records in a table
extern RC insertRecord (RM_TableData *rel, Record *record);
extern RC deleteRecord (RM_TableData *rel, RID id);
extern RC updateRecord (RM_TableData *rel, Record *record);
extern RC getRecord (RM_TableData *rel, RID id, Record *record);

// scans
extern RC startScan (RM_TableData *rel, RM_ScanHandle *scan, Expr *cond);
extern RC next (RM_ScanHandle *scan, Record *record);
extern RC closeScan (RM_ScanHandle *scan);

// dealing with schemas
extern int getRecordSize (Schema *schema);
extern Schema *createSchema (int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys);
extern RC freeSchema (Schema *schema);

// dealing with records and attribute values
extern RC createRecord (Record **record, Schema *schema);
extern RC freeRecord (Record *record);
extern RC getAttr (Record *record, Schema *schema, int attrNum, Value **value);
extern RC setAttr (Record *record, Schema *schema, int attrNum, Value *value);

#endif // RECORD_MGR_H

#include <stdlib.h>
#include "dberror.h"
#include "expr.h"
#include "record_mgr.h"
#include "tables.h"
#include "test_helper.h"


#define ASSERT_EQUALS_RECORDS(_l,_r, schema, message)			\
  do {									\
    Record *_lR = _l;                                                   \
    Record *_rR = _r;                                                   \
    ASSERT_TRUE(memcmp(_lR->data,_rR->data,getRecordSize(schema)) == 0, message); \
    int i;								\
    for(i = 0; i < schema->numAttr; i++)				\
      {									\
        Value *lVal, *rVal;                                             \
		char *lSer, *rSer; \
        getAttr(_lR, schema, i, &lVal);                                  \
        getAttr(_rR, schema, i, &rVal);                                  \
		lSer = serializeValue(lVal); \
		rSer = serializeValue(rVal); \
        ASSERT_EQUALS_STRING(lSer, rSer, "attr same");	\
		free(lVal); \
		free(rVal); \
		free(lSer); \
		free(rSer); \
      }									\
  } while(0)

#define ASSERT_EQUALS_RECORD_IN(_l,_r, rSize, schema, message)		\
  do {									\
    int i;								\
    boolean found = false;						\
    for(i = 0; i < rSize; i++)						\
      if (memcmp(_l->data,_r[i]->data,getRecordSize(schema)) == 0)	\
	found = true;							\
    ASSERT_TRUE(0, message);						\
  } while(0)

#define OP_TRUE(left, right, op, message)		\
  do {							\
    Value *result = (Value *) malloc(sizeof(Value));	\
    op(left, right, result);				\
    bool b = result->v.boolV;				\
    free(result);					\
    ASSERT_TRUE(b,message);				\
   } while (0)



// struct for test records
typedef struct TestRecord {
    int a;
    char *b;
    int c;
} TestRecord;

// helper methods
Record *testRecord(Schema *schema, int a, char *b, int c);
Schema *testSchema (void);
Record *fromTestRecord (Schema *schema, TestRecord in);
void testUpdateScan(void);
// test name
char *testName;

// main method
int
main (void)
{
    testName = "";
    testUpdateScan();
    return 0;
}


void testUpdateScan(void)
{
    RM_TableData *table = (RM_TableData *) malloc(sizeof(RM_TableData));
    TestRecord inserts[] = {
            {1, "aaaa", 3},
            {2, "bbbb", 2},
            {3, "cccc", 1},
            {4, "eeee", 3},
            {5, "dddd", 5},
            {6, "ffff", 1},
            {7, "gggg", 3},
            {8, "hhhh", 3},
            {9, "iiii", 2},
            {10, "ddd2", 5},
    };
    bool foundScan[] = {
            FALSE,
            FALSE,
            FALSE,
            FALSE,
            FALSE,
            FALSE,
            FALSE,
            FALSE,
            FALSE,
            FALSE
    };

    TestRecord updates[] = {
            {22, "dddd", 8}
    };
    TestRecord updateRec[] = {
            {1, "aaaa", 3},
            {2, "bbbb", 2},
            {3, "cccc", 1},
            {4, "dddd", 3},
            {22, "dddd", 8},
            {6, "ffff", 1},
            {7, "gggg", 3},
            {8, "hhhh", 3},
            {9, "iiii", 2},
            {22, "dddd", 8},
    };
    int numInserts = 10, i;
    Record *r,*updateRecord;
    RID *rids;
    Schema *schema;
    RM_ScanHandle *sc = (RM_ScanHandle *) malloc(sizeof(RM_ScanHandle));
    Expr *sel, *left, *right;
    int rc;

    testName = "test update scan";
    schema = testSchema();
    rids = (RID *) malloc(sizeof(RID) * numInserts);

    TEST_CHECK(initRecordManager(NULL));
    TEST_CHECK(createTable("test_table_r",schema));
    TEST_CHECK(openTable(table, "test_table_r"));

    // insert rows into table
    for(i = 0; i < numInserts; i++)
    {
        r = fromTestRecord(schema, inserts[i]);
        TEST_CHECK(insertRecord(table,r));
        rids[i] = r->id;

    }

    TEST_CHECK(closeTable(table));
    TEST_CHECK(openTable(table, "test_table_r"));


    MAKE_CONS(left, stringToValue("sdddd"));
    MAKE_ATTRREF(right, 1);
    MAKE_BINOP_EXPR(sel, left, right, OP_COMP_EQUAL);

    TEST_CHECK(startScan(table, sc, sel));
    updateRecord = fromTestRecord(schema, updates[0]);
    for (int j = 0; j <numInserts ; ++j) {
        if(updates[0].a == updateRec[j].a)
        {
            r->id.page = rids[j].page;
            r->id.slot = rids[j].slot;
            updateScan(table,r,updateRecord,sc);
            getRecord(table,r->id,r);
            TEST_CHECK(closeScan(sc));
            TEST_CHECK(startScan(table,sc,sel));
            while((rc = next(sc, r)) == RC_OK)
            {
               char* str = serializeRecord(r, schema);
                for(i = 0; i < numInserts; i++)
                {
                    Record * r1 = fromTestRecord(schema, updateRec[i]);
                    if (memcmp(r1->data,r->data,getRecordSize(schema)) == 0)
                        foundScan[i] = TRUE;
                    freeRecord(r1);
                }
                free(str);
            }
        }
    }

    freeRecord(updateRecord);
    if (rc != RC_RM_NO_MORE_TUPLES)
        TEST_CHECK(rc);
    TEST_CHECK(closeScan(sc));

    ASSERT_TRUE(!foundScan[0], "not greater than seven");
    ASSERT_TRUE(foundScan[4], "greater than seven");
    ASSERT_TRUE(foundScan[9], "greater than seven");

    // clean up
    TEST_CHECK(closeTable(table));
    TEST_CHECK(deleteTable("test_table_r"));
    TEST_CHECK(shutdownRecordManager());

    freeRecord(r);
    free(rids);
    free(table);
    free(sc);
    freeExpr(sel);
    freeSchema(schema);
    TEST_DONE();
}


Schema *
testSchema (void)
{
    Schema *result;
    char *names[] = { "a", "b", "c" };
    DataType dt[] = { DT_INT, DT_STRING, DT_INT };
    int sizes[] = { 0, 4, 0 };
    int keys[] = {0};
    int i;
    char **cpNames = (char **) malloc(sizeof(char*) * 3);
    DataType *cpDt = (DataType *) malloc(sizeof(DataType) * 3);
    int *cpSizes = (int *) malloc(sizeof(int) * 3);
    int *cpKeys = (int *) malloc(sizeof(int));

    for(i = 0; i < 3; i++)
    {
        cpNames[i] = (char *) malloc(2);
        strcpy(cpNames[i], names[i]);
    }
    memcpy(cpDt, dt, sizeof(DataType) * 3);
    memcpy(cpSizes, sizes, sizeof(int) * 3);
    memcpy(cpKeys, keys, sizeof(int));

    result = createSchema(3, cpNames, cpDt, cpSizes, 1, cpKeys);

    return result;
}

Record *
fromTestRecord (Schema *schema, TestRecord in)
{
    return testRecord(schema, in.a, in.b, in.c);
}

Record *
testRecord(Schema *schema, int a, char *b, int c)
{
    Record *result;
    Value *value;

    TEST_CHECK(createRecord(&result, schema));

    MAKE_VALUE(value, DT_INT, a);
    TEST_CHECK(setAttr(result, schema, 0, value));
    freeVal(value);

    MAKE_STRING_VALUE(value, b);
    TEST_CHECK(setAttr(result, schema, 1, value));
    freeVal(value);

    MAKE_VALUE(value, DT_INT, c);
    TEST_CHECK(setAttr(result, schema, 2, value));
    freeVal(value);

    return result;
}

#include "btree_mgr.h"
#include "tables.h"
#include "storage_mgr.h"
#include "record_mgr.h"
#include <stdlib.h>
#include <string.h>

SM_FileHandle fh;
int maxKeyNum = 0;
int tempEL[2][10];
typedef struct Node
{
    int *key;
    struct Node **ptr;
    int kNum;
    RID *id;
    DataType keyType;
} Node;


Node *root;
Node *scan;
int indexNum = 0;
RC initIndexManager (void *mgmtData)
{
    initStorageManager();
    return RC_OK;
}

RC shutdownIndexManager ()
{
    return RC_OK;
}

RC createBtree (char *idxId, DataType keyType, int n)
{

    if(n < 0 || idxId == NULL) return RC_ERROR;
    root = (Node*)malloc(sizeof(Node));
    root->id =  malloc(sizeof (int)*n);
    root->key = (int*)malloc(sizeof (int)*n);
    root->kNum = n;
    root->keyType = keyType;
    root->ptr = ( Node *)malloc(sizeof(Node)*(n+1));

    int i = 0;
    while (i<n+1) {
        root->ptr[i] = 0;
        i++;
    }
    maxKeyNum = n;
    char* initData= (char*)malloc(sizeof(char)*PAGE_SIZE);

    memset(initData,0,PAGE_SIZE);
    RC rc = createPageFile(idxId);
    if(rc!=RC_OK){
        return rc;
    }
    rc = openPageFile(idxId,&fh);
    if(rc!=RC_OK){
        return rc;
    }
    rc = writeBlock(0,&fh,initData);
    if(rc!=RC_OK){
        return rc;
    }
    rc = closePageFile(&fh);
    if(rc!=RC_OK){
        return rc;
    }
    return RC_OK;
}

RC openBtree (BTreeHandle **tree, char *idxId)
{
   RC rc = openPageFile(idxId,&fh);
    if(rc!=RC_OK){
        return RC_ERROR;
    }
    return RC_OK;
}

RC closeBtree (BTreeHandle *tree)
{
    RC rc  =closePageFile(&fh);
//    if(rc == RC_OK)
    if(root!=NULL)
    {
        free(root->id);
        free(*root->ptr);
        free(root->ptr);
       free(root);
       root = NULL;
    }
    return rc;

}
void freeNode(Node* node)
{
    if(node == NULL) return;
    for (int i=0;i<2;i++)
    {
        if(node->ptr[i] == NULL){
            free(node);
        }else {
            freeNode(node->ptr[i]);
        }
    }
}
RC deleteBtree (char *idxId)
{
    RC rc  =destroyPageFile(idxId);
    if(root!=NULL){
//        free(root->id);
//        free(*root->ptr);
//        free(root->ptr);

//        freeNode(root);
        free(root);
    }
    return rc;
}


RC getNumNodes (BTreeHandle *tree, int *result)
{

    int count=0,i =0;
    while (i<=maxKeyNum+1)
    {
        count++;
        i++;
    }
    *result = count;

    return RC_OK;
}

RC getNumEntries (BTreeHandle *tree, int *result)
{

    int count= 0;

    Node* node = root;
    while (node!=NULL){
        int i =0;
        while (i<maxKeyNum){
            if(node->key[i] != 0) {
                count++;
            }
            i++;
        }
        node = node->ptr[maxKeyNum];
    }
    *result = count;
     return RC_OK;
}



RC getKeyType (BTreeHandle *tree, DataType *result)
{
    return RC_OK;
}


RC findKey (BTreeHandle *tree, Value *key, RID *result)
{
    RC rc = RC_IM_KEY_NOT_FOUND;
    Node* node = root;
    while (node!=NULL){
        int i =0;
        while (i<maxKeyNum){
            if (node->key[i] == key->v.intV) {
                (*result).page = node->id[i].page;
                (*result).slot = node->id[i].slot;
               rc = RC_OK;
               goto here;
            }
            i++;
        }
        node = node->ptr[maxKeyNum];
    }
    here:
    return rc;
}



RC insertKey (BTreeHandle *tree, Value *key, RID rid)
{
    Node *temp = NULL;
    Node *node = (Node*)malloc(sizeof(Node));
    node->key = malloc(sizeof(int) * maxKeyNum);
    node->id = malloc(sizeof(int) * maxKeyNum);
    node->ptr = (Node**)malloc(sizeof(Node*) * (maxKeyNum + 1));

    for (int i = 0; i < maxKeyNum+1; i ++) {
        node->ptr[i] = malloc(sizeof(Node));
    }
    for (int i = 0; i < maxKeyNum; i ++) {
        node->key[i] = 0;
    }


    bool isFull = FALSE;
    temp = root;
    while (temp!=NULL){
        int i =0;
        while (i<maxKeyNum){
            if(temp->key[i] == 0) {
                temp->id[i].page = rid.page;
                temp->id[i].slot = rid.slot;
                temp->key[i] = key->v.intV;
                temp->ptr[i] = NULL;
                 isFull = TRUE;
                break;
            }
            i++;
        }


        if ((!isFull) && (temp->ptr[maxKeyNum] == NULL)) {
            node->ptr[maxKeyNum] = NULL;
            temp->ptr[maxKeyNum] = node;
        }

        temp = temp->ptr[maxKeyNum];

    }

     int ct =0;
    temp = root;
    while (temp!=NULL){
        int i =0;
        while (i<maxKeyNum){
            if(temp->key[i] != 0) {
                ct++;
            }
            i++;
        }
        temp = temp->ptr[maxKeyNum];
    }

    return RC_OK;
}

RC deleteKey (BTreeHandle *tree, Value *key)
{
RC rc = RC_ERROR;
    Node* node = root;
    while (node!=NULL){
        int i =0;
        while (i<maxKeyNum){
            if (node->key[i] == key->v.intV) {
                node->key[i] = 0;
                node->id[i].page = 0;
                node->id[i].slot = 0;
                rc = RC_OK;
                goto here;
            }
            i++;
        }
        node = node->ptr[maxKeyNum];
    }
    here:
//    free(node);
    return rc;
}

void bubble_sort(int a[],int n)
{
        int tempPage, tempSlot;
    for(int i=0; i<n-1; i++)
    {
        for(int j=0; j<n-1-i; j++)
        {
            if(a[j] > a[j+1])
            {
                int temp = a[j];
                tempPage = tempEL[0][j];
                tempSlot = tempEL[1][j];
                a[j] = a[j+1];
                tempEL[0][j] = tempEL[0][j + 1];
                tempEL[1][j] = tempEL[1][j + 1];
                a[j+1]=temp;
                tempEL[0][j + 1] = tempPage;
                tempEL[1][j + 1] = tempSlot;
            }
        }
    }
}

RC openTreeScan (BTreeHandle *tree, BT_ScanHandle **handle)
{
    scan = root;
    indexNum = 0;

    Node *node =NULL;
    int ct = 0;

    node = root;
    while (node!=NULL){
        int i =0;
        while (i<maxKeyNum){
            if(node->key[i] != 0) {
                ct++;
            }
            i++;
        }
        node = node->ptr[maxKeyNum];
    }

    int key[ct];
ct = 0;
    node = root;
    while (node!=NULL){
        int i =0;
        while (i<maxKeyNum){
            key[ct] = node->key[i];
            tempEL[0][ct] = node->id[i].page;
            tempEL[1][ct] = node->id[i].slot;
            ct ++;
            i++;
        }
        node = node->ptr[maxKeyNum];
    }

    bubble_sort(key,ct);

    ct = 0;
    node = root;
    while (node!=NULL){
        int i =0;
        while (i<maxKeyNum){
            key[ct] = node->key[ct];
            node->id[i].page = tempEL[0][ct];
            node->id[i].slot = tempEL[1][ct];
            ct ++;
            i++;
        }
        node = node->ptr[maxKeyNum];
    }

    return RC_OK;
}


RC nextEntry (BT_ScanHandle *handle, RID *result)
{


    if(scan->ptr[maxKeyNum] != NULL) {
        if(maxKeyNum == indexNum) {
            indexNum = 0;
            scan = scan->ptr[maxKeyNum];
        }

        (*result).page = scan->id[indexNum].page;
        (*result).slot = scan->id[indexNum].slot;
        indexNum ++;
    }
    else{

         if(maxKeyNum == indexNum){
             indexNum = 0;

             return RC_IM_NO_MORE_ENTRIES;
         }

        (*result).page = scan->id[indexNum].page;
        (*result).slot = scan->id[indexNum].slot;
        indexNum ++;
    }


    return RC_OK;
}


RC closeTreeScan (BT_ScanHandle *handle)
{
    indexNum = 0;
    free(scan);
    return RC_OK;
}


char *printTree (BTreeHandle *tree)
{
    return RC_OK;
}
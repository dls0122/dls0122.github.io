--------Record Manager CS - 525 Assignment No.3-------------
-Xu haoliang  - A20476453
-Dong shansi  - A20466369
-Zou bin - A20467075

RUNNING THE SCRIPT

1) Go to root assign3 by using Terminal.

2) Type command "ls" to list the files and check that the current directory is correct.

3) Type command "make clean" to delete old compiled .o files.

4) Type command "make test1" to compile all project files including "test_assign3_1.c " file

5) Type command "make run_test1" to run "test_assign3_1.c" file.

6) Type command "make test2" to compile all project files including "test_assign3_2.c " file

7) Type command "make run_test2" to run "test_assign3_2.c" file.

8) Type command "make test_expr" to compile test expression related files including "test_expr.c".

9) Type command "make run_expr" to run "test_expr.c" file.


SOLUTION DESCRIPTION

0.
project structure

source file
  ***buffer_mgr.c
  ***buffer_mgr_stat.c
  ***dberror.c
  ***storage_mgr.c
  ***record_mgr.c
  ***expr.c
  ***rm_serializer.c

head file
  ***dberror.h
  ***buffer_mgr.h
  ***storage_mgr.h
  ***buffer_mgr_stat.h
  ***record_mgr.h
  ***table.h

test source file
  ***test_assign3_1.c
  ***test_assign3_2.c
  ***test_expr.c

test head file
 ***test_helper.h


compile file
 ***makefile

README file
 ***README.md


1. TABLE AND RECORD MANAGER FUNCTIONS

The recordmanager related functions are used to initialize and shut down the recordmanager. We use the table-related functions to create, open, close and delete tables. We make use of BufferManager to access pages via Buffer Pool using a page replacement policy. Also, the StorageManager is used indirectly to perform operations on page files on disks.

initRecordManager (...)
--> This function initializes the record manager.
--> The initStorageManager(...) function of Storage Manager is called to initialize the storagemanager.

shutdownRecordManager(...)
--> This function shuts down the scanManager,tableManager, recordmanager and de-allocates all the resources allocated to the recordmanager.
--> The function frees up all resources/memory space being used by the Record Manager.
--> We set the manager data structure pointer to NULL and C function free() is called to de-allocate the memory space.

createTable(...)
--> This function opens the table with the name specified by the parameter's name.
--> By calling initBufferPool(), this function initializes the Buffer Pool. The LRU page replacement policy is used.
--> All the values of the table are initialized, and this function also sets the following attributes of the table: name, data type and size.
--> This function creates a page file, opens that page file, writes the block containing the table in the page file, and then closes the page file.

openTable(...)
--> This function creates a table with name as specified in the parameter 'name' in the schema specified in the parameter 'schema'.
--> This function will open the file with the given name. It checks if the file already exists. If not, it throws the error. It initializes the buffer manager with the given filename. It then loads the table info and schema into memory.

closeTable(...)
--> This function closes the table, pointed by the parameter 'tableData', by calling BufferManager's function: shutdownBufferPool().
--> Before shutting the buffer pool, the buffermanager writes the table's changes in the pagefile.

deleteTable()
-->The table with name specified by the parameter 'name' is deleted.
--> StorageManager's function destroyPageFile(...) is called in this function.
--> Function destroyPageFile() deletes the page from disk and frees up ane memory space which is allocated for that kind of mechanism.

getNumTuples()
--> The function returns the total number of tuples in a given table which is referenced by the parameter 'rel'.


2. RECORD FUNCTIONS

We use these functions retrieve a record with a certain RID, delete a record with a certain RID, insert a new record, and update an existing record with new values.

insertRecord(...)
--> The function inserts a new record in the table and then updates the 'record' parameter with the Record ID passed in the function insertRecord(), if there is any available slot for it.

deleteRecord(...)
--> This function deletes the record having RID 'id' passed through the parameter from the given table referenced by the parameter 'rel'.
--> We set our table's meta-data fPage to the PID of the page whose record is gonna be deleted, so that this space can be used by a new record later.
--> At last, we mark the page dirty so that the BufferManager can save the page's contents back to disk and then unpin this page.

updateRecord()
--> The function updates a record that is referenced by the parameter "record" in the table that is referenced by the parameter "rel".
--> This function finds the page where the record is located by the table's meta-data and pins that page in the bufferpool.
--> This function sets the Record ID, navigates to the record's data’s location.
--> We copy the record's data (passed through parameter 'record') into the new record using memcpy() C function, mark the page dirty and then unpin the page.

getRecord(....)
--> This function retrieves a record having Record ID "id" passed in the parameter in the table referenced by the parameter "rel", which is also passed in the parameter. The resulting record is stored in the location which is referenced by the parameter "record".
--> The function finds the page where the record is located by the table's meta-data and by using the 'id' of the record, it pins that page in the bufferpool.
--> The function sets the Record ID of the 'record' parameter with the record's id which exists on the page and copies the data too.
--> At last, it unpins the page.


3. SCAN FUNCTIONS

The Scan related functions are used to retrieve all tuples from a table that fulfill a certain expr (represented as an Expr). Starting a scan initializes the RM_ScanHandle data structure passed as an argument to startScan. Afterward, calls to the next method is made which returns the next tuple that fulfills the scan expr.

startScan(...)
--> The function starts a scan by getting data from the RM_ScanHandle data structure passed as an argument to the startScan() function.
--> We initialize our custom data structure's scan-related variables.
--> If expr is NULL, the error code RC_SCAN_CONDITION_NOT_FOUND would be returned.

next(...)
--> The function returns the next tuple which satisfies the expr (test expression).
--> If expr iS NULL, the error code RC_SCAN_CONDITION_NOT_FOUND would be returned.
--> If there’re no tuples in the table, the error code RC_RM_NO_MORE_TUPLES would be returned.
--> We’ll iterate through the table’s tuples. Pinning the page having that tuple, navigate to the stored data’s location, copy data into a temporary buffer, and evaluate the test expression by calling function eval().
--> If we get the TRUE result (v.boolV) of the test expression , that means the tuple fulfills the expr. Then we can unpin this page, and return RC_OK.
--> If no tuples fulfill the expr, then the error code RC_RM_NO_MORE_TUPLES would be returned.

closeScan(...)
--> his function closes the scan operation.
--> We check if the scan was incomplete by checking the scanned count value of the table's metadata. If it is greater than 0, it means the scan was incomplete.
--> If the scan was incomplete, we unpin the page and reset all scan mechanism-related variables in our table's meta-data (custom data structure).
--> We then free (de-allocate) the space occupied by the metadata.


4. SCHEMA FUNCTIONS

These functions return the size in bytes of records for a given schema and create a new schema.

getRecordSize(...)
--> This function returns the size of a record in the specified schema.
--> We iterate through the attributes of the schema. We iteratively add the size (space in bytes) required by each attribute to the variable 'size'.
--> The value of the variable 'size' is the size of the record.

freeSchema(...)
--> This function will frees the memory assigned to data type, attributes names, attribute size, keys size and in the end it will free memory for schema.


createSchema(...)
--> This function create create the schema object and number of attributes, their datatypes and the size is stored.
--> numAttr specifies the number of parameters. attrNames specifies the name of the attributes. datatypes specify the data type of the attributes. typeLength specifies the length of the attribute (example: length of STRING).


5. ATTRIBUTE FUNCTIONS

These functions are used to get or set the attribute values and create a new record for a given schema. Creating a new record should allocate enough memory to the data field to hold the binary representations for all record attributes as determined by the schema.

createRecord(...)
--> This function creates a new record in the schema passed by parameter 'schema' and passes the new record to the 'record' parameter in the createRecord() function.
--> We allocate proper memory space to the new record. Also we give memory space for the data of the record which is the record size.
--> Also, we add a '-' to the first position and append '\0' which NULL in C. '-' denotes that this is a new blank record.
--> Finally, we assign this new record to the 'record' passed through the parameter.

attrOffset(...)
--> The function is used to get the offset value of different attributes as per the attribute numbers. Then attribute data is assigned to value pointer as per different data types.

freeRecord(...)
--> The function frees up the memory space allocated to the 'record' passed through the parameter.
-->The C function free() is used to free up the record’s memory space.

getAttr(...)
--> This function will basically allocates the space to the value data structre where the attribute values is to be fetched.
-->We locate the attribute using the attrOffset(...) function. Depending on the datatype of the attribute, we then copy the attribute's datatype and value of the '*value' parameter.

setAttr(...)
--> This function sets the attribute value in the record in the specified schema. The record, schema and attribute number whose data is to be retrieved is passed through the parameter.
--> The data which are to be stored in the attribute is passed by 'value' parameter.
--> We can locate the attribute by using the attOset() function. Then we depend on the attribute's data type, copy the data in the '*value' parameter to the attributes data type and value both.

ADDITIONAL IMPLEMENTATION (Conditional Updates using Scans):
- It takes a condition(expression) based on which the tuples to be updated is selected and a pointer to a function that takes a record as input and returns the updated version of the record.
please run the test_assign3_2.c

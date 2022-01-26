--------Record Manager CS - 525 Assignment No.4-------------
-Xu haoliang  - A20476453
-Dong shansi  - A20466369
-Zou bin - A20467075

RUNNING THE SCRIPT

1) Go to root assign4 by using Terminal.

2) Type command "ls" to list the files and check that the current directory is correct.

3) Type command "make clean" to delete old compiled .o files.

4) Type command "make test1" to compile all project files including "test_assign4_1.c " file

5) Type command "make run_test1" to run "test_assign4_1.c" file.

6) Type command "make test_expr" to compile test expression related files including "test_expr.c".

7) Type command "make run_expr" to run "test_expr.c" file.


SOLUTION DESCRIPTION

project structure

source file
  ***btree_mgr.c
  ***buffer_mgr.c
  ***buffer_mgr_stat.c
  ***dberror.c
  ***storage_mgr.c
  ***record_mgr.c
  ***expr.c
  ***rm_serializer.c

head file
  ***btree_mgr.h
  ***dberror.h
  ***buffer_mgr.h
  ***storage_mgr.h
  ***buffer_mgr_stat.h
  ***record_mgr.h
  ***tables.h

test source file
  ***test_assign4_1.c
  ***test_expr.c

test head file
 ***test_helper.h


compile file
 ***makefile

README file
 ***README.md
 
1. Index Manager Functions
initIndexManager(...)
-->initialize the index manager

shutdownIndexManager(...)
-->Shuts down the index manager

2. B+-tree Functions

createBtree(...)
-->Creat a B-tree

openBtree(...)
-->open the b-tree before a client can access the b-tree index  

closeBtree(...)
-->close the B-tree.
-->Frees all the allocated memory.

deleteBtree(...)
-->Deletes all the record of the mentioned Btree

3. access information about a b-tree

getNumNodes(...)
-->Returns the numbers of nodes in the Btree

getNumEntries(...)
-->Returns the total number of entries in the Btree

getKeyType(...)
-->Returns the key type 

4. Key Functions(index access)

findKey(...)
-->return the RID for the entry with the search key in the b-tree.
-->If the key does not exist this function should return RC_IM_KEY_NOT_FOUND.

insertKey(...)
-->inserts a new key and record pointer pair into the index. 
-->It should return error code RC_IM_KEY_ALREADY_EXISTS if this key is already stored in the b-tree.

deleteKey(...)
-->removes a key (and corresponding record pointer) from the index. 
-->It should return RC_IM_KEY_NOT_FOUND if the key is not in the index.

openTreeScan(...)
-->initialize the scan handle
-->Scans all entries of the tree

nextEntry(...)
-->return RC_IM_NO_MORE_ENTRIES if there are no more entries to be returned

closeTreeScan(...)
-->Closes scanning of tree elements

5. Debug functions
printTree(...) 
-->Create a string representation of a b-tree. 
-->Used in the test cases and can be helpful for debugging.

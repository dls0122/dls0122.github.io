--------Storage Manager CS - 525 Assignment No.1-------------
-Xu haoliang  - A20476453
-Dong shansi  - A20466369
-Zou bin - A20467075

----------------------------------------------------

Running Instructions :

Step 1-  $ make 
Step 2-  $ ./test_assign1


---------------------------------------------------

************************************************************************************
------------------------------------------------------------------------------------


project structure

source file 
  ***storage_mgr.c
  ***dberror.c

head file
  ***dberror.h
  ***storage_mgr.h

test source file
  ***test_assign1_1.c


test head file
 ***test_helper.h

compile file
 ***makefile

README file
 ***README.md
************************************************************************************
------------------------------------------------------------------------------------

Function Description :

1. Fucntions to manipulate page file(Create,Open,Close,Destroy)

a) createPageFile():
Create an empty file with read/write mode, 
if a file with the same name already exists its contents are discarded and the file is treated as a new empty file.
If not, then create a new file with page size 1 and empty inside.
      

b) openPageFile():Opens an existing page file. If the file does not exist, then returns RC_FILE_NOT_FOUND. 
If opening the file is successful, then filling the fields of this file handle with the information
(file name, total number of pages,current page position,mgmtinfo) about the opened file.
     
c) closePageFile():
Close an open page file, if closing the file is successful,returns RC_OK; otherwise returns RC_FILE_NOT_FOUND.   

d) destroyPageFile():
Destroy(delete) a page file, if destroying the file is successful,returns RC_OK; otherwise returns RC_FILE_NOT_FOUND.

************************************************************************************
------------------------------------------------------------------------------------
2. Read functions :

a) readBlock():
	Overview Fucntion: according to the passed pageNum ,fileHandle,memPage read relevant block which in given file.

	Details: 1. check pageNum and fHandle,if the passed pageNum was illegal, then return an abnormality returns code

	         2. open designate file by read only

	         3. compute offset num from the start offset in the file and then seek offset num bytes

	         4. read a pagesize data from the file and update current page position in the file Handle.

	         5. once read commplete the data , then close the file and release applying memeory space.


     
b) getBlockPos():

	Overview Fucntion: according to the passed fileHandle get  position in the block.
    
    Details: 1. check  fHandle,if the passed fileHandle, then return an abnormality returns code

             2. get the position which in the fileHandle

c) readFirstBlock():
   
   Overview Fucntion: get first  block which in given file.

	Details: 1. reuse the readBlock function , passing zero for pageNum ,passing fHandle and memPage;


d) readPreviousBlock():

    Overview Fucntion: get previous  block which base on current block in given file.

	Details: 1. reuse the readBlock function , passing relevant data for pageNum ,passing fHandle and memPage;

	         2. compute currentPageNum , the logic is currentPageNum = fHandle->curPagePos / PAGE_SIZE;

	         3. pass (currentPageNum-1) for pageNum .


e) readCurrentBlock():

	Overview Fucntion: get current block which in given file.

	Details: 1. reuse the readBlock function , passing relevant data for pageNum ,passing fHandle and memPage;

	         2. compute currentPageNum , the logic is currentPageNum = fHandle->curPagePos / PAGE_SIZE;

	         3. pass (currentPageNum) for pageNum .
     

f) readNextBlock():
     

	Overview Fucntion: get next block which base on current block in given file.

	Details: 1. reuse the readBlock function , passing relevant data for pageNum ,passing fHandle and memPage;

	         2. compute currentPageNum , the logic is currentPageNum = fHandle->curPagePos / PAGE_SIZE;

	         3. pass (currentPageNum + 1) for pageNum .

g) readLastBlock():

	Overview Fucntion: get last block which  in given file.

	Details: 1. reuse the readBlock function , passing relevant data for pageNum ,passing fHandle and memPage;

	         2. compute lastPageNum , the logic is lastPageNum = fHandle->totalNumPages -1;

	         3. pass (lastPageNum) for pageNum .


-------------------------------------------------------------------------------------------------------------

3. Write Functions

a) writeBlock():
The data written to memPage is in the block, and the file descriptor comes from fHandle
		Ensure that the capacity is sufficient
		Opens the file, or returns if the file fails to open
		Determine the size of the open file and whether it meets the requirements
		Write data to a file and determine the return value
		Saves the current page location

b)writeCurrentBlock():      
Writes a block from the specified position
     Error code is returned when fHandle is empty
		If curPagePos < 0 is empty, page does not exist. An error code is displayed
		write block

c)appendEmptyBlock():
Add blank block
		Error code is returned when fHandle is empty
		Error code is returned when fileName is empty
		Apply for the corresponding memory
		Opens the file, or returns if the file fails to open
		Writes data to a block,And determine the return value
		TotalNumPages increments 1 on success
		Release the previously allocated memory
    
d)ensureCapacity():
 Ensure that the capacity is sufficient
		Error code is returned when fHandle is empty
		Error code is returned when fileName is empty
		Determine whether the current totalNumPages is greater than the required numberOfPages
		Calculate the size of the request
		Check whether data application is successful
		Opens the file, or returns if the file fails to open
		Writes data to a block,And determine the return value
		Data handling and memory freeing for write errors and successes respectively


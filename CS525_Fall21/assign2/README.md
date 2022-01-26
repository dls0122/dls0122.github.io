--------Storage Manager CS - 525 Assignment No.1-------------
-Xu haoliang  - A20476453
-Dong shansi  - A20466369
-Zou bin - A20467075

----------------------------------------------------

Running Instructions :
to test test_assign2_1.c
Step 1-  $ make test1
Step 2-  $ ./test1

to test test_assign2_2.c
Step 1-  $ make test2
Step 2-  $ ./test2

Note: clock policy in test_assign2_2.c
---------------------------------------------------

************************************************************************************
------------------------------------------------------------------------------------


project structure

source file 
  ***buffer_mgr.c
  ***buffer_mgr_stat.c
  ***dberror.c
  ***storage_mgr.c

head file
  ***dberror.h
  ***buffer_mgr.h
  ***storage_mgr.h
  ***buffer_mgr_stat.h

test source file
  ***test_assign2_1.c
  ***test_assign2_2.c


test head file
 ***test_helper.h

compile file
 ***makefile

README file
 ***README.md
************************************************************************************
------------------------------------------------------------------------------------



************************************************************************************
------------------------------------------------------------------------------------
functions :

a) initBufferPool():creates a new buffer pool with numPages page frames using the page replacement strategy.
The pool is used to cache pages from the page file with name pageFileName. Initially, all page frames are empty. 
This function would not generate a new page file. stratData can be used to pass parameters for the page replacement strategy. 
	

     
b) shutdownBufferPool():destroys a buffer pool. This function frees up all resources associated with buffer pool. 
If the buffer pool contains any dirty pages(with fixcount = 0), then these pages would be written back to disk before destroying the pool. 
	

c) forceFlushPool():causes all dirty pages (with fixcount = 0) from the buffer pool to be written to disk.
   
   

d) markDirty():
Select numPages from BufferPool, and then start traversing PageInfo to see if there is a pagenum (PageHandle -> Pagenum) required, if found set identifier bit to true
    

e) unpinPage():Removes the page page. The pageNum field of page could be used to figure out which page to remove.

	

f) forcePage():
Open the required file, file name stored in bufferPool, write the data stored in pageHandle->data to the file. And write counter increment 1, close file, Select numPages from BufferPool, and then start traversing PageInfo to see if there is a pagenum (PageHandle -> Pagenum) required, if found set identifier bit to true 

	

g) pinPage():

pins the page with page number pageNum. The buffer manager is responsible to set the pageNum field of the page handle passed to the method. Similarly, the data field should point to the page frame the page is stored in (the area in memory storing the content of the page).

h) getFrameContents():
Buffersize is used to apply for heap memory, and the data is iterated, when pagenum equals -1, write NO_PAGE, otherwise pagenum, and finally return the memory pointer.

i) getDirtyFlags():
Follow the supplied NumPages to apply for heap memory and iterate over the data, writing dirty data to heap memory and returning a memory pointer

j) getFixCounts():
Follow the supplied NumPages to apply for heap memory, and iterate over the data, writing fixCount data to heap memory, and returning a memory pointer

k) getNumReadIO():
Get the number of reads, the core principle is to use a variable to count
l):getNumWriteIO():
Get the number of writes, the core principle is to use a variable to count
o):FIFOStrategy():
Eliminate elements according to the first-in first-out rule
p): LRUStrategy():
Eliminate elements according to the least recently used rules
q): CLOCKStrategy():
The clock replacement algorithm can be considered as a recently unused algorithm, that is, the pages that are evicted are the ones that have not been used recently. We set a mark u for each page, u=1 means it has been used recently u=0 means the page has not been used recently and should be evicted
r): LFUStrategy():
That is, the least-access algorithm eliminates data based on the historical frequency of access to the cache. The core idea is "If the data has been accessed very few times in the past period of time, the probability of being accessed in the future will also be very low."


	


-------------------------------------------------------------------------------------------------------------



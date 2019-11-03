Kace Cottam (11609561)\
Maia Whitley (11625970)\
Programming Assignment 5\
CPTS223.01

Programming Assignment 5
===

Scheduler System Design
-----------------------

{insert a figure to explain our final design for data structures - I will make it in Powerpoint}

Runtime Complexities
--------------------

| Function           | Complexity |
|--------------------|------------|
| InsertJob          |            |
| FindShortest       |            |
| DeleteShortest     |            |
| CheckAvailibility  |            |
| RunJob             |            |
| DecrementTimer     |            |
| ReleaseProcs       |            |

Short-Comings of Shortest-Job-First Strategy
--------------------------------------------

One of the major disadvantages of the shortest-job-first strategy is that it's optimizing jobs purely based
on which one takes the least amount of time to run. This overlooks the importance of the number of processors, the length of time
a job has been in the queue, and the importance of a job. For example, a job that was just inserted that takes 4 ticks and uses
20 processors would always be prioritized over a job that has been waiting in the queue, takes 5 ticks, and uses 1 processor. 
This could result in the inefficient use of resources and longer jobs could wait in the queue indefinitely if shorter jobs
continue to be added. 

From a performance standpoint, the shortest-job-first strategy doesn't seem to offer a significant benefit over a 
first-come-first-serve strategy and adds additional time complexity for jobs to percolate within the priority queue as 
opposed to jobs simply being added to the end of a regular queue for a first-come-first-serve approach.

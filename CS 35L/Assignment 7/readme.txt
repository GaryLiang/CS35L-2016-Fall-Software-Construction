In order to make the program into parallel, I first
need to read through the code first to know the structure 
of the program. So I found out there are some functions,
enums and then there is the main function.

On the first few lines in main, the program reads the 
arguements to know how many threads and it seems that
if the thread number is not equal to 1 then the program
will terminate.

To support multithreadings, I include the pthread.h and 
delete the terminate condition when nthread > 1.

There are several issue I ran into:
1. Need to put several data into the do_work function
Since we need to put the whole nested for loop into the 
parallel function, but there are some variables that are
declared outside of the for loop and also the start and
end index that we need to let the parallel function know.
But when we create the thread we can only pass one arguement
into the function. Therefore we need some other ways to pass
in our variables.
I created a 
struct partial{
	int startx;
	int endx;
	scene_t picscene;
}
to contain the data and pass it into parallel function.
To get the data from partial we need to use arrow head
to refer to the data
for example:
struct partial part;
part->startx = 1;
int temp = part->startx;

2. Printing output can't be parallel
The is another issue. As we know the image is being processed 
by several threads. If we print the value of each pixel 
in different thread to the file that may caused problem. I opened
the file and it shows me the output of each thread in every line
instead of printing out the pixels vertically folloing the width
index.
To fix this issue, I create a 3D array
double array[width][height][3]
since for each pixel it has red blue and green
value. Instead of printing the value i the parallel for loop,
I save the value in the parallel function. After the parallel 
function exits and joins the threads, then I print the pixel out.


3. multitasking thread work distribution
This is the issue that takes me some time to figure out.
After fixing the previous issues, I got the almost correct image
except that there are a few lines of pixel at the bottom of 
the image that are totally black and as the thread number increases
the number of black lines increase. So I think some of the pixels
are not being processed, especially the last few lines and that must
be related to the parallelism because I got the full image when 
thread number = 1. So I print out the x start index and end index
for each thread. To my surprise, the program does not finish all 
the work.
The way I separate the work is devide the width by the number of
threads
work_load = width/nthreads
since each thread has index from 0 to n-1, 
I set the start index as
t*work_load
and end index as 
(t+1)*work_load
since the thread number is power of 2, the whole number index that
are being processed are at the power of 2.
because work_load = 16. We cannot assume that the computer can do
cancellation between numerator and denominator
For example:
if we are at thread#8 which is 7 in index
start index will be 7*16 = 112
end index will be 8*16 = 128
and the parallel program ends when it finishes with x index 128.
To fix this issue, I only need the assign the rest of the work 
to the last thread. So when I just add a condition when the program
is assigning the end index for each thread. And that is if the 
current thread is the last thread, assign the rest of work to that 
thread to finish the work.

4.make: *** [check] Error 1
This is the error I constantly get even when I output the 
same correct images. Don't really have clues why. 
I tried to run 
diff -u baseline.ppm 1-test.ppm
and it seems that the whole 1-test.ppm is different from 
baseline.ppm while 1-test.ppm is the same as 2-test.ppm
and the image it produce is correct

Conclusion:
With the parallelism, the efficiency of the program has been improved
almost linearly as the number of thread grows. Even though the check

real    0m42.920s
user    0m42.911s
sys     0m0.003s

real    0m22.795s
user    0m44.542s
sys     0m0.004s

real    0m15.281s
user    0m44.551s
sys     0m0.007s

real    0m9.585s
user    0m44.542s
sys     0m0.000s

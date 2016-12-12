/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/urandom and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "randcpuid.h"
/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  //  printf("Here in main");
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      //  printf("nbytes is %d\n",nbytes);
      // printf("endptr is %s\n",*endptr);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }

  // printf("valid is %d\n",valid);
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;
  //printf("before dlopen\n");
  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  void *dl_handle;
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  char* error;
  //printf("before rand support\n");

  if (rdrand_supported ())
    {
      printf("HW\n");
      dl_handle = dlopen("randlibhw.so", RTLD_LAZY);//RTLD_NOW 
      if(!dl_handle) {
	printf("dlopen() error - %s\n", dlerror()); return 1;
      }
      initialize = dlsym(dl_handle, "hardware_rand64_init"); error = dlerror();
      if(error != NULL) {
        printf("dlsym hardware_rand_init error - %s\n", error); return 1;
      }

      rand64 = dlsym(dl_handle, "rand64"); error = dlerror();
      if(error != NULL) {
	printf("dlsym rand64 error - %s\n", error); return 1;
      }
      //      printf("done hardware rand64\n");
      finalize = dlsym(dl_handle, "hardware_rand64_fini"); error = dlerror();
      if(error != NULL) {
        printf("dlsym software_rand_fini error - %s\n", error); return 1;
      }

    }
  else
    {
      printf("SW\n");
      dl_handle = dlopen("randlibsw.so", RTLD_LAZY);//RTLD_NOW 
      if(!dl_handle) {
	printf("dlopen() error - %s\n", dlerror()); return 1;
      }
      //Calling mul5(&i);
      initialize = dlsym(dl_handle, "software_rand64_init"); error = dlerror();
      if(error != NULL) {
	printf("dlsym software_rand_init error - %s\n", error); return 1;
      }
      rand64 = dlsym(dl_handle, "rand64"); error = dlerror();
      if(error != NULL) {
        printf("dlsym rand64 error - %s\n", error); return 1;
      }
      finalize = dlsym(dl_handle, "software_rand64_fini"); error = dlerror();
      if(error != NULL) {
        printf("dlsym software_rand_fini error - %s\n", error); return 1;
      } 
    }
  initialize();
  // printf("after initialize\n");
  long long wordsize = sizeof rand64 ();
  int output_errno = 0;
  //printf("word size is %d\n",wordsize);
  
  //num8 = nbytes/wordsize;
  //  remainder = nbytes%wordsize;
  int num8 = 0;
  int remainder = 0;
  num8 = nbytes/wordsize;
  remainder = nbytes%wordsize;


  do
    {
      //  printf("In while loop\n");
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      // printf("x is %llu \n",);     
      int temp = 1;
      long long i = 0;
      //      printf("num8 is %d\n",num8);
      if(num8 == 0)
	{
      
	  while(i < outbytes)
	    {
	      temp =temp*10;
	      i++;
	    }
	  printf("%llu\n",x%temp);
	}
      else
	{
	  //  printf("greater than 8\n");
	  i = 0;
	  while(i < num8)
	    {
	      temp = 1;
	      x = rand64 ();
	      int j =0;
	      while(j < 8)
		{
		  temp =temp*10;
		  j++;
		}
	      printf("%llu",x%temp);
	      i++;
	    }
	  i = 0;
	  x = rand64();
	  temp = 1;
	  while(i < remainder)
	    {
	  
                  temp =temp*10;
                  i++;
               
	    }
	  if( remainder !=0)
	    printf("%llu",x%temp);
	}
      printf("\n");
      //if (fwrite (&x, 8, outbytes, stdout) != outbytes)
      //{
      //output_errno = errno;
      //break;
     
      nbytes -= outbytes;
      //printf("bytes left %d\n",nbytes);
    }
  while (0 < nbytes);
  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      finalize ();
      return 1;
    }

  //  finalize ();
  dlclose(dl_handle);
  return 0;
}

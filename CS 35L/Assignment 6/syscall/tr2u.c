#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  char* from = argv[1];
  char* to = argv[2];

  if(from == NULL || to == NULL)
    {
      fprintf(stderr,"Not enough argument \n");
      exit(1);
    }

  int from_length = strlen(from);
  int to_length = strlen(to);

  if(from_length != to_length)
    {
      fprintf(stderr,"from and to should be the same length \n");
      exit(1);
    }

  for(int i = 0; i < from_length;i++)
    {
      if(i == from_length-1)
	break;
      for(int j = i+1; j < from_length; j++)
	{
	  if(*(from+i) == *(from+j))
	    {
	      fprintf(stderr,"same characters in from");
	      exit(1);
	    }

	  //printf("*(from+i) is %c %d \n",*(from + i),(int)(*(from+i)));

	  if((int)(*(from+i)) == 92 || (*(from+i)) == '[' 
		  || (int)(*(from+i)) == 44 ||(int)(*(from+i)) == 45 )
	    {
	      fprintf(stderr,"invalid character");
	      exit(1);
	    }
	}
    }

  char input;
  char* input_buffer = malloc(1);
  void* output_buffer = malloc(1);
  while(read(0,input_buffer,1))
    {
      input = *input_buffer;
      char output;
      int valid = 0;

      if( input == EOF)
	break;

      for(int i = 0;i < from_length; i++)
	{
	  if(input == *(from+i))
	    {
	      valid = 1;
	      output = *(to+i);
	      output_buffer = &output;
	      break;
	    }
	}
      if(valid == 0)
	output_buffer = &input;

      write(1,output_buffer,1);
    }

  
}

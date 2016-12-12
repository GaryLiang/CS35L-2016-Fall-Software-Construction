#include <stdio.h>
#include <stdlib.h>

int frobcmp(void const* a, void const* b)
{
  int diff = 0;
  int mask = 42;

  char* a_p;
  char* b_p;

  a_p = *(char**)a;
  b_p = *(char**)b;

  for(int i = 0;; i++)
    {
      if(a_p[i]==" " && b_p[i]==" ")
	{
	  diff = 0;
	  break;
	}
      else if(a_p[i]==" ")
	{
	  diff = 0 - b_p[i];
	  break;
	}
      else if(b_p[i]==" ")
	{
	  diff = a_p[i];
	  break;
	}
      else if(a_p[i]!=b_p[i])
	{
	  diff = (a_p[i]^mask)-(b_p[i]^mask);
	  break;
	}
    }
  return diff;
}


int main()
{
  int Basic_Size = 1000;
  int Basic_Line_Length = 100;
  int Actual_Size;
  int Num_Line = 0;
  int tempchar = 0;
  int char_index = 0;
  char **Line;

  Line = (char**)malloc(Basic_Size);

  Actual_Size = Basic_Size;

  int temp_length = Basic_Line_Length;

  while(tempchar = getchar())
    {
      if(tempchar == -1)
	  break;
	
      if(tempchar == 32)
	{
	  Num_Line++;
	  char_index = 0;
	  if(Num_Line == Basic_Size)
	    {
	      Line = (char**)realloc(Line,Basic_Size+100);
	      if(Line == NULL)
		{
		  fprintf(stderr,"No memory \n");
		  exit(1);
		}
	      temp_length = Basic_Line_Length;
	      Basic_Size = Basic_Size + 100;
	    }
	    //reallocate memory for line number
	}
      else
	{
	  if(char_index == 0)
	    Line[Num_Line] = (char*)malloc(temp_length);
	  Line[Num_Line][char_index] = (char*)tempchar;
	  char_index++;
	  if(char_index == temp_length)
	    {
	      Line[Num_Line] = (char*)realloc(Line[Num_Line],temp_length+10);
	      if(Line[Num_Line] == NULL)
                {
                  fprintf(stderr,"No memory \n");
                  exit(1);
                }
	      temp_length = temp_length + 10;
            }
	    //reallocate memory for that line
	}
    }
  Num_Line++;
  
  qsort(Line,Num_Line,sizeof(char*),frobcmp);
  for(int i = 0; i < Num_Line; i++)
    {
      printf("%s ",Line[i]);
    }
  return 0;
}

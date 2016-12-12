#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int num_cmp = 0;

int frobcmp(void const* a, void const* b)
{
  num_cmp++;
  int diff = 0;
  int mask = 42;

  char* a_p;
  char* b_p;

  a_p = *(char**)a;
  b_p = *(char**)b;
  // printf("a is %c \nb is %c \n",a_p[0],b_p[0]);

  for(int i = 0;; i++)
    {
      if(a_p[i]==' ' && b_p[i] == ' ')
	{
	  diff = 0;
	  break;
	}
      else if(a_p[i]==' ')
	{
	  diff = 0 - b_p[i];
	  break;
	}
      else if(b_p[i]==' ')
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


int main(int argc, char* argv[])
{
  struct stat fileStat;
  fstat(0,&fileStat);
  int init_word_size = 100;
  int init_ptr_size = 100;
  int is_file = 0;
  int is_word_full = 0;
  int is_ptr_full = 0;

  //printf("Information for %s\n",argv[1]);
  //printf("---------------------------\n");
  //printf("File Size: \t\t%d bytes\n",fileStat.st_size);
  //printf("Number of Links: \t%d\n",fileStat.st_nlink);
  //printf("File inode: \t\t%d\n",fileStat.st_ino);

  char** all = malloc(init_ptr_size);
  //printf("initial size of all is %d \n",sizeof(all));
  char* file;
  if(fileStat.st_size > 0)
    {
      //  printf("file \n");
      file = (char*)malloc(fileStat.st_size);
      read(0,file,fileStat.st_size);
      is_file = 1;
    }
  else
    {
      file = (char*)malloc(init_word_size);
      is_file = 0;
    }


  char* current_byte = malloc(1);
  int current_line = 0;
  int current_word_index = 0;
  int vertical_size = init_ptr_size;
  int word_size = init_word_size;
  int max_word_size =init_word_size;
  char** temp_all;

  // printf("here 1111111111111111111\n");
  if(is_file == 0)
    {
      //  printf("here 2222222222222222\n");
      while(read(0,current_byte,1) > 0)
	{
	  char temp = *current_byte;
	  //all[current_line] = malloc(init_word_size);
	  // all[current_line][current_word_index] = temp;
	  if(temp == ' ')
	    {
	      all[current_line][current_word_index] = temp;
	      current_line++;
	      current_word_index = 0;
	      word_size = init_word_size;
	    }
	  else
	    {
	      if(current_word_index == 0)
	  	all[current_line] = malloc(init_word_size);
	      all[current_line][current_word_index] = temp;
	      current_word_index++;
	    }
	  if(current_line == vertical_size)
	    {
	      if(all = realloc(all,100*(vertical_size++)))
		{
		 
		}
	      else
		{
		  fprintf(stderr,"Error");
		  exit(1);
		}
	    }
	  if(current_word_index == word_size)
	    {
	      if(all[current_line] = realloc(all[current_line],100*(word_size++)))
		{
		  if(max_word_size < word_size*100)
		    max_word_size = word_size*100;
		  //printf("not enough");
		}
	      else
		{
		  fprintf(stderr,"Error");
		  exit(1);
		}
	    }
	}
      for(int i = 0; i < max_word_size; i++)
	{
	  // printf("%c \n",all[current_line][i]);
	  if(all[current_line][i] == '\0')
	    {
	      // printf("here haha");
	      all[current_line][i] == ' ';
	      break;
	    }
	}
      
    }    
  else
    {
      for(int i =0; i < fileStat.st_size;i++)
	{
	  char temp = file[i];
    
      

          if(temp == ' ')
            {
              all[current_line][current_word_index] = temp;
              current_line++;
              current_word_index = 0;
            }
          else
            {
              if(current_word_index == 0)
                all[current_line] = malloc(init_word_size);
	      
              all[current_line][current_word_index] = temp;
              //printf("current char is %c, current line is %d \n", all[current_line][current_word_index],current_line);
	      if(i+1 == fileStat.st_size && temp != ' ')
		all[current_line][current_word_index] = ' ';
	      current_word_index++;
            }

          if(current_line == vertical_size)
            {
              if(all = realloc(all,100*(vertical_size++)))
                {
                  //printf("not enough lines");
                }
              else
                {
                  fprintf(stderr,"Error");
		  exit(1);
                }
            }

          if(current_word_index == word_size)
            {
              if(all[current_line] = realloc(all[current_line],100*(word_size++)))
                {
		  if(max_word_size < word_size*100)
		    max_word_size = word_size*100;
		  //          printf("not enough");
                }
	      else
		{
		  fprintf(stderr,"Error");
		  exit(1);
		}
            }
       

	}
    }
  current_line++;
  qsort(all,current_line,sizeof(char*),frobcmp);
  for(int i =0; i < current_line; i++)
    {
      for(int j =0; j < max_word_size;j++)
	{
	  write(1,(all[i]+j),sizeof(char));
	  if(*(all[i]+j) == ' ')
	    break;
	}
    }

  for(int i = 0;i < current_line;i++)
    free(all[i]);

  free(all);

  fprintf(stderr, "Comparisons: %d\n",num_cmp);
  return 0;
}

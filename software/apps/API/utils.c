#include <stdio.h>
#include <stdint.h>

#include "utils.h"

void print_hex_info(const char* title,int calling_line_num,const unsigned char* hex_info,unsigned int info_size)
{
   #define COLUMN_NUM 16
   unsigned int i;
   printf("from \"%s\"@%d: size=%d\n",title,calling_line_num,info_size);

   for(i=0;i<info_size;i++)
   {
      if(i%COLUMN_NUM==0)
        printf("<%03d>",i/COLUMN_NUM+1); //print line num
      printf("%02X%c",hex_info[i],(i==(info_size-1)||(i+1)%COLUMN_NUM==0)?'\n':',');
   }
}

//return: 1 succefully
//        -1 failed
int str_to_2_hex(char* str,uint8_t *hex)
{
  if(sscanf(str,"%hhx",hex)!=1)
  {
    fprintf(stderr,"invalid %s\n",str);
    return -1;
  }
  return 1;
}

//return: 1 succefully
//        -1 failed
int str_to_8_hex(char* str,uint32_t *hex)
{
  if(sscanf(str,"%x",hex)!=1)
  {
    fprintf(stderr,"invalid %s\n",str);
    return -1;
  }
  return 1;
}

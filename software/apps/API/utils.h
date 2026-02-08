#ifndef __UTILS_H__
#define __UTILS_H__

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG(fmt, args...) printf("@%s:%d, " fmt,__FUNCTION__,__LINE__,## args)
#else //!DEBUG_MODE
#define DEBUG(fmt, args...)
#endif //DEBUG_MODE

#define ERR_LOG(fmt, args...) fprintf(stderr,"@%s:%d, " fmt,__FUNCTION__,__LINE__,## args)
void print_hex_info(const char* title,int calling_line_num,const unsigned char* hex_info,unsigned int info_size); 
int str_to_2_hex(char* str,uint8_t *hex);
int str_to_8_hex(char* str,uint32_t *hex);
#endif //__UTILS_H__

#ifndef __DEBUG_H__
#define __DEBUG_H__
#define NAME_ONLY(FILE_NAME) (strrchr(__FILE__,'/')?strrchr(__FILE__,'/')+1:__FILE__)
#define critical_log(fmt,args...) printk(KERN_ALERT "JIN.%s@%s@%d: "fmt,NAME_ONLY(__FILE__),__FUNCTION__,__LINE__,##args)
#define watchme critical_log
#endif// __DEBUG_H__


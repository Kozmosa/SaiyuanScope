#ifndef __LCDFONT_H
#define __LCDFONT_H 	   

typedef struct 
{
	unsigned char Index[2];	
	unsigned char Msk[32];
//	unsigned char Msk[24];
}typFNT_GB16; 

extern const unsigned char ascii_1608[][16];
extern const unsigned char ascii_3216[][64];
extern const typFNT_GB16 tfont16[6];

#endif



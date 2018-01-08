#ifndef __TM16XX_H__
#define __TM16XX_H__

extern unsigned char TM16XX[16];

void TM16XX_Init(unsigned char st);
void TM1624_Write_Data(unsigned char* buf,unsigned char len);
void TM1640_Write_Data(unsigned char* buf,unsigned char len);

#endif

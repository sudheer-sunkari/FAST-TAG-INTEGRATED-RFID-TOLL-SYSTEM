//primary lcd drivers
void writeLCD(unsigned char dat);
void cmdLCD(unsigned char cmd);
void charLCD(unsigned char asciival);
void initLCD(void);
//seccondary LCD drivers
void strLCD(char*s);
void u32LCD(unsigned int n);
void s32LCD(int n);
void f32LCD(float f,unsigned char nDP);
void binLCD(unsigned int n,unsigned char nBD);
void hexLCD(unsigned int n);
void octLCD(unsigned int n);
void buildCGRAM(unsigned char *p,unsigned char nbytes);
void RTC_Init(void);
void RTC_SetTimeInfo(unsigned int hour, unsigned int min, unsigned int sec,unsigned int dom,unsigned int month,unsigned int year,char* dow);

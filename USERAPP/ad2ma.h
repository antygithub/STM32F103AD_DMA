#ifndef _AD2MA_H_
#define _AD2MA_H_

#include "main.h"

//A1 10765
//A2 34447
//A3 58060
//K1 0.337809
//K2 0.337865
#define A1 10765
#define A2 34447
#define A3 58060
#define K1 337.809f
#define K2 337.865f






uint16_t Collect_OnceAD(void);
float AD2mA(void);	//x10000uA	
uint32_t AD2mV(void);	//x100uV
uint32_t AD2PPM(void);	//x100000PPM

#endif


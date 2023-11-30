#ifndef _USER_IO_H_
#define _USER_IO_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ParseFat.h"

void ActionMenu(int16_t * UserInput);
uint8_t IO_Print_Folder(uint32_t StartFolderSec);
uint8_t IO_Print_Root();
void IO_ReadDataFile(uint32_t FirstClus);
void IO_GetData();

#endif	/* _USER_IO_H_ */

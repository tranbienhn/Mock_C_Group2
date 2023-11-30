#include "ParseFat.h"
#define SET_ADDRESS(Addr)		fseek(FATptr, Addr, SEEK_SET)
#define BYTE_PER_SEC	512
#define BYTE_PER_ENTRY	32

static FILE * FATptr = NULL;
static Boot_t s_Boot;
static uint32_t NumOfFatSec;
static uint32_t NumOfRootSec;
static DataBoot_t s_Data_Boot;
static Entry_t s_Entry;

FATStatus_t FAT_OpenFile(const char * filename)
{
	FATStatus_t status = FAT_OK;
	FATptr = fopen(filename, "rb");

	if (NULL == FATptr)
	{
		status = FAT_NOT_EXIST;
	}
	return status;
}

void FAT_CloseFile()
{
	fclose(FATptr);
}

void Read_Boot()
{
	SET_ADDRESS(0x0);
	fread(&s_Boot, 1, sizeof(Boot_t), FATptr);
}

DataBoot_t FAT_CalculateDataFromBoot()
{
	s_Data_Boot.StartFatAddr = s_Boot.ReservedSec * s_Boot.BytePerSec;									// 0x200
	NumOfFatSec = s_Boot.NumOfFat * s_Boot.SecPerFat;													// 18
	s_Data_Boot.StartRootAddr = s_Data_Boot.StartFatAddr + (NumOfFatSec * s_Boot.BytePerSec);			// 0x2600
	NumOfRootSec = (sizeof(Entry_t) * s_Boot.EntryOfRoot) / s_Boot.BytePerSec;							// 14
	s_Data_Boot.StartDataAddr = s_Data_Boot.StartRootAddr + ((NumOfRootSec - 2) * s_Boot.BytePerSec);	// 0x3E00
	s_Data_Boot.BytePerSec = s_Boot.BytePerSec;															// 512
	s_Data_Boot.EntryOfRoot = s_Boot.EntryOfRoot;														// 224
	
	return s_Data_Boot;
}

Entry_t Read_Entry(uint32_t Address)
{
	SET_ADDRESS(Address);
	fread(&s_Entry, 1, sizeof(Entry_t), FATptr);
	return s_Entry;
}

void Read_Data_Sector(uint32_t Address, uint8_t* Buff)
{
	SET_ADDRESS(Address);
	fread(Buff, 1 , s_Data_Boot.BytePerSec, FATptr);
}

uint32_t Find_NextClus(uint32_t CurrentClus)
{
	uint8_t Buff[2];
	uint32_t Addr = 0;
	uint32_t NextClus = 0;

	Addr = s_Data_Boot.StartFatAddr + (CurrentClus + (CurrentClus >> 1));
	SET_ADDRESS(Addr);

	fgets(Buff, 3, FATptr);

	if ((CurrentClus % 2) == 0)
	{
		NextClus = Buff[0] | ((Buff[1] & 0xF) << 8);
	}
	else
	{
		NextClus = ((Buff[0] & 0xF0) >> 4) | (Buff[1] << 4);
	}
	return NextClus;
}

uint32_t Find_SelectedRootEntryAddr(uint16_t UserInput)
{
	uint8_t AttributesBuff;
	uint32_t EntryOffset = 0;
	uint8_t check = 0;
	uint32_t Address;
	
	while(check != UserInput)
	{
		SET_ADDRESS(s_Data_Boot.StartRootAddr + 0x0B + EntryOffset);
		AttributesBuff = fgetc(FATptr);
		if(AttributesBuff != 0x0F)
		{
			check++;
		}
		EntryOffset += sizeof(Entry_t);
	}
	Address = s_Data_Boot.StartRootAddr + EntryOffset - sizeof(Entry_t);
	
	return Address;
}

uint32_t Find_SelectedFolderEntryAddr(uint16_t UserInput, uint32_t StartClus)
{
	uint8_t AttributesBuff;
	uint32_t EntryOffset = 64;
	uint8_t check = 0;
	uint32_t NextClus = 0;
	uint32_t Address = 0;
	
	NextClus = StartClus;
	
	while(check != UserInput && NextClus != 0xFFF)
	{
		Address = s_Data_Boot.StartDataAddr + NextClus*s_Data_Boot.BytePerSec;
		
		while(check != UserInput && EntryOffset < s_Data_Boot.BytePerSec)		// read 1 clus
		{
			SET_ADDRESS(Address + 0x0B + EntryOffset);
			AttributesBuff = fgetc(FATptr);
			if(AttributesBuff != 0x0F)
			{
				check++;
			}
			EntryOffset += sizeof(Entry_t);
		}
		if(check != UserInput)
		{
			EntryOffset = 0;
			NextClus = Find_NextClus(NextClus);
		}
	}
	
	Address += EntryOffset - sizeof(Entry_t);
	
	return Address;
}

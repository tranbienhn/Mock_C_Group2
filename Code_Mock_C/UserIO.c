#include "UserIO.h"

static Entry_t s_Entry;
static DataBoot_t s_Data;
static uint32_t BytePerSec;

void IO_GetData()
{
	s_Data = FAT_CalculateDataFromBoot();
}

void IO_Print_Header()
{
	printf(" ================================================= FAT 12 ==========================================================\n\n");
	printf("\t");
	printf("%s","| Name |");
	printf("\t%s","| Ext |");
	printf("\t\t%s","| Type |");
	printf("\t%s","| Date modified |");
	printf("\t%s","| Time modified |");
	printf("\t%s\n","| Size |");
}

void ActionMenu(int16_t * UserInput)
{
	printf("\n =================================================== END ===========================================================\n");
	printf(" BACK : 0\n");
	printf(" EXIT PROGRAM : -1\n");
	printf(" ENTER YOUR ACTION : ");
	scanf("%d", UserInput);
}

void IO_Print_EntryInfo(uint8_t FileNum)
{
	uint8_t i = 0;
	printf("%-6s", "");
	printf("%-3d", FileNum);
	for (i = 0; i<8; i++)
	{
		printf("%c", s_Entry.Name[i]);
	}
	if(0x0 == s_Entry.Attributes)		// File Info
	{
		printf("%-9s", "");
		printf("%-16s", s_Entry.Extension);
		printf("%-17s", "File");
		printf("%.2d/%.2d/%.4d         ", s_Entry.Date_v.Date.Day, s_Entry.Date_v.Date.Month, s_Entry.Date_v.Date.Year+1980);
		printf("%-6s", "");
		printf("%.2d:%.2d:%.2d       ", s_Entry.Time_v.Time.Hour, s_Entry.Time_v.Time.Minute, s_Entry.Time_v.Time.Second);
		printf("%-8s", "");
		printf("%d bytes", s_Entry.Size);
		
	}
	else		// Folder Info
	{
		printf("%-25s", "");
		printf("%-17s", "Folder");
		printf("%.2d/%.2d/%.4d         ", s_Entry.Date_v.Date.Day, s_Entry.Date_v.Date.Month, s_Entry.Date_v.Date.Year+1980);
		printf("%-6s", "");
		printf("%.2d:%.2d:%.2d       ", s_Entry.Time_v.Time.Hour, s_Entry.Time_v.Time.Minute, s_Entry.Time_v.Time.Second);
	}
	printf("\n");
}

uint8_t IO_Print_Folder(uint32_t StartFolderSec)
{
	uint32_t EntryOffset = sizeof(Entry_t) * 2;
	uint8_t FileNum = 0;
	uint32_t CurrentAddress = 0;
	uint32_t NextClus = 0;
	
	system("cls");
	IO_Print_Header();
	NextClus = StartFolderSec;

	while(NextClus != 0xFFF)			// break when meet last cluster
	{
		CurrentAddress = s_Data.StartDataAddr + NextClus*s_Data.BytePerSec;
		
		do
		{
			s_Entry = Read_Entry(CurrentAddress + EntryOffset);
			if(s_Entry.Name[0] != 0)
			{
				if(s_Entry.Attributes != 0x0F)										// if not a long name entry
				{
					FileNum++;
					IO_Print_EntryInfo(FileNum);
				}
			}
			EntryOffset += sizeof(Entry_t);
		} while((s_Entry.Name[0] != 0) && (EntryOffset < s_Data.BytePerSec));		// break when meet zero entry or finish read all root sectors

		NextClus = Find_NextClus(NextClus);
		EntryOffset = 0;
	}
	return FileNum;
}

uint8_t IO_Print_Root()
{
	uint32_t EntryCount = 0;
	uint32_t EntryOffset = 0;
	uint8_t FileNum = 0;
	
	system("cls");
	IO_Print_Header();
	do
	{
		s_Entry = Read_Entry(s_Data.StartRootAddr + EntryOffset);
		if(s_Entry.Name[0] != 0)
		{
			if(s_Entry.Attributes != 0x0F)										// if not a long name entry
			{
				FileNum++;
				IO_Print_EntryInfo(FileNum);
			}
			EntryOffset += sizeof(Entry_t);
		}
		EntryCount++;
	} while((s_Entry.Name[0] != 0) && (EntryCount < s_Data.EntryOfRoot));		// break when meet zero entry or finish read all root sectors
	return FileNum;
}

void IO_ShowDataFile(uint32_t AddressData)
{
	uint32_t i = 0;
	uint8_t Buff[s_Data.BytePerSec];
	
	Read_Data_Sector(AddressData, Buff);
	for (i=0; i < s_Data.BytePerSec; i++)
	{
		if(Buff[i] != 0x0)
		{
			printf("%c", Buff[i]);
		}
	}
}

void IO_ReadDataFile(uint32_t FirstClus)
{
	uint32_t AddressData = 0;
	uint32_t NextClus = 0;

	NextClus = FirstClus;
	while (NextClus != 0xFFF)
	{
		AddressData = s_Data.StartDataAddr + NextClus * s_Data.BytePerSec;
		IO_ShowDataFile(AddressData);
		NextClus = Find_NextClus(NextClus);
	}
}

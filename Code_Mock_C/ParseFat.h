#ifndef _PARSE_FAT_H_
#define _PARSE_FAT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

// Macro use for Boot Info

#define BS_PART_OF_BOOTSTRAP			3U
#define BS_OMD							8U

#define BS_TOTAL_SECTOR_IN_FILE_SYSTEM	2U
#define BS_MEDIA_DISCRIP				1U

#define BS_SECTOR_PER_TRACK				2U
#define BS_HEAD							2U
#define BS_HIDDEN_SECTOR				4U
#define BS_TOTAL_SECTOR					4U
#define BS_PHYSICAL_DRIVE_NUMBER		2U
#define BS_EXTENDED_BOOT_SIGNATURE		1U
#define BS_VOLUME_SERIAL_NUMBER			4U
#define BS_VOLUME_LABEL					11U
#define BS_NAME_OF_FAT					8U
#define BS_BOOTSTRAP_REMAIN				448U
#define BS_SIGNATURE					2U

typedef struct
{
	uint8_t partBootstrap[BS_PART_OF_BOOTSTRAP];
	uint8_t omd[BS_OMD];
	uint16_t BytePerSec;
	uint8_t SecPerCluster;
	uint16_t ReservedSec;
	uint8_t  NumOfFat;
	uint16_t EntryOfRoot;
	uint8_t SecInFS[BS_TOTAL_SECTOR_IN_FILE_SYSTEM];
	uint8_t mediaDescrip[BS_MEDIA_DISCRIP];
	uint16_t SecPerFat;
	uint8_t SecPerTrack[BS_SECTOR_PER_TRACK];
	uint8_t head[BS_HEAD];
	uint8_t hiddenSec[BS_HIDDEN_SECTOR];
	uint8_t totalSec[BS_TOTAL_SECTOR];
	uint8_t physicalDriveNumber[BS_PHYSICAL_DRIVE_NUMBER];
	uint8_t extendedBootSignature[BS_EXTENDED_BOOT_SIGNATURE];
	uint8_t volumeSerialNumber[BS_VOLUME_SERIAL_NUMBER];
	uint8_t volumeLabel[BS_VOLUME_LABEL];
	uint8_t fileSystemType[BS_NAME_OF_FAT];
	uint8_t bootstrapRemain[BS_BOOTSTRAP_REMAIN];
	uint8_t signature[BS_SIGNATURE];
} Boot_t;

// Macro Entry
#define NAME_LEN		8
#define EXTENSION_LEN	3
#define RESERVED_LEN	10

typedef union
{
	struct {
		uint16_t Second	: 5;
		uint16_t Minute	: 6;
		uint16_t Hour	: 5;
	} Time;
	uint16_t Data;
} TimeType;

typedef union
{
	struct {
		uint16_t Day	: 5;
		uint16_t Month	: 4;
		uint16_t Year	: 7;
	} Date;
	uint16_t Data;
} DateType;

typedef struct
{
	uint8_t Name[NAME_LEN];
	uint8_t Extension[EXTENSION_LEN];
	uint8_t Attributes;
	uint8_t Reserved[RESERVED_LEN];
	TimeType Time_v;
	DateType Date_v;
	uint16_t StartClus;
	uint32_t Size;
} Entry_t;

typedef struct
{
	uint16_t BytePerSec;
	uint32_t StartFatAddr;
	uint32_t StartRootAddr;
	uint32_t StartDataAddr;
	uint16_t EntryOfRoot;
} DataBoot_t;

typedef enum
{
	FAT_OK				= 0x0U,
	FAT_NOT_EXIST
} FATStatus_t;

FATStatus_t FAT_OpenFile(const char * filename);
void Read_Boot();
DataBoot_t FAT_CalculateDataFromBoot();
Entry_t Read_Entry(uint32_t Address);
uint32_t Find_NextClus(uint32_t CurrentClus);
void Read_Data_Sector(uint32_t Address, uint8_t* Buff);

uint32_t Find_SelectedRootEntryAddr(uint16_t UserInput);
uint32_t Find_SelectedFolderEntryAddr(uint16_t UserInput, uint32_t StartClus);

#endif	/* _PARSE_FAT_H_ */

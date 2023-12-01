#include "UserIO.h"
#include "Linked_List.h"


int main(int argc, char *argv[])
{
	DataBoot_t s_Data;
	Entry_t s_Entry;
	int16_t UserInput = 0;
	uint32_t CurrentAddr = 0;
	uint16_t FileNum = 0;
	uint32_t Address = 0;
	
	FATStatus_t status = FAT_OK;
	status = FAT_OpenFile(argv[1]);
	
//	status = FAT_OpenFile("floppy.img");
	
	if(FAT_OK == status)
	{
		Read_Boot();
		FAT_CalculateDataFromBoot();
		IO_GetData();
		
		s_Data = FAT_CalculateDataFromBoot();
		
		//Add Root Node
		List_Address_Add_Node(s_Data.StartRootAddr);
	
		do
		{
			CurrentAddr = List_Address_GetAddr();
			
			if(CurrentAddr == s_Data.StartRootAddr)
			{
				FileNum = IO_Print_Root();
			}
			else
			{
				FileNum = IO_Print_Folder(CurrentAddr);
			}
			
			// Choose Action
			do
			{
				ActionMenu(&UserInput);
				if(UserInput < -1 || UserInput > FileNum)
				{
					printf("Range of Input : [-1 ; %d]\nChoose again!", FileNum);
				}
			} while(UserInput < -1 || UserInput > FileNum);

			if(-1 != UserInput)
			{
				if(0 == UserInput)
				{
					if(CurrentAddr != s_Data.StartRootAddr)
					{
						List_Address_DelNode();
					}
				}
				else
				{
					// Find and read selected entry
					if(CurrentAddr == s_Data.StartRootAddr)
					{
						Address = Find_SelectedRootEntryAddr(UserInput);
					}
					else
					{
						Address = Find_SelectedFolderEntryAddr(UserInput, CurrentAddr);
					}
					s_Entry = Read_Entry(Address);
					// Save parent address if user open a File
					if(0x10 != s_Entry.Attributes)
					{
						printf("\n ----------------------Contents of the file -------------------------\n\n");
						IO_ReadDataFile(s_Entry.StartClus);
						printf("\n\n --------------------------End of File -----------------------------\n");
						printf("\nPress ENTER to close file ");
						getchar();
					}
					else
					{
						List_Address_Add_Node(s_Entry.StartClus);
					}	
				}
			}
			getchar();
		} while(UserInput != -1);

		FAT_CloseFile();
		List_free();
		printf("End Program!");
	}
	else
	{
		printf("File not exist! Retry!");
	}
}

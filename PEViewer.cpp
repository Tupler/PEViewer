// PeViewer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/**
* author:tupler
* github:@tupler
* description:PE学习写的不好
*/
#include <iostream>
#include <windows.h>
DWORD numberofsection;
PIMAGE_SECTION_HEADER secion_table;
DWORD RVA2RAW(DWORD RVA);
int main(int argc,char* argv[])
{
	char* contents=0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwReadSize = 0;
	hFile = CreateFileA(argv[1], GENERIC_ALL, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 分配内存用于存放读取的文件数据
	contents = (PCHAR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	ReadFile(hFile, contents, dwFileSize, &dwReadSize, NULL);
	CloseHandle(hFile);
	int filesize = dwFileSize;
	PIMAGE_DOS_HEADER dos_Header = (PIMAGE_DOS_HEADER)contents;
	if (dos_Header->e_magic!= IMAGE_DOS_SIGNATURE)
	{
		printf("it's not pe1");
		return 0;
	}
	// 0x1234 -> contents    contents=0xdeadbeef  0xdeadbeef->DOS_header
	PIMAGE_NT_HEADERS64 nt_Header64 = (PIMAGE_NT_HEADERS64)((DWORD)contents + dos_Header->e_lfanew);
	PIMAGE_NT_HEADERS nt_Header32 = (PIMAGE_NT_HEADERS)((DWORD)contents + dos_Header->e_lfanew);
	if (nt_Header32->Signature!=IMAGE_NT_SIGNATURE || nt_Header64->Signature != IMAGE_NT_SIGNATURE)
	{
		printf("it's not pe2");
		return 0;
	}
	IMAGE_FILE_HEADER file_Header = (IMAGE_FILE_HEADER)nt_Header32->FileHeader;
	numberofsection = file_Header.NumberOfSections;
	DWORD imageBase=0;
  //判断文件运行平台
	if (file_Header.Machine== IMAGE_FILE_MACHINE_I386)
	{
		IMAGE_OPTIONAL_HEADER32 optional_Header =(IMAGE_OPTIONAL_HEADER32)nt_Header32->OptionalHeader;
		printf("+[machine]:%s\n", "x86");
		printf("+[入口点]:0x%x\n", optional_Header.AddressOfEntryPoint);
		printf("+[基地址]:0x%x\n", optional_Header.ImageBase);
		imageBase = optional_Header.ImageBase;
		printf("+[文件大小]:0x%x\n", optional_Header.SizeOfImage);
		//节表
		secion_table = (PIMAGE_SECTION_HEADER)((DWORD)nt_Header32 + 24 + file_Header.SizeOfOptionalHeader);
		printf("+[节数量]:%d\n", numberofsection);
		printf("+[节名称]\n");
		for (int i = 0; i < numberofsection; i++)
		{
			printf("+[%d]:%s\n",i,secion_table[i].Name);
		}
		int temp='y';
		printf("+[输入] 是否要获取导出表[y/n]:");
		scanf_s("%c",&temp);
		//是否有导出表 
		if (optional_Header.DataDirectory[0].VirtualAddress!=0&&temp=='y') {
			//导出表的RAW
			DWORD eatoffset = RVA2RAW(optional_Header.DataDirectory[0].VirtualAddress);
			//RAW加上imagebase 因为是直接读文件并没有映射为内存
			PIMAGE_EXPORT_DIRECTORY eat = (PIMAGE_EXPORT_DIRECTORY)((DWORD)dos_Header + eatoffset);
			printf("+[dllName]:%s\n", (DWORD)dos_Header + RVA2RAW(eat->Name));
			printf("+[导出函数]\n");
			PDWORD functionanems = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNames));
			PWORD addressOfNameOrdinals = (PWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNameOrdinals));
			PDWORD addressOfFunction = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfFunctions));
			for (int i = 0; i < eat->NumberOfNames; i++)
			{
				WORD ordinalsIndex = addressOfNameOrdinals[i];
				printf("+[序号]:%d[RVA]:0x%x[名称]:%s\n", ordinalsIndex, addressOfFunction[ordinalsIndex],RVA2RAW(functionanems[i]) + DWORD(dos_Header));
				
			}
		}                  
	}
	else if (file_Header.Machine == IMAGE_FILE_MACHINE_AMD64)
	{
		IMAGE_OPTIONAL_HEADER64 optional_Header = (IMAGE_OPTIONAL_HEADER64)nt_Header64->OptionalHeader;
		printf("+[machine]:%s\n", "x64");
		printf("+[入口点]:0x%x\n", optional_Header.AddressOfEntryPoint);
		printf("+[基地址]:0x%x\n", optional_Header.ImageBase);
		imageBase = optional_Header.ImageBase;
		printf("+[文件大小]:0x%x\n", optional_Header.SizeOfImage);
		secion_table = (PIMAGE_SECTION_HEADER)((DWORD)nt_Header64 + 24 + file_Header.SizeOfOptionalHeader);
		printf("+[节数量]:%d\n", numberofsection);
		printf("+[节名称]\n");
		for (int i = 0; i < numberofsection; i++)
		{
			printf("+[%d]:%s\n", i, secion_table[i].Name);
		}
		int temp = 'y';
		printf("+[输入] 是否要获取导出表[y/n]:");
		scanf_s("%c", &temp);
		//是否有导出表
		if (optional_Header.DataDirectory[0].VirtualAddress != 0 && temp == 'y') {
			//导出表的RAW
			DWORD eatoffset = RVA2RAW(optional_Header.DataDirectory[0].VirtualAddress);
			//RAW加上imagebase 因为是直接读文件并没有映射为内存
			PIMAGE_EXPORT_DIRECTORY eat = (PIMAGE_EXPORT_DIRECTORY)((DWORD)dos_Header + eatoffset);
			printf("+[dllName]:%s\n", (DWORD)dos_Header + RVA2RAW(eat->Name));
			//printf("%x", RVA2RAW(0xe9a4));
			printf("+[导出函数]\n");
			PDWORD functionanems = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNames));
			PWORD addressOfNameOrdinals = (PWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNameOrdinals));
			PDWORD addressOfFunction = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfFunctions));
			for (int i = 0; i < eat->NumberOfNames; i++)
			{
				WORD ordinalsIndex = addressOfNameOrdinals[i];
				printf("+[序号]:%d[RVA]:0x%x[名称]:%s\n", ordinalsIndex, addressOfFunction[ordinalsIndex], RVA2RAW(functionanems[i]) + DWORD(dos_Header));
			}
		}
	}
	
}
DWORD RVA2RAW(DWORD RVA) {
	for (int i = 0; i < numberofsection; i++)
	{
		if (secion_table [i].VirtualAddress <= RVA &&RVA <= secion_table[i + 1].VirtualAddress) {
			return RVA-secion_table[i].VirtualAddress + secion_table[i].PointerToRawData;
		}
	}
	return -1;
}

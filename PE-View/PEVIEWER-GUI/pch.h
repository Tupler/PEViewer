// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"

#endif //PCH_H


extern char* contents;
extern IMAGE_FILE_HEADER file_Header;
extern PIMAGE_NT_HEADERS64 nt_Header64;
extern PIMAGE_NT_HEADERS nt_Header32;
extern PIMAGE_SECTION_HEADER secion_table;
extern DWORD image_base;
extern DWORD numberofsection;
extern HICON m_hIcon;
extern PIMAGE_DOS_HEADER dos_Header;
extern int isx86;
extern int nums;
extern IMAGE_OPTIONAL_HEADER32 optional_Header32;
extern IMAGE_OPTIONAL_HEADER64 optional_Header64;
static DWORD RVA2RAW(DWORD RVA) {
	for (int i = 0; i < numberofsection; i++)
	{

		if (secion_table[i].VirtualAddress <= RVA && RVA <= secion_table[i + 1].VirtualAddress) {
			return RVA - secion_table[i].VirtualAddress + secion_table[i].PointerToRawData;

		}

	}
	return -1;
}
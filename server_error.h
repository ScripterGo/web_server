
#pragma once
#include<Windows.h>
#include<string>

using namespace std;

void create_message_box(wstring error_desc, int line_nr, const char* file) {
	wstring res_desc(error_desc);
	res_desc.push_back('\n');
	res_desc.append(wstring(L"on line: ") + to_wstring(line_nr));
	res_desc.push_back('\n');
	
	const int MAX_CNT = 300;
	wchar_t wide_filename[MAX_CNT];
	size_t res_sz;
	mbstowcs_s(&res_sz, wide_filename, file, MAX_CNT);
	res_desc.append(wstring(L"in file: ")).append(wstring(wide_filename));

	MessageBoxEx(nullptr, res_desc.c_str(), L"Error!", MB_OKCANCEL | MB_ICONERROR, 0);
}

#define SERVER_ERROR_THROW(desc) (create_message_box(desc, __LINE__, __FILE__)); 

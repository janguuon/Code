#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>


using namespace std;

#define MY_MAX 1024

void UTF16toUTF8(const wchar_t * utf16, char * utf8)
{
    int index_UTF16 = 0;
	int index_UTF8 = 0;
	
	while (L'\0' != utf16[index_UTF16])
	{	
		if (0xd800 == (utf16[index_UTF16] & 0xd800) &&  // 4byte
			0xdc00 == (utf16[index_UTF16 + 1] & 0xdc00))
		{
			utf8[index_UTF8++] = static_cast<char>(0Xf0 + (((utf16[index_UTF16] / (0b010000000)) % (0b01000))));
			utf8[index_UTF8++] = static_cast<char>(0x90 + (((utf16[index_UTF16] / (0b01000000)) % 0b10) << 5) + (((utf16[index_UTF16] / (0b0100)) % (0b010000))));
			utf8[index_UTF8++] = static_cast<char>(0x80 + ((utf16[index_UTF16] % (0b0100)) << 4) + ((utf16[index_UTF16 + 1] / (0b1000000)) % 0b10000));
			utf8[index_UTF8++] = static_cast<char>(0x80 + (utf16[index_UTF16 + 1] % (0b1000000)));
			index_UTF16++;
		}
		else if (0x7f >= utf16[index_UTF16]) // 1byte
		{
			utf8[index_UTF8++] = static_cast<char>(utf16[index_UTF16]);
		}
		else if (0x7ff >= utf16[index_UTF16]) // 2byte
		{
			utf8[index_UTF8++] = static_cast<char>(0xc0 + (utf16[index_UTF16] / (0b01000000)));
			utf8[index_UTF8++] = static_cast<char>(0x80 + (utf16[index_UTF16] % (0b01000000)));
		}
		else if (0xffff >= utf16[index_UTF16]) // 3byte
		{
			utf8[index_UTF8++] = static_cast<char>(0xe0 + (utf16[index_UTF16]  / (0b01 << 12)));
			utf8[index_UTF8++] = static_cast<char>(0x80 + ((utf16[index_UTF16] / (0b01000000)) % (0b01000000)));
			utf8[index_UTF8++] = static_cast<char>(0x80 + (utf16[index_UTF16]  % (0b01000000)));
		}

		index_UTF16++;
	}

	utf8[index_UTF8] = '\0';
}

void UTF8toUTF16(const unsigned char* utf8, wchar_t* utf16)
{
	int number_Unicode = 0;
	int index_UTF8 = 0;
	int index_UTF16 = 0;


	while ('\0' != utf8[index_UTF8])
	{

		if (0xf0 == ((utf8[index_UTF8]) & 0xf0) &&
			0x80 == ((utf8[index_UTF8 + 1]) & 0xc0) &&
			0x80 == ((utf8[index_UTF8 + 2]) & 0xc0) &&
			0x80 == ((utf8[index_UTF8 + 3]) & 0xc0))
		{
			number_Unicode += 0xd800;
			number_Unicode += (utf8[index_UTF8++] & 0x07) << 7;
			number_Unicode += (utf8[index_UTF8] & 0x20) << 1;
			number_Unicode += (utf8[index_UTF8++] & 0xf) << 2;
			number_Unicode += (utf8[index_UTF8] & 0x30) / 0b10000;
			utf16[index_UTF16++] = static_cast<wchar_t>(number_Unicode);
			
			number_Unicode = 0;
			number_Unicode += 0xdc00;
			number_Unicode += (utf8[index_UTF8++] & 0xf) << 6;
			number_Unicode += (utf8[index_UTF8++] & 0x3f);
		}
		else if (0 == ((utf8[index_UTF8]) & 0x80)) 				 
		{
			number_Unicode = utf8[index_UTF8++];
		}
		else if (0xc0 == ((utf8[index_UTF8]) & 0xe0) &&
				 0x80 == ((utf8[index_UTF8 + 1]) & 0xc0))
		{
			number_Unicode += ((utf8[index_UTF8++]) & 0x1f) << 6;
			number_Unicode += ((utf8[index_UTF8++]) & 0x3f);
		}
		else if (0xe0 == ((utf8[index_UTF8]) & 0xe0)		&&
				 0x80 == ((utf8[index_UTF8 + 1]) & 0xc0)	&&
				 0x80 == ((utf8[index_UTF8 + 2]) & 0xc0))
		{
			number_Unicode += ((utf8[index_UTF8++]) & 0x1f) << 12;
			number_Unicode += ((utf8[index_UTF8++]) & 0x3f) << 6;
			number_Unicode += ((utf8[index_UTF8++]) & 0x3f);
		}
		

		utf16[index_UTF16++] = static_cast<wchar_t>(number_Unicode);
		number_Unicode = 0;
	}

	utf16[index_UTF16] = L'\0';
}

void FileConv_UTF8ToUTF16(string filepath)
{
	string _path = "C:/Users/ohsy/Desktop/";
	_path += filepath;
	char input_Char[MY_MAX] = { 0 };
	wchar_t* output_WChar = new wchar_t[MY_MAX]{ 0 };

	FILE* pWrite_File, * pRead_File;
	pWrite_File = fopen("C:/Users/ohsy/Desktop/utf16-conv.txt", "wb");
	fopen_s(&pRead_File, _path.c_str(), "rb");
	if (!pRead_File) return;

	fread(&input_Char, sizeof(char), MY_MAX, pRead_File);

	if (0xef != (((unsigned char)input_Char[0])) &&
		0xbb != (((unsigned char)input_Char[1])) &&				// BOM 체크.
		0xbf != (((unsigned char)input_Char[2])))
	{
		unsigned char bom[] = { 0xFF, 0xFE };
		fwrite(bom, sizeof(unsigned char), sizeof(bom), pWrite_File);
	}

	UTF8toUTF16((const unsigned char*)input_Char, output_WChar);
	
	fwrite(output_WChar, sizeof(wchar_t), wcslen(output_WChar), pWrite_File);

	fclose(pWrite_File);
	fclose(pRead_File);
	delete[] output_WChar;
	output_WChar = nullptr;
}

void FileConv_UTF16ToUTF8(string filepath)
{
	string _path = "C:/Users/ohsy/Desktop/";
	_path += filepath;

	wchar_t input_WChar[MY_MAX] = { 0 };
	char* output_Char = new char[MY_MAX];

	FILE* pRead_File, * pWrite_File;
	fopen_s(&pRead_File, _path.c_str(), "rb");
	if (!pRead_File) return;

	fread(&input_WChar, sizeof(wchar_t), 1, pRead_File);
	if (input_WChar[0] != (wchar_t)0xFEFF) return;
	
	fread(&input_WChar, sizeof(wchar_t), MY_MAX, pRead_File);
	UTF16toUTF8(input_WChar, output_Char);

	pWrite_File = fopen("C:/Users/ohsy/Desktop/utf8-conv.txt", "wb");
	fwrite(output_Char, sizeof(char), strlen(output_Char), pWrite_File);

	fclose(pRead_File);
	fclose(pWrite_File);

	delete[] output_Char;
	output_Char = nullptr;
}

int main(int argc, char * argv[])
{
    //setlocale(LC_ALL, "ko_KR.utf8");
	locale::global(locale(".UTF-8"));
	enum MENU {UTF16 = 1, UTF8 = 2};
	int index_Menu = 0;
	cout << "=========================" << endl;
	wcout << L"(1) UTF-8 -> UTF-16 변환 \n\n(2) UTF-16 -> UTF-8 변환 \n\n(3) 종료" << endl;
	cout << "=========================" << endl;
	cin >> index_Menu;

	switch (index_Menu)
	{
	case UTF16:
		FileConv_UTF8ToUTF16(argv[1]);
		break;
	case UTF8:
		FileConv_UTF16ToUTF8(argv[1]);
		break;
	default:
		return 0;
		break;
	}

	return 0;
}
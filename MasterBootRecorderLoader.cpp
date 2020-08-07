#include <Windows.h>
#include <iostream>
using namespace std;

#define SMODE 512
#define MMODE 2048
int mode;

void ReadFile(unsigned char ReadBuffer[], char path[], int mode) {

    if (mode == 0) {
        wchar_t wtext[SMODE];
        mbstowcs(wtext, path, strlen(path) + 1);
        LPWSTR ptr = wtext;

        HANDLE hFile;
        DWORD dwBytesRead = 0;
        ReadBuffer[SMODE] = { 0 };
        OVERLAPPED ol = { 0 };
        hFile = CreateFile(ptr, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
        ReadFile(hFile, ReadBuffer, SMODE, &dwBytesRead, &ol);
        CloseHandle(hFile);
    }
    else {
        wchar_t wtext[MMODE];
        mbstowcs(wtext, path, strlen(path) + 1);
        LPWSTR ptr = wtext;

        HANDLE hFile;
        DWORD dwBytesRead = 0;
        ReadBuffer[MMODE] = { 0 };
        OVERLAPPED ol = { 0 };
        hFile = CreateFile(ptr, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
        ReadFile(hFile, ReadBuffer, MMODE, &dwBytesRead, &ol);
        CloseHandle(hFile);
    }
}

void LoadInMBR(BYTE *MasterBootRecord, int size) {
    DWORD dwBytesWritten;
    HANDLE hDevice = CreateFileW(
        L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
        FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
        OPEN_EXISTING, 0, 0);

    WriteFile(hDevice, &MasterBootRecord, size, &dwBytesWritten, 0);
    CloseHandle(hDevice);
}

int main(int argc, char** argv)
{   
    Link:
    int mode = 0;

    cout << "Enter mode ('0' for 512 bytes MBR or '1' for 4 kilobytes MBR)";
    cin >> mode;

    if (mode == 0) {
        BYTE fbuffer[SMODE + 12];

        BYTE buffer[SMODE];

        ReadFile(fbuffer, argv[1], mode);

        for (int i = 13; i < SMODE; i++) buffer[i - 12] = fbuffer[i];

        LoadInMBR(buffer, 512);
    }
    else if(mode == 1){

        BYTE fbuffer[MMODE + 12];

        BYTE buffer[MMODE];

        ReadFile(fbuffer, argv[1], mode);

        for (int i = 13; i < MMODE; i++) buffer[i - 12] = fbuffer[i];

        LoadInMBR(buffer, 2048);
    }
    else {
        MessageBox(0, L"Unknown mode!", L"Error", MB_OK);
        goto Link;
    }
    return 0;
}

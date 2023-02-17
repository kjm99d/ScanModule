#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <tchar.h>
#include <sstream>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>

#pragma comment(lib, "Version.lib")


typedef struct _FileVersion {
    WORD major;
    WORD minor;
    WORD buildNum;
    WORD revisionNum;

    std::string GetString() {
        std::stringstream stream;
        stream << major << "." << minor << "." << buildNum << "." << revisionNum;
        return stream.str();
    }
} FileVersion;

int PrintModules(DWORD processID);
BOOL GetFileVersion(std::string strFilePath, FileVersion& version);


int main(void)
{
    try
    {
        DWORD dwProcessId = 0;
        // Get the list of process identifiers.
        std::cout << "PID : ";
        std::cin >> dwProcessId;

        PrintModules(dwProcessId);
    }
    catch (const std::exception&)
    {
        printf("Exception !");
    }

    system("pause");

    return 0;
}



BOOL GetFileVersion(std::string strFilePath, FileVersion& version)
{
    BOOL nRet = FALSE;

    if (strFilePath.empty() == false)
    {
        // 파일 정보 저장 버퍼
        TCHAR atcBuffer[MAX_PATH] = { 0, };
        if (FALSE != GetFileVersionInfoA(strFilePath.c_str(), 0, MAX_PATH, atcBuffer))
        {
            VS_FIXEDFILEINFO* pFineInfo = NULL;
            UINT bufLen = 0;
            // VS_FIXEDFILEINFO 정보 가져오기
            if (FALSE != VerQueryValueA(atcBuffer, "\\", (LPVOID*)&pFineInfo, &bufLen) != 0)
            {
                WORD majorVer, minorVer, buildNum, revisionNum;
                majorVer = HIWORD(pFineInfo->dwFileVersionMS);
                minorVer = LOWORD(pFineInfo->dwFileVersionMS);
                buildNum = HIWORD(pFineInfo->dwFileVersionLS);
                revisionNum = LOWORD(pFineInfo->dwFileVersionLS);

                version = {
                    majorVer,
                    minorVer,
                    buildNum,
                    revisionNum
                };

                nRet = TRUE;
            }
        }
    }
    return nRet;
}

int PrintModules(DWORD processID)
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    // Get a handle to the process.
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);
    if (NULL == hProcess)
        return 1;

    std::ofstream logger("ScanModule.csv");

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH] = { 0, };

            // Get the full path to the module's file.
            if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                // Wide String to Ansi String
                std::wstring wstr(szModName);
                std::string strMod = std::string().assign(wstr.begin(), wstr.end());

                FileVersion version;
                if (GetFileVersion(strMod, version))
                {
                    logger << strMod << "," << version.GetString() << std::endl;
                    std::cout << strMod << "\t" << version.GetString() << std::endl;
                }
            }
        }
    }

    logger.close();

    // Release the handle to the process.
    CloseHandle(hProcess);

    return 0;
}

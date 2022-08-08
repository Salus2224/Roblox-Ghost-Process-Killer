// Roblox Ghost Process Killer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <iostream>
#include <vector>

using namespace std;

struct window_data { // for passing win data to enumWindows callback
    DWORD pid = -1;
    bool hasWindow = false;
    wstring winName = L"";
};

BOOL CALLBACK enumWindowsCallback(HWND handle, LPARAM lParam)
{
    window_data& winData = *(window_data*)lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId(handle, &process_id);
    if (winData.pid == process_id) {

        wchar_t name[1000]; // make var
        memset(name, 0, 1000); // alloc mem
        GetWindowTextW(handle, name, 1000); // get window name and store in var

        wstring name2 = name; // convert to wstring for comparing later
        if (name2 == winData.winName)  // compare window name to either "Roblox" (default name), or the parameter of the most recent call to setwindowtitle()
            winData.hasWindow = true;
    }
    return TRUE;
}

int main()
{
    HANDLE hndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE, 0);
    HANDLE memInfo;
    PROCESS_INFORMATION_CLASS pic = ProcessAppMemoryInfo;
    wstring name = L"RobloxPlayerBeta.exe"; // exe name (should be constant)
    wstring newname = L"";
    APP_MEMORY_INFORMATION ami;
    vector<DWORD> pids; // store pids for later process execution

    if (hndl) {
        PROCESSENTRY32  process = { sizeof(PROCESSENTRY32) };
        Process32First(hndl, &process);
        do {
            newname = process.szExeFile; // didn't want to learn to compare wchar* so I converted to wstring
            if (newname == name) {

                memInfo = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process.th32ProcessID);
                
                if (GetProcessInformation(memInfo, pic, &ami, sizeof(APP_MEMORY_INFORMATION))) {
                    if (ami.PrivateCommitUsage > 100000000) { // more than 100mb of memory

                        window_data winData; // struct to pass PID, and recieve output
                        winData.pid = process.th32ProcessID;
                        winData.winName = L"Roblox"; // or setwindowtitle() value
                        EnumWindows(enumWindowsCallback, (LPARAM)(&winData)); // loop through windows

                        if (!winData.hasWindow) {
                            pids.push_back(process.th32ProcessID); // add to list of PIDs to kill
                        }
                    }
                }
            }
        } while (Process32Next(hndl, &process));

        CloseHandle(hndl);

        if (pids.size()) {
            for (int i = 0; i < pids.size(); i++) { // iterate saved processes
                const auto explorer = OpenProcess(PROCESS_TERMINATE, false, pids[i]); // kill
                TerminateProcess(explorer, 1);
                CloseHandle(explorer);
            }
            cout << "KILLED: " << pids.size() << " DEAD PROCESS(ES)" << endl;
        }
        else {
            cout << "NO DEAD PROCESSES FOUND" << endl;
        }
    }
}
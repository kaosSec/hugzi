#include <windows.h>

// Define the function to be exported from the DLL
__declspec(dllexport) void hook_open(DWORD pid, const char* filename) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        MessageBox(NULL, TEXT("Error: could not open process"), TEXT("Error"), MB_OK);
        return;
    }

    LPVOID pFilename = VirtualAllocEx(hProcess, NULL, strlen(filename) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pFilename == NULL) {
        MessageBox(NULL, TEXT("Error: could not allocate memory"), TEXT("Error"), MB_OK);
        CloseHandle(hProcess);
        return;
    }

    if (!WriteProcessMemory(hProcess, pFilename, filename, strlen(filename) + 1, NULL)) {
        MessageBox(NULL, TEXT("Error: could not write to process memory"), TEXT("Error"), MB_OK);
        VirtualFreeEx(hProcess, pFilename, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    // Get the address of the kernel32!LoadLibraryA function
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    FARPROC pLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");
    if (pLoadLibraryA == NULL) {
        MessageBox(NULL, TEXT("Error: could not get the address of LoadLibraryA"), TEXT("Error"), MB_OK);
        VirtualFreeEx(hProcess, pFilename, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    // Create a remote thread in the target process to execute the LoadLibraryA function
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, pFilename, 0, NULL);
    if (hThread == NULL) {
        MessageBox(NULL, TEXT("Error: could not create remote thread"), TEXT("Error"), MB_OK);
        VirtualFreeEx(hProcess, pFilename, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pFilename, 0, MEM_RELEASE);

    CloseHandle(hThread);
    CloseHandle(hProcess);
}

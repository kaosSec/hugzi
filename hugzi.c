#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>

typedef void (*HookOpenFunction)(DWORD pid, const char* filename, const char* server_address, const char* server_port);

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: %s <process name> <file name> <server address> <server port>\n", argv[0]);
        return 1;
    }

    const char* process_name = argv[1];
    const char* file_name = argv[2];
    const char* server_address = argv[3];
    const char* server_port = argv[4];

    HMODULE hDLL = LoadLibrary("hook.dll");
    if (hDLL == NULL) {
        printf("Error: could not load DLL\n");
        return 1;
    }

    HookOpenFunction hook_open = (HookOpenFunction)GetProcAddress(hDLL, "hook_open");
    if (hook_open == NULL) {
        printf("Error: could not find hook_open function\n");
        FreeLibrary(hDLL);
        return 1;
    }

    DWORD pid = find_process(process_name);
    if (pid == 0) {
        printf("Could not find process\n");
        FreeLibrary(hDLL);
        return 1;
    }

    hook_open(pid, file_name, server_address, server_port);

    FreeLibrary(hDLL);

    return 0;
}

void hook_open(DWORD pid, const char* filename, const char* server_address, const char* server_port) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        printf("Error: could not open process with PID %u\n", pid);
        return;
    }

    CONTEXT ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.ContextFlags = CONTEXT_FULL;

    WaitForDebugEvent(NULL, INFINITE);

    GetThreadContext(GetCurrentThread(), &ctx);

    DWORD orig_rax = ctx.Eax;
    DWORD params[3];
    params[0] = ctx.Ebx;
    params[1] = ctx.Ecx;
    params[2] = ctx.Edx;

    if (orig_rax == 0x3) {
        ctx.Ebx = (DWORD)filename;

        SetThreadContext(GetCurrentThread(), &ctx);
    }

    ContinueDebugEvent(GetCurrentProcessId(), GetCurrentThreadId(), DBG_CONTINUE);

    WaitForDebugEvent(NULL, INFINITE);

    GetThreadContext(GetCurrentThread(), &ctx);

    DWORD rax = ctx.Eax;
    if (rax == (DWORD)-1) {
        printf("Hook failed: open returned %ld\n", rax);
        return;
    }

    ctx.Eax = orig_rax;
    ctx.Ebx = params[0];
    ctx.Ecx = params[1];
    ctx.Edx = params[2];

    SetThreadContext(GetCurrentThread(), &ctx);

    ContinueDebugEvent(GetCurrentProcessId(), GetCurrentThreadId(), DBG_CONTINUE);
}

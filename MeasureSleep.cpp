#include <iostream>
#include <windows.h>

typedef NTSTATUS(CALLBACK* NTQUERYTIMERRESOLUTION) (
    OUT PULONG MinimumResolution,
    OUT PULONG MaximumResolution,
    OUT PULONG CurrentResolutionolution
    );

int main() {
    ULONG MinimumResolution, MaximumResolution, CurrentResolution;
    LARGE_INTEGER start, end, freq;

    QueryPerformanceFrequency(&freq);

    HMODULE hNtDll = LoadLibrary(L"NtDll.dll");

    if (!hNtDll) {
        printf("LoadLibrary failed");
        return 1;
    }

    NTQUERYTIMERRESOLUTION NtQueryTimerResolution = (NTQUERYTIMERRESOLUTION)GetProcAddress(hNtDll, "NtQueryTimerResolution");

    for (;;) {
        // get current resolution
        if (NtQueryTimerResolution(&MinimumResolution, &MaximumResolution, &CurrentResolution) != 0) {
            printf("NtQueryTimerResolution failed");
            return 1;
        }

        // benchmark Sleep(1)
        QueryPerformanceCounter(&start);
        Sleep(1);
        QueryPerformanceCounter(&end);

        double delta_s = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        double delta_ms = delta_s * 1000;

        printf("Resolution: %lfms, Sleep(1) slept %lfms (delta: %lf)\n",
            CurrentResolution / 10000.0, delta_ms, delta_ms - 1);

        Sleep(1000); // pause for a second between iterations
    }
}

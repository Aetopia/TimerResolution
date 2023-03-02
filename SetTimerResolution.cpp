#include <stdio.h>
#include <windows.h>

typedef NTSTATUS(CALLBACK *NTQUERYTIMERRESOLUTION)(
    OUT PULONG MinimumResolution,
    OUT PULONG MaximumResolution,
    OUT PULONG CurrentResolutionolution);

typedef NTSTATUS(CALLBACK *NTSETTIMERRESOLUTION)(
    IN ULONG DesiredResolution,
    IN BOOLEAN SetResolution,
    OUT PULONG CurrentResolutionolution);

int main() {
    ULONG MinimumResolution, MaximumResolution, CurrentResolution;
    PROCESS_POWER_THROTTLING_STATE PowerThrottling;

    HMODULE hNtDll = LoadLibrary(L"NtDll.dll");

    if (!hNtDll) {
        printf("LoadLibrary failed");
        return 1;
    }

    NTQUERYTIMERRESOLUTION NtQueryTimerResolution = (NTQUERYTIMERRESOLUTION)GetProcAddress(hNtDll, "NtQueryTimerResolution");
    NTSETTIMERRESOLUTION NtSetTimerResolution = (NTSETTIMERRESOLUTION)GetProcAddress(hNtDll, "NtSetTimerResolution");

    RtlZeroMemory(&PowerThrottling, sizeof(PowerThrottling));

    PowerThrottling.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
    PowerThrottling.ControlMask = PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION;
    PowerThrottling.StateMask = 0;

    SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &PowerThrottling, sizeof(PowerThrottling));

    if (NtQueryTimerResolution(&MinimumResolution, &MaximumResolution, &CurrentResolution) != 0) {
        printf("NtQueryTimerResolution failed");
        return 1;
    }

    if (NtSetTimerResolution(MaximumResolution, true, &CurrentResolution) != 0) {
        printf("NtSetTimerResolution failed");
        return 1;
    }

    printf("Resolution set to: %lfms", (double)CurrentResolution / 10000);
    Sleep(INFINITE);
}

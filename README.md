# TimerResolution

Raise the clock interrupt frequency in Windows

See [Fixing timing precision in Windows after the "The Great Rule Change"](https://github.com/amitxv/PC-Tuning/blob/main/docs/research.md#fixing-timing-precision-in-windows-after-the-great-rule-change) for a detailed explanation regarding restoring the old timer resolution implementation otherwise nothing below will make sense.

``MeasureSleep`` is used to measure the precision of Sleep(1). The closer it sleeps to 1ms in reality, the better.

``SetTimerResolution`` queries and sets the maximum timer resolution applicable to the system (usually 0.5ms). To start it automatically on boot, you can place the binary in ``shell:startup``.

On Windows Server 21H2+ and Windows 11+, the registry key below should also be used so that requesting a higher resolution is effective on a system-wide level rather than only the calling process.

```
[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\kernel]
"GlobalTimerResolutionRequests"=dword:00000001
```

#ifndef COSMOPOLITAN_LIBC_NT_RUNTIME_H_
#define COSMOPOLITAN_LIBC_NT_RUNTIME_H_
#include "libc/nt/struct/overlapped.h"
#include "libc/nt/thunk/msabi.h"
#include "libc/nt/typedef/handlerroutine.h"

/**
 * @fileoverview NT Obligatory Runtime Functions.
 *
 * These functions are placed in their own file because they're (a)
 * abstracted by the Cosmopolitan runtime; and (b) it helps GCC avoid
 * bloating binaries with debug information the user doesn't need.
 */

#define kNtCpUtf8             65001
#define kNtInvalidHandleValue -1L
#define kNtStdInputHandle     -10L
#define kNtStdOutputHandle    -11L
#define kNtStdErrorHandle     -12L

#if !(__ASSEMBLER__ + __LINKER__ + 0)
COSMOPOLITAN_C_START_

char16_t *GetCommandLine(void) nosideeffect;
char16_t *GetEnvironmentStrings(void) nodiscard;
bool32 FreeEnvironmentStrings(char16_t *) paramsnonnull();
bool32 ReadFile(int64_t hFile, void *lpBuffer, uint32_t nNumberOfBytesToRead,
                uint32_t *lpNumberOfBytesRead,
                struct NtOverlapped *opt_lpOverlapped);
bool32 WriteFile(int64_t hFile, const void *lpBuffer,
                 uint32_t nNumberOfBytesToWrite,
                 uint32_t *lpNumberOfBytesWritten,
                 struct NtOverlapped *opt_lpOverlapped);
bool32 TerminateProcess(int64_t hProcess, uint32_t uExitCode);
int64_t GetCurrentProcess(void) pureconst;
void ExitProcess(uint32_t uExitCode) wontreturn;
uint32_t GetLastError(void) nosideeffect;
bool32 CloseHandle(int64_t hObject) nothrow nocallback;
intptr_t GetStdHandle(int64_t nStdHandle) nosideeffect;
bool32 SetStdHandle(int64_t nStdHandle, int64_t hHandle);
bool32 SetDefaultDllDirectories(unsigned dirflags);

#if ShouldUseMsabiAttribute()
#include "libc/nt/thunk/runtime.inc"
#endif /* ShouldUseMsabiAttribute() */
COSMOPOLITAN_C_END_
#endif /* !(__ASSEMBLER__ + __LINKER__ + 0) */
#endif /* COSMOPOLITAN_LIBC_NT_RUNTIME_H_ */

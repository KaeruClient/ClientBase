//
// Created by KaeruClient on 2026/05/23.
//

#include "ExceptionHandler.hpp"

#include <string>
#include <Windows.h>

#include "fmt/format.h"

auto WINAPI ExceptionHandler::handlerCallback(PEXCEPTION_POINTERS exceptionInfo) -> LONG {
    const auto* exceptionRecord = exceptionInfo->ExceptionRecord;
    const auto* contextRecord = exceptionInfo->ContextRecord;

    std::string exceptionName;
    switch (exceptionRecord->ExceptionCode) {
        case EXCEPTION_ACCESS_VIOLATION:         exceptionName = "ACCESS_VIOLATION"; break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    exceptionName = "ARRAY_BOUNDS_EXCEEDED"; break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:       exceptionName = "INT_DIVIDE_BY_ZERO"; break;
        case EXCEPTION_STACK_OVERFLOW:           exceptionName = "STACK_OVERFLOW"; break;
        default: exceptionName = "UNKNOWN"; break;
    }

    const std::string errorReport = fmt::format(
        "{} caught an unhandled exception and has crashed!\n\n"
        "--------------------------------------------------\n"
        "Exception Code: 0x{:X} ({})\n"
        "Faulting Address: 0x{:X}\n"
        "--------------------------------------------------\n"
        "Registers:\n"
        "RAX: 0x{:X}   RBX: 0x{:X}\n"
        "RCX: 0x{:X}   RDX: 0x{:X}\n"
        "RSP: 0x{:X}   RBP: 0x{:X}\n"
        "RIP: 0x{:X}\n"
        "--------------------------------------------------\n"
        "Press OK to terminate the process.\n"
        "You can attach a debugger now before pressing OK.",
        CLIENT_NAME, exceptionRecord->ExceptionCode, exceptionName,
        reinterpret_cast<uintptr_t>(exceptionRecord->ExceptionAddress),
        contextRecord->Rax, contextRecord->Rbx,
        contextRecord->Rcx, contextRecord->Rdx,
        contextRecord->Rsp, contextRecord->Rbp,
        contextRecord->Rip
    );

    MessageBoxA(
        nullptr,
        errorReport.c_str(),
        CLIENT_NAME,
        MB_OK | MB_ICONERROR | MB_SYSTEMMODAL
    );

    return EXCEPTION_EXECUTE_HANDLER;
}

//
// Created by KaeruClient on 2026/05/23.
//

#pragma once
#include <Windows.h>

class ExceptionHandler {
    static inline PVOID mVehHandle = nullptr;
    static auto WINAPI handlerCallback(PEXCEPTION_POINTERS exceptionInfo) -> LONG;
public:
    static auto init() -> void {
        if (mVehHandle == nullptr)
            mVehHandle = AddVectoredExceptionHandler(1, handlerCallback);
    }

    static auto shutdown() -> void {
        if (mVehHandle != nullptr) {
            RemoveVectoredExceptionHandler(mVehHandle);
            mVehHandle = nullptr;
        }
    }
};
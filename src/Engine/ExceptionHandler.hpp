//
// Created by KaeruClient on 2026/05/23.
//

#pragma once
#include <Windows.h>

class ExceptionHandler {
    static inline PVOID mVehHandle = nullptr;
    static LONG WINAPI handlerCallback(PEXCEPTION_POINTERS exceptionInfo);
public:
    static void init() {
        if (mVehHandle == nullptr)
            mVehHandle = AddVectoredExceptionHandler(1, handlerCallback);
    }

    static void shutdown() {
        if (mVehHandle != nullptr) {
            RemoveVectoredExceptionHandler(mVehHandle);
            mVehHandle = nullptr;
        }
    }
};
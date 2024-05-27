﻿#include <windows.h>
#include <string>
#include <iostream>
#include "tchar.h"
#include <fstream>

CRITICAL_SECTION FileLockingCriticalSection;

int ReadFromFile() {
    EnterCriticalSection(&FileLockingCriticalSection);
    std::fstream myfile("balanceCritical.txt", std::ios_base::in);
    int result;
    myfile >> result;
    myfile.close();
    LeaveCriticalSection(&FileLockingCriticalSection);

    return result;
}

void WriteToFile(int data) {
    EnterCriticalSection(&FileLockingCriticalSection);
    std::fstream myfile("balanceCritical.txt", std::ios_base::out);
    myfile << data << std::endl;
    myfile.close();
    LeaveCriticalSection(&FileLockingCriticalSection);
}



int GetBalance() {
    int balance = ReadFromFile();
    return balance;
}

void Deposit(int money) {
    int balance = GetBalance();
    balance += money;

    WriteToFile(balance);
    printf("Balance after deposit: %d\n", balance);
}

void Withdraw(int money) {
    if (GetBalance() < money) {
        printf("Cannot withdraw money, balance lower than %d\n", money);
        return;
    }

    Sleep(20);
    int balance = GetBalance();
    balance -= money;
    WriteToFile(balance);
    printf("Balance after withdraw: %d\n", balance);
}

DWORD WINAPI DoDeposit(CONST LPVOID lpParameter)
{
    EnterCriticalSection(&FileLockingCriticalSection);
    Deposit((int)lpParameter);
    LeaveCriticalSection(&FileLockingCriticalSection);
    return 0;
}

DWORD WINAPI DoWithdraw(CONST LPVOID lpParameter)
{
    EnterCriticalSection(&FileLockingCriticalSection);
    Withdraw((int)lpParameter);
    LeaveCriticalSection(&FileLockingCriticalSection);
    return 0;
}

int main(int argc, CHAR* argv[])
{
    const int numThreads = 51;
    HANDLE* handles = new HANDLE[numThreads];

    InitializeCriticalSection(&FileLockingCriticalSection);

    WriteToFile(0);

    for (int i = 0; i < numThreads; i++) {
        handles[i] = (i % 2 == 0)
            ? CreateThread(NULL, 0, &DoDeposit, (LPVOID)230, 0, NULL)
            : CreateThread(NULL, 0, &DoWithdraw, (LPVOID)1000, 0, NULL);
    }

    WaitForMultipleObjects(numThreads, handles, true, INFINITE);
    printf("Final Balance: %d\n", GetBalance());

    for (int i = 0; i < numThreads; i++) {
        CloseHandle(handles[i]);
    }

    delete[] handles;

    DeleteCriticalSection(&FileLockingCriticalSection);
    system("pause");
    
    return 0;
}

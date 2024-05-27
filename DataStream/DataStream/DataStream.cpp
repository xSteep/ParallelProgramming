#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
    int threadNumber = *(int*)(lpParam);
    cout << "Поток №" << threadNumber << " выполняет свою работу" << endl;
    ExitThread(0);
}

int main(int argc, char* argv[]) {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (argc != 2) {
        return 1;
    }

    int numThreads = atoi(argv[1]);

    vector<HANDLE> Handles(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        int* threadNumber = new int(i + 1);
        Handles[i] = CreateThread(NULL, 0, ThreadProc, &i, CREATE_SUSPENDED, NULL);
    }

    for (int i = 0; i < numThreads; ++i) {
        ResumeThread(Handles[i]);
    }

    WaitForMultipleObjects(numThreads, Handles.data(), TRUE, INFINITE);

    for (int i = 0; i < numThreads; ++i) {
        CloseHandle(Handles[i]);
    }

    return 0;
}

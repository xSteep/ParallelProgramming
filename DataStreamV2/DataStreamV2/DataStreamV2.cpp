#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

ofstream out("output.txt");
clock_t start;


DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
    int threadNumber = *(reinterpret_cast<int*>(lpParam));
    for (int i = 0; i < 20; i++) {
        for (int i = 0; i < 1000000; i++) {
            int a = 0;
        }
        string str = to_string(threadNumber) + " | " + to_string(clock() - start) + "\n";
        out << str;
    }
    ExitThread(0);
}

int main(int argc, char* argv[]) {
    start = clock();
    int numThreads = 2;

    vector<HANDLE> Handles(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        int* threadNumber = new int(i + 1);
        Handles[i] = CreateThread(NULL, 0, ThreadProc, threadNumber, CREATE_SUSPENDED, NULL);
        if (Handles[i] == NULL) {
            return 1;
        }
    }
    SetThreadPriority(Handles[0], THREAD_PRIORITY_HIGHEST);

    for (int i = 0; i < numThreads; ++i) {
        ResumeThread(Handles[i]);
    }

    WaitForMultipleObjects(numThreads, Handles.data(), TRUE, INFINITE);

    for (int i = 0; i < numThreads; ++i) {
        CloseHandle(Handles[i]);
    }

    out.close();
    return 0;
}

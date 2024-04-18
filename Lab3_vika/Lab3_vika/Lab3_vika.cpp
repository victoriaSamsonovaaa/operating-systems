#include <iostream>
#include <vector>
#include <Windows.h>

bool isPrime(int num) {
    if (num <= 1)
        return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0)
            return false;
    }
    return true;
}

//  данные в поток
struct ThreadData {
    int start;
    int end;
    std::vector<int>* primes;
    CRITICAL_SECTION* criticalSection;
};

DWORD WINAPI findPrimes(void* param) {
    ThreadData* data = (ThreadData*)param;
    for (int i = data->start; i <= data->end; ++i) {
        if (isPrime(i)) {
            EnterCriticalSection(data->criticalSection);
            data->primes->push_back(i);
            LeaveCriticalSection(data->criticalSection);
        }
    }
    return 0;
}

int main() {
    std::vector<int> primes;
    CRITICAL_SECTION criticalSection;
    InitializeCriticalSection(&criticalSection);

    int rangeStart = 1;
    int rangeEnd = 100;

    int mid = (rangeStart + rangeEnd) / 2;

    ThreadData data1;
    data1.start = rangeStart;
    data1.end = mid;
    data1.primes = &primes;
    data1.criticalSection = &criticalSection;

    ThreadData data2;
    data2.start = mid + 1;
    data2.end = rangeEnd;
    data2.primes = &primes;
    data2.criticalSection = &criticalSection;

    HANDLE thread1 = CreateThread(NULL, 0, findPrimes, &data1, 0, NULL);
    HANDLE thread2 = CreateThread(NULL, 0, findPrimes, &data2, 0, NULL);

    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

   
    CloseHandle(thread1);
    CloseHandle(thread2);

    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;

    DeleteCriticalSection(&criticalSection);
    return 0;
}

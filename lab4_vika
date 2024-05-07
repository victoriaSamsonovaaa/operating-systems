#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>

struct ThreadData {
    int index;
    int start;
    int end;
    std::vector<int>* primes;
};

int N = 4;
int range = 100;

std::vector<int> primes;
ThreadData* threads;
HANDLE* hThreads;
HANDLE hMutex;
HANDLE hEvent;

bool isPrime(int num) {
    if (num <= 1)
        return false;
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0)
            return false;
    }
    return true;
}

DWORD WINAPI findPrimes(void* param) {
    ThreadData* data = (ThreadData*)param;

    for (int i = data->start; i <= data->end; ++i) {
        if (isPrime(i)) {
            WaitForSingleObject(hMutex, INFINITE);
            data->primes->push_back(i);
            ReleaseMutex(hMutex);
            SetEvent(hEvent);
        }
    }
    return 0;
}

int main() {
    threads = new ThreadData[N];
    hThreads = new HANDLE[N];
    hMutex = CreateMutex(NULL, FALSE, NULL);
    hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    for (int i = 0; i < N; ++i) {
        threads[i].start = (i * range / N + 1);
        threads[i].end = ((i + 1) * range / N);
        threads[i].index = i;
        threads[i].primes = &primes;
    }

    for (int i = 0; i < N; ++i) { hThreads[i] = CreateThread(NULL, 0, findPrimes, &threads[i], 0, NULL); }
    for (int i = 0; i < N; ++i) { WaitForSingleObject(hThreads[i], INFINITE); }

    for (int i = 0; i < N; ++i) { CloseHandle(hThreads[i]); }
    CloseHandle(hMutex);
    CloseHandle(hEvent);

    std::cout << "Before sorting: ";
    for (int prime : primes) { std::cout << prime << " "; }
    std::sort(primes.begin(), primes.end());
    std::cout << "\nAfter sorting: ";
    for (int prime : primes) { std::cout << prime << " "; }

    delete[] threads;
    delete[] hThreads;
    return 0;
}

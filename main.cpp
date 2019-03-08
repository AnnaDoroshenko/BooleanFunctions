#include <iostream>
#include <exception>
#include <mutex>
#include <thread>
#include "Function.h"

int main() {
    std::srand(std::time(nullptr));
    const unsigned int N = 28;
    const unsigned int STATISTIC_AMOUNT = 98;

    std::vector<unsigned int> nonlinearityStat;
    nonlinearityStat.reserve(STATISTIC_AMOUNT);

    const unsigned int THREADS_COUNT = 7;
    std::mutex mtx;
    auto statisticsStep = [&mtx, &nonlinearityStat](unsigned int threadId) {
        for (unsigned int i = threadId; i < STATISTIC_AMOUNT; i += THREADS_COUNT) {
            std::cout << "---------------- " << i <<" ------------------" << std::endl;
            const auto start = std::chrono::steady_clock::now();
            const auto testFunction = generateNonlinearFunc<N>();
            const auto generationEnd = std::chrono::steady_clock::now();
            std::cout << "######## " << i <<" has created func in "
                << std::chrono::duration_cast<std::chrono::seconds>(generationEnd - start).count()
                << " seconds." << std::endl;

            // try {
            //     testFunction.calculateNonlinearity();
            // } catch(const std::invalid_argument& e) {
            //     std::cerr << e.what() << std::endl;
            // }

            const auto sortedStats = testFunction.getSortedStatistics();
            const auto sortingEnd = std::chrono::steady_clock::now();
            std::cout << "######## " << i <<" has generated stats in "
                << std::chrono::duration_cast<std::chrono::seconds>(sortingEnd - generationEnd).count()
                << " seconds." << std::endl;
            const unsigned int currN = testFunction.calculateMinH(std::move(sortedStats));
            const auto minEnd = std::chrono::steady_clock::now();
            std::cout << "######## " << i <<" has calculated minH in "
                << std::chrono::duration_cast<std::chrono::seconds>(minEnd - sortingEnd).count()
                << " seconds." << std::endl;
            mtx.lock();
            nonlinearityStat.push_back(currN);
            mtx.unlock();
            std::cout << "######## " << i <<" took in total "
                << std::chrono::duration_cast<std::chrono::seconds>(minEnd - start).count()
                << " seconds." << std::endl;
            std::cout << "======================================" << std::endl;
            std::cout << "======================================" << std::endl;
            /* std::vector<unsigned int> index = {1}; */
            /* testFunction.calculateH(index); */
        }
    };
    std::thread threads[THREADS_COUNT];
    for (unsigned int threadId = 0; threadId < THREADS_COUNT; threadId++) {
        threads[threadId] = std::thread(statisticsStep, threadId);
    }
    for (auto& thread : threads) thread.join();

    // Stats
    unsigned int sum = 0;
    for (unsigned int k = 0; k < STATISTIC_AMOUNT; k++) {
        sum += nonlinearityStat.at(k);
    }
    unsigned int mean = static_cast<unsigned int>(sum / STATISTIC_AMOUNT) + 1;
    std::cout << "mean = " << mean << std::endl;

    return 0;
}

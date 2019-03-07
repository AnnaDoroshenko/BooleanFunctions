#include <iostream>
#include <exception>
#include <mutex>
#include <thread>
#include "Function.h"

int main() {
    std::srand(std::time(nullptr));
    const unsigned int N = 28;
    const unsigned int STATISTIC_AMOUNT = 4;

    std::vector<unsigned int> nonlinearityStat;
    nonlinearityStat.reserve(STATISTIC_AMOUNT);

    const unsigned int THREADS_COUNT = 4;
    std::mutex mtx;
    auto statisticsStep = [&mtx, &nonlinearityStat](unsigned int threadId) {
        for (unsigned int i = threadId; i < STATISTIC_AMOUNT; i += THREADS_COUNT) {
            std::cout << "---------------- " << i <<" ------------------" << std::endl;
            const auto testFunction = generateNonlinearFunc<N>();

            // try {
            //     testFunction.calculateNonlinearity();
            // } catch(const std::invalid_argument& e) {
            //     std::cerr << e.what() << std::endl;
            // }

            std::cout << "---- " << i <<" has created func." << std::endl;
            const auto sortedStats = testFunction.getSortedStatistics();
            std::cout << "---- " << i <<" has generated stats." << std::endl;
            const unsigned int currN = testFunction.calculateMinH(std::move(sortedStats));
            mtx.lock();
            nonlinearityStat.push_back(currN);
            mtx.unlock();
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

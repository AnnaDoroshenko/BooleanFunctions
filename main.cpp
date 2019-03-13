#include <iostream>
#include <exception>
#include <mutex>
#include <thread>
#include "Function.h"

int main() {
    std::srand(std::time(nullptr));
    const unsigned int N = 6;
    const unsigned int STATISTIC_AMOUNT = 98;

    std::vector<unsigned int> passesStat;
    passesStat.reserve(STATISTIC_AMOUNT);
    std::vector<float> percentStats;
    percentStats.reserve(STATISTIC_AMOUNT);

    const unsigned int THREADS_COUNT = 1;
    std::mutex mtx;
    auto statisticsStep = [&mtx, &passesStat, &percentStats](unsigned int threadId) {
        for (unsigned int i = threadId; i < STATISTIC_AMOUNT; i += THREADS_COUNT) {
            std::cout << "---------------- " << i <<" ------------------" << std::endl;
            const auto start = std::chrono::steady_clock::now();
            const auto testFunction = generateNonlinearFunc<N>();
            const auto generationEnd = std::chrono::steady_clock::now();
            std::cout << "######## " << i <<" has created func in "
                << std::chrono::duration_cast<std::chrono::seconds>(generationEnd - start).count()
                << " seconds." << std::endl;

            unsigned int realNL = 0;
            try {
                realNL = testFunction.calculateNonlinearity();
            } catch(const std::invalid_argument& e) {
                std::cerr << e.what() << std::endl;
            }

            const auto sortedStats = testFunction.getSortedStatistics();
            const auto sortingEnd = std::chrono::steady_clock::now();
            std::cout << "######## " << i <<" has generated stats in "
                << std::chrono::duration_cast<std::chrono::seconds>(sortingEnd - generationEnd).count()
                << " seconds." << std::endl;
            const auto [currN, currNonLin] = testFunction.calculateMinH(std::move(sortedStats));
            const auto minEnd = std::chrono::steady_clock::now();
            std::cout << "######## " << i <<" has calculated minH in "
                << std::chrono::duration_cast<std::chrono::seconds>(minEnd - sortingEnd).count()
                << " seconds." << std::endl;
            const auto percent = static_cast<float>(currNonLin) / static_cast<float>(realNL);
            std::cout << "$$$$$$$$$$$$$$$$$$Real = " << realNL << " and curr = " << currNonLin
                << "(percent = " << percent << ")" << std::endl;
            mtx.lock();
            percentStats.push_back(percent);
            passesStat.push_back(currN);
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
    float sumPercent = 0;
    float maxPercent = 0.0f;
    for (unsigned int k = 0; k < STATISTIC_AMOUNT; k++) {
        sum += passesStat.at(k);
        const float percent = percentStats.at(k);
        sumPercent  += percent;
        if (percent > maxPercent) { maxPercent = percent; }
    }
    unsigned int mean = static_cast<unsigned int>(sum / STATISTIC_AMOUNT) + 1;
    float meanPercent = static_cast<float>(sumPercent) / static_cast<float>(STATISTIC_AMOUNT);
    std::cout << "mean = " << mean << std::endl;
    std::cout << "percent mean = " << meanPercent << std::endl;
    std::cout << "max percent = " << maxPercent << std::endl;

    return 0;
}

#include <iostream>
#include <complex>
#include <iomanip>
#include <thread>

#include "../bitmap_image.hpp"
#include "../Timer.h"
#include "../eval.h"

// ===== Parameters =====
static constexpr double xmin = -5, xmax = 3, ymin = -1, ymax = 10;
static constexpr size_t w = 3840, h = 2160;
static constexpr size_t preIter = 100000;
static constexpr size_t postIter = 100000;
static constexpr double delX = (xmax - xmin) / w;

void StrideComputeValues(bitmap_image& bmp, int threadIndex, int numThreads)
{
    size_t totalIter = (xmax - xmin) / delX;
    size_t logMultiple = std::max(totalIter / 200, 1ULL);

    for (size_t i = threadIndex; i < totalIter; i += numThreads)
    {
        // Evaluate function at value and store sequence in 'seqVals'
        std::vector<std::complex<double>> seqVals;
        Eval({ xmin + delX * i, 0 }, seqVals, preIter, postIter);

        for (const std::complex<double>& v : seqVals)
        {
            size_t pixX = i * w / totalIter;
            size_t pixY = h - (abs(v) - ymin) / (ymax - ymin) * h;
            if (pixX < w && pixY >= 0 && pixY < h)
                bmp.set_pixel(pixX, pixY, 0, 0, 0);
        }
    }
}

int main()
{
    // Initialize image
    bitmap_image bmp(w, h);
    bmp.clear(255);

    // Prepare thread vector
    int threadNum = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(threadNum);

    TIMER(t);
    std::cout << "Calculating..." << std::endl;

    // Initialize threads
    for (int t = 0; t < threadNum; t++)
        threads.emplace_back(StrideComputeValues, std::ref(bmp), t, threadNum);

    // Wait for threads
    for (int t = 0; t < threadNum; t++)
        threads[t].join();
    
    STOP_LOG(t);

    std::cout << "Saving...\n";
    bmp.save_image("C:\\Users\\matty\\Desktop\\bi.bmp");
    std::cout << "Saved.\n";
}
#include <iostream>
#include <iomanip>
#include <complex>
#include <thread>

#include "../bitmap_image.hpp"
#include "../Timer.h"
#include "../eval.h"

// ===== Parameters =====
static constexpr size_t w = 2048, h = 2048;
static constexpr double xmin = -10 , xmax = 10, ymin = -10, ymax = 10;
static constexpr size_t iter = 2000;
static constexpr double threshold = 1000.0;

void StrideComputePixels(bitmap_image& bmp, int threadIndex, int numThreads)
{
    int imgW = bmp.width();
    int imgH = bmp.height();

    size_t index = 0;
    for (int yi = 0; yi < h; yi++)
    {
        for (int xi = 0; xi < w; xi++)
        {
            // Log progress (based on thread 0)
            if (threadIndex == 0 && index % 10000 == 0)
                std::cout << std::setprecision(3) << 100.0 * index / (w * h) << "%" << "\t\r";

            // Check if pixel must be calculated
            if (index % numThreads == threadIndex)
            {
                std::complex<double> c
                    { (xmax - xmin) / w * xi + xmin,
                    (ymax - ymin) / h * yi + ymin };

                if (Converges(c, iter, threshold)) bmp.set_pixel(xi, h - yi - 1, 0, 0, 0);
                else bmp.set_pixel(xi, h - yi - 1, 255, 255, 255);
            }
            index++;
        }
    }
}

int main()
{
    // Initialize image
    bitmap_image bmp(w, h);

    // Prepare thread vector
    int threadNum = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(threadNum);

    TIMER(calculation);
    std::cout << "Calculating..." << std::endl;

    // Initialize threads
    for (int t = 0; t < threadNum; t++)
        threads.emplace_back(StrideComputePixels, std::ref(bmp), t, threadNum);

    // Wait for threads
    for (int t = 0; t < threadNum; t++)
        threads[t].join();

    STOP_LOG(calculation);

    std::cout << "Saving...\n";
    bmp.save_image("C:\\Users\\matty\\Desktop\\output.bmp");
    std::cout << "Saved.\n";
}
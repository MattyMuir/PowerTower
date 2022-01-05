#include <iostream>
#include <iomanip>
#include <complex>
#include <thread>
using namespace std::complex_literals;

#include "bitmap_image.hpp"
#include "Timer.h"
#include "SimpleSet.h"

#define IsFinite(z) (std::isfinite(z.real()) && std::isfinite(z.imag()))
#define E 2.71828182845904523536

//double xmin = -4.15, xmax = -4.10, ymin = -0.025, ymax = 0.025;
double xmin = -5 , xmax = 5, ymin = -5, ymax = 5;
int iter = 500;
double threshold = 1000;

void Branch(bitmap_image& bmp, int branchIndex, int branchNum)
{
    int h = bmp.height();
    int w = bmp.width();

    double ep = 1e-7;
    auto hash = [](const std::complex<long double>& obj) { return obj.imag() * 723657 + obj.real() * 2938756; };
    auto pred = [ep](const std::complex<long double>& obj1, const std::complex<long double>& obj2) { return abs(obj1.real() - obj2.real()) < ep && abs(obj1.imag() - obj2.imag()) < ep; };
    DeclareSet(set, std::complex<long double>, 8);

    std::complex<long double> c, z;
    int index = 0;
    for (int yi = 0; yi < h; yi++)
    {
        for (int xi = 0; xi < w; xi++)
        {
            if (branchIndex == 0 && index % 10000 == 0)
            {
                std::cout << std::setprecision(3) << 100.0 * index / (w * h) << "%" << "\t\r";
            }
            if (index % branchNum == branchIndex)
            {
                set.clear();
                c = (xmax - xmin) / w * xi + xmin + ((ymax - ymin) / h * yi + ymin) * 1i;
                z = c;

                for (int i = 0; i < iter; i++)
                {
                    z = pow(c, z);
                    if (!IsFinite(z))
                    {
                        break;
                    }
                    if (set.contains(z) && IsFinite(z))
                    {
                        bmp.set_pixel(xi, h - yi - 1, 0, 0, 0);
                        break;
                    }
                    set.insert(z);
                }
                if (abs(z) < threshold)
                {
                    bmp.set_pixel(xi, h - yi - 1, 0, 0, 0);
                }
            }
            index++;
        }
    }
}

int main()
{
    int w = 3840, h = 3840;
    bitmap_image bmp(w, h);
    bmp.clear(255);

    int threadNum = std::thread::hardware_concurrency() - 1;
    std::vector<std::thread> threads;
    threads.reserve(threadNum);

    Timer t;
    std::cout << "Calculating..." << std::endl;
    for (int t = 0; t < threadNum; t++)
    {
        threads.emplace_back(Branch, std::ref(bmp), t, threadNum);
    }
    for (int t = 0; t < threadNum; t++)
    {
        threads[t].join();
    }
    t.Stop();

    std::cout << std::setprecision(5) << "Took: " << t.duration * 0.001 << "ms" << std::endl;
    std::cout << "Saving..." << std::endl;
    bmp.save_image("C:\\Users\\matty\\Desktop\\conv.bmp");
}
#include <iostream>
#include <complex>
using namespace std::complex_literals;

#include "bitmap_image.hpp"
#include "Timer.h"

int main()
{
    double xmin = -5, xmax = 3, ymin = -1, ymax = 10;
    //double xmin = -4.2, xmax = -4, ymin = -1, ymax = 10;
    int w = 3840, h = 3840;
    bitmap_image bmp(w, h);
    bmp.clear(255);
    
    int preIter = 1000;
    int postIter = 1000;

    Timer t;
    std::complex<long double> c, z;
    for (int xi = 0; xi < w; xi++)
    {
        c = (xmax - xmin) / w * xi + xmin + 0.001i;
        z = c;

        for (int i = 0; i < preIter; i++)
        {
            z = pow(c, z);
        }
        for (int i = 0; i < postIter; i++)
        {
            z = pow(c, z);

            int y = h - (abs(z) - ymin) / (ymax - ymin) * h;
            if (y >= 0 && y < h)
            {
                bmp.set_pixel(xi, y, 0, 0, 0);
            }
        }
    }
    t.Stop();

    std::cout << "Saving..." << std::endl;
    bmp.save_image("C:\\Users\\matty\\Desktop\\bi.bmp");
    std::cout << "Math took: " << t.duration * 0.001 << "ms" << std::endl;
}
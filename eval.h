#pragma once
#include <vector>
#include <unordered_set>
#include <complex>

static bool IsFinite(const std::complex<double>& z)
{
    return std::isfinite(z.real()) && std::isfinite(z.imag());
}

static bool Converges(std::complex<double> c, size_t iter, double threshold)
{
    // Hash function for complex numbers
    auto hash = [](const std::complex<double>& obj)
    {
        size_t h1 = std::hash<double>{}(obj.real());
        size_t h2 = std::hash<double>{}(obj.imag());

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    };

    // Declare set to contain values in the sequence
    std::unordered_set<std::complex<double>, decltype(hash)> seqVals{ 1, hash };

    std::complex<double> z = c;
    for (size_t i = 0; i < iter; i++)
    {
        z = pow(c, z);
        if (!IsFinite(z)) return false;
        if (seqVals.contains(z)) return true;
        seqVals.insert(z);
    }
    if (abs(z) < threshold) return true;
    else return false;
}

static void Eval(std::complex<double> c, std::vector<std::complex<double>>& values, size_t preIter, size_t postIter)
{
    values.clear();

    // Hash function for complex numbers
    auto hash = [](const std::complex<double>& obj)
    {
        size_t h1 = std::hash<double>{}(obj.real());
        size_t h2 = std::hash<double>{}(obj.imag());

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    };

    // Declare set to contain values in the sequence
    std::unordered_set<std::complex<double>, decltype(hash)> seqVals{ 1, hash };

    std::complex<double> z = c;

    // Perform pre-iterations
    for (int i = 0; i < preIter; i++)
        z = pow(c, z);

    // Perform post-iterations
    for (int i = 0; i < postIter; i++)
    {
        z = pow(c, z);
        if (!IsFinite(z)) return;
        if (seqVals.contains(z)) break;
        seqVals.insert(z);
    }

    values.reserve(seqVals.size());
    for (const std::complex<double>& seqVal : seqVals)
        values.push_back(seqVal);
}
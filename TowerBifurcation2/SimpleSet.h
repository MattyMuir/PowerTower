#pragma once
#include <iostream>
#include <unordered_set>

/*
auto hash = [](const T& obj) { return 0; };
auto pred = [](const T& obj1, const T& obj2) { return false; };
*/

#define DeclareSet(varName, T, bits) std::unordered_set<T, decltype(hash), decltype(pred)> varName(1 << bits, hash, pred); varName.max_load_factor(DBL_MAX)

template<typename T, typename... Args>
void PrintSet(const std::unordered_set<T, Args...>& set)
{
	for (size_t bno = 0; bno < set.bucket_count(); ++bno)
	{
		std::cout << bno << ": ";
		for (auto bit = set.begin(bno), end = set.end(bno); bit != end; ++bit)
		{
			const auto& element = *bit;
			std::cout << element << ", ";
		}
		std::cout << std::endl;
	}
}
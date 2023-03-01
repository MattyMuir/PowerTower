#pragma once
#include <iostream>
#include <string>
#include <mpir.h>

class Complex
{
public:
	void Initialize(size_t bits_);

	Complex(size_t bits_ = 64); // Default constructor
	Complex(const Complex& other); // Copy contructor
	Complex(uint64_t rp, uint64_t rq, uint64_t ip, uint64_t iq, size_t bits_ = 64);
	Complex(const std::string& rstr, const std::string& istr, size_t bits_ = 64);

	~Complex();

	friend std::ostream& operator<<(std::ostream& os, const Complex& dt);
private:
	int bits;
	mpf_t r, i;
};
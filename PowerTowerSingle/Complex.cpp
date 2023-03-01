#include "Complex.h"

void Complex::Initialize(size_t bits_)
{
	bits = bits_;
	mpf_init2(r, bits);
	mpf_init2(i, bits);
}

Complex::Complex(size_t bits_)
{
	Initialize(bits_);
}

Complex::Complex(const Complex& other)
{
	std::cout << "Copying.\n";
	Initialize(other.bits);

	mpf_set(r, other.r);
	mpf_set(i, other.i);
}

Complex::Complex(uint64_t rp, uint64_t rq, uint64_t ip, uint64_t iq, size_t bits_)
{
	Initialize(bits_);
	mpf_set_ui(r, rp);
	mpf_div_ui(r, r, rq);

	mpf_set_ui(i, ip);
	mpf_div_ui(i, i, iq);
}

Complex::Complex(const std::string& rstr, const std::string& istr, size_t bits_)
{
	Initialize(bits_);
	mpf_set_str(r, rstr.c_str(), 10);
	mpf_set_str(i, istr.c_str(), 10);
}

Complex::~Complex()
{
	mpf_clear(r);
	mpf_clear(i);
}

std::ostream& operator<<(std::ostream& os, const Complex& dt)
{
	// TODO: insert return statement here
	return os;
}
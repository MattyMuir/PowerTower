#include <iostream>
#include <iomanip>

#include <mpir.h>
#include "Complex.h"
#include "MpfExtensions.h"
#include "../Timer.h"

#define PREC 11

int main()
{
	mpf_t x, tau;

	// Compute tau
	mpf_init2(tau, 1 << PREC);
	Pi(tau);
	mpf_mul_ui(tau, tau, 2);

	mpf_init2(x, 1 << PREC);
	mpf_set_ui(x, 10);

	// Compute sin
	mpf_sin(x, x, &tau);
	std::cout << std::setprecision(100000000) << x << "\n";
	mpf_mul_ui(x, x, 2);
	mpf_sin(x, x, &tau);
	std::cout << std::setprecision(100000000) << x << "\n";

	std::cin.get();
	mpf_clear(x);
	mpf_clear(tau);
}
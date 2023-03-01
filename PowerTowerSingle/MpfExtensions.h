#pragma once
#include <mpir.h>

void Pi(mpf_t& result)
{
	size_t bits = mpf_get_prec(result);
	mpf_set_ui(result, 0);

	mpf_t term, temp1, temp2;
	mpf_init2(term, bits);
	mpf_init2(temp1, bits);
	mpf_init2(temp2, bits);

	for (int k = 0; k <= bits / 4; k++)
	{
		mpf_set_ui(term, 1);
		for (int i = 0; i < k; i++)
			mpf_div_ui(term, term, 16);

		mpf_set_ui(temp1, 0);

		mpf_set_ui(temp2, 4);
		mpf_div_ui(temp2, temp2, 8 * k + 1);

		mpf_add(temp1, temp1, temp2);

		mpf_set_ui(temp2, 2);
		mpf_div_ui(temp2, temp2, 8 * k + 4);

		mpf_sub(temp1, temp1, temp2);

		mpf_set_ui(temp2, 1);
		mpf_div_ui(temp2, temp2, 8 * k + 5);

		mpf_sub(temp1, temp1, temp2);

		mpf_set_ui(temp2, 1);
		mpf_div_ui(temp2, temp2, 8 * k + 6);

		mpf_sub(temp1, temp1, temp2);

		mpf_mul(term, term, temp1);

		mpf_add(result, result, term);
	}

	mpf_clear(term);
	mpf_clear(temp1);
	mpf_clear(temp2);
}

double mpf_log2(const mpf_t& x)
{
	signed long exp;
	double d = mpf_get_d_2exp(&exp, x);

	return log2(d) + exp;
}

void mpf_sin(mpf_t& result, const mpf_t& x, mpf_t* tauPtr = nullptr)
{
	size_t bits = mpf_get_prec(x);
	mpf_t temp, term, tau;
	mpz_t fac;
	mpf_init2(temp, bits);
	mpf_init2(term, bits);
	
	mpz_init(fac);

	// Get x into range (0 - tau) and save in temp
	// Calculate or retrieve tau
	if (tauPtr)
	{
		mpf_init_set(tau, *tauPtr);
	}
	else
	{
		mpf_init2(tau, bits);
		Pi(tau);
		mpf_mul_ui(tau, tau, 2);
	}

	mpf_div(temp, x, tau);
	mpf_floor(temp, temp);
	mpf_mul(temp, temp, tau);

	mpf_sub(temp, x, temp);

	// Calculate sin(temp) using the taylor series
	mpf_set_ui(result, 0);
	int k = 1;
	bool pos = true;
	while (true)
	{
		mpf_pow_ui(term, temp, k);
		mpz_fac_ui(fac, k);
		mpf_set_z(tau, fac);
		mpf_div(term, term, tau);

		if (-mpf_log2(term) > bits) { break; }

		if (!pos) { mpf_neg(term, term); }

		mpf_add(result, result, term);

		pos = !pos;
		k += 2;
	}

	mpf_clear(temp);
	mpf_clear(term);
	mpf_clear(tau);
	mpz_clear(fac);
}
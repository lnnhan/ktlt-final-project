#pragma once
#include <string>
#include <vector>
#include <utility>
#include <stdexcept>

// ============================================================
//  DATA CONTRACTS
// ============================================================

struct Fraction {
    int num, den;  // invariant: den > 0, gcd(|num|, den) == 1
};

struct Polynomial {
    std::string id;
    int degree;
    std::vector<Fraction> coeffs;  // coeffs[i] = coefficient of x^i
};

struct Experiment {
    std::string name;
    std::vector<Polynomial> polys;
    std::vector<double>     x_vals;
};

// ============================================================
//  LAYER 0 — Fraction Operations
// ============================================================

int      gcd(int a, int b);
Fraction make_fraction(int num, int den);           // throws if den == 0
Fraction add_fractions(Fraction a, Fraction b);
Fraction multiply_fractions(Fraction a, Fraction b);
bool     equal_fractions(Fraction a, Fraction b);
std::string fraction_to_string(Fraction f);

// ============================================================
//  LAYER 1 — Polynomial Operations
// ============================================================

double     evaluate_polynomial(const Polynomial& p, double x);          // Horner
Polynomial differentiate_polynomial(const Polynomial& p);
Polynomial add_polynomials(const Polynomial& a, const Polynomial& b);
std::string polynomial_to_string(const Polynomial& p);

// ============================================================
//  LAYER 2 — Array / Subarray Algorithms
// ============================================================

std::vector<double> build_px_array(const Polynomial& p, const std::vector<double>& xs);
std::pair<int,int>  kadane_max_subarray(const std::vector<double>& arr);   // returns {l, r}
std::vector<int>    longest_increasing_run(const std::vector<double>& arr); // indices of longest run
std::vector<double> build_prefix_sum(const std::vector<double>& arr);
double              query_range_sum(const std::vector<double>& prefix, int l, int r);

// ============================================================
//  LAYER 3 — Statistics
// ============================================================

double              compute_mean(const std::vector<double>& arr);
double              compute_std(const std::vector<double>& arr, double mean);
std::vector<double> zscore_normalize(const std::vector<double>& arr);
std::string         compare_polynomials(const Polynomial& a, const Polynomial& b,
                                        const std::vector<double>& xs);

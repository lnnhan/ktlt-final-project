#include "processing.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <climits>

// ============================================================
//  LAYER 0 — Fraction Operations
// ============================================================

// Euclidean GCD (always returns positive value)
int gcd(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return (a == 0) ? 1 : a;
}

// ASSUMPTION: den == 0 is treated as an error (bad CSV data)
Fraction make_fraction(int num, int den) {
    if (den == 0)
        throw std::invalid_argument("Fraction: denominator cannot be zero");

    // Normalise: keep den > 0
    if (den < 0) { num = -num; den = -den; }

    int g = gcd(std::abs(num), den);
    return Fraction{num / g, den / g};
}

Fraction add_fractions(Fraction a, Fraction b) {
    // a.num/a.den + b.num/b.den = (a.num*b.den + b.num*a.den) / (a.den*b.den)
    int num = a.num * b.den + b.num * a.den;
    int den = a.den * b.den;
    return make_fraction(num, den);
}

Fraction multiply_fractions(Fraction a, Fraction b) {
    return make_fraction(a.num * b.num, a.den * b.den);
}

bool equal_fractions(Fraction a, Fraction b) {
    // Both already normalised, so cross-multiply
    return (a.num * b.den) == (b.num * a.den);
}

std::string fraction_to_string(Fraction f) {
    if (f.den == 1) return std::to_string(f.num);
    return std::to_string(f.num) + "/" + std::to_string(f.den);
}

// ============================================================
//  LAYER 1 — Polynomial Operations
// ============================================================

// Horner's method: P(x) = a0 + x*(a1 + x*(a2 + ... + x*an))
double evaluate_polynomial(const Polynomial& p, double x) {
    double result = 0.0;
    // Iterate from highest degree down
    for (int i = p.degree; i >= 0; --i) {
        double coeff = static_cast<double>(p.coeffs[i].num) / p.coeffs[i].den;
        result = result * x + coeff;
    }
    return result;
}

// P'(x): coefficient of x^i in P'(x) is (i+1) * coeffs[i+1] from P(x)
Polynomial differentiate_polynomial(const Polynomial& p) {
    Polynomial deriv;
    deriv.id     = p.id + "'";
    deriv.degree = (p.degree > 0) ? p.degree - 1 : 0;

    if (p.degree == 0) {
        // Derivative of constant is 0
        deriv.coeffs.push_back(make_fraction(0, 1));
        return deriv;
    }

    for (int i = 1; i <= p.degree; ++i) {
        // d/dx [a_i * x^i] = i * a_i * x^(i-1)
        Fraction coeff = multiply_fractions(p.coeffs[i], make_fraction(i, 1));
        deriv.coeffs.push_back(coeff);
    }
    return deriv;
}

// Add two polynomials with Fraction coefficients
Polynomial add_polynomials(const Polynomial& a, const Polynomial& b) {
    Polynomial result;
    result.id     = "(" + a.id + "+" + b.id + ")";
    result.degree = std::max(a.degree, b.degree);

    int max_terms = result.degree + 1;
    Fraction zero = make_fraction(0, 1);

    for (int i = 0; i < max_terms; ++i) {
        Fraction ca = (i < static_cast<int>(a.coeffs.size())) ? a.coeffs[i] : zero;
        Fraction cb = (i < static_cast<int>(b.coeffs.size())) ? b.coeffs[i] : zero;
        result.coeffs.push_back(add_fractions(ca, cb));
    }
    return result;
}

std::string polynomial_to_string(const Polynomial& p) {
    std::ostringstream oss;
    oss << p.id << " [deg=" << p.degree << "]: ";
    for (int i = p.degree; i >= 0; --i) {
        oss << "(" << fraction_to_string(p.coeffs[i]) << ")x^" << i;
        if (i > 0) oss << " + ";
    }
    return oss.str();
}

// ============================================================
//  LAYER 2 — Array / Subarray Algorithms
// ============================================================

std::vector<double> build_px_array(const Polynomial& p, const std::vector<double>& xs) {
    std::vector<double> result;
    result.reserve(xs.size());
    for (double x : xs)
        result.push_back(evaluate_polynomial(p, x));
    return result;
}

// Kadane's algorithm — returns {l, r} indices of max-sum subarray
std::pair<int,int> kadane_max_subarray(const std::vector<double>& arr) {
    if (arr.empty()) return {-1, -1};

    double max_sum    = arr[0];
    double curr_sum   = arr[0];
    int best_l = 0, best_r = 0;
    int curr_l = 0;

    for (int i = 1; i < static_cast<int>(arr.size()); ++i) {
        if (curr_sum + arr[i] < arr[i]) {
            curr_sum = arr[i];
            curr_l   = i;
        } else {
            curr_sum += arr[i];
        }

        if (curr_sum > max_sum) {
            max_sum = curr_sum;
            best_l  = curr_l;
            best_r  = i;
        }
    }
    return {best_l, best_r};
}

// LIS-style: find longest consecutive run where arr[i] < arr[i+1]
// Returns vector of indices in that run
std::vector<int> longest_increasing_run(const std::vector<double>& arr) {
    if (arr.empty()) return {};

    int best_start = 0, best_len = 1;
    int curr_start = 0, curr_len = 1;

    for (int i = 1; i < static_cast<int>(arr.size()); ++i) {
        if (arr[i] > arr[i - 1]) {
            ++curr_len;
        } else {
            if (curr_len > best_len) {
                best_len   = curr_len;
                best_start = curr_start;
            }
            curr_start = i;
            curr_len   = 1;
        }
    }
    // Check last run
    if (curr_len > best_len) {
        best_len   = curr_len;
        best_start = curr_start;
    }

    std::vector<int> indices;
    for (int i = best_start; i < best_start + best_len; ++i)
        indices.push_back(i);
    return indices;
}

// prefix[i] = arr[0] + arr[1] + ... + arr[i]
std::vector<double> build_prefix_sum(const std::vector<double>& arr) {
    std::vector<double> prefix(arr.size());
    if (arr.empty()) return prefix;
    prefix[0] = arr[0];
    for (int i = 1; i < static_cast<int>(arr.size()); ++i)
        prefix[i] = prefix[i - 1] + arr[i];
    return prefix;
}

// Sum of arr[l..r] in O(1)
double query_range_sum(const std::vector<double>& prefix, int l, int r) {
    if (l > r || l < 0 || r >= static_cast<int>(prefix.size()))
        throw std::out_of_range("query_range_sum: index out of bounds");
    if (l == 0) return prefix[r];
    return prefix[r] - prefix[l - 1];
}

// ============================================================
//  LAYER 3 — Statistics
// ============================================================

double compute_mean(const std::vector<double>& arr) {
    if (arr.empty()) return 0.0;
    double sum = std::accumulate(arr.begin(), arr.end(), 0.0);
    return sum / static_cast<double>(arr.size());
}

double compute_std(const std::vector<double>& arr, double mean) {
    if (arr.size() <= 1) return 0.0;
    double variance = 0.0;
    for (double v : arr)
        variance += (v - mean) * (v - mean);
    return std::sqrt(variance / static_cast<double>(arr.size()));
}

std::vector<double> zscore_normalize(const std::vector<double>& arr) {
    double mean = compute_mean(arr);
    double std  = compute_std(arr, mean);
    std::vector<double> normalized;
    normalized.reserve(arr.size());

    // ASSUMPTION: if std == 0 (all values identical), z-score is 0 for all
    for (double v : arr)
        normalized.push_back((std < 1e-12) ? 0.0 : (v - mean) / std);
    return normalized;
}

std::string compare_polynomials(const Polynomial& a, const Polynomial& b,
                                const std::vector<double>& xs) {
    double mean_a = compute_mean(build_px_array(a, xs));
    double mean_b = compute_mean(build_px_array(b, xs));

    if (std::abs(mean_a - mean_b) < 1e-9)
        return a.id + " == " + b.id;
    return (mean_a > mean_b)
        ? a.id + " > " + b.id
        : a.id + " < " + b.id;
}

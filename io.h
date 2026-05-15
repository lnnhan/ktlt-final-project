#pragma once
#include "processing.h"
#include <string>
#include <vector>

// ============================================================
//  LAYER 4 — IO (CSV read / write)
// ============================================================

// Parse a single token "num/den", "num", or "" → Fraction
// "" or missing  → {0, 1}
// "n/0"          → throws invalid_argument
Fraction parse_fraction_token(const std::string& token);

// Parse one CSV data row → Polynomial
// Throws if row is malformed or any denominator is 0
Polynomial parse_polynomial_row(const std::string& csv_line);

// Read polynomials.csv → vector<Polynomial>
// Skips header row; skips rows with invalid fractions (logs warning)
std::vector<Polynomial> read_polynomials_csv(const std::string& filepath);

// Read x_values.csv → vector<double> (skips header)
std::vector<double> read_x_values_csv(const std::string& filepath);

// Write poly_report.csv:
//   columns: poly_id, x_val, P(x), P_prime(x)
void write_poly_report_csv(const std::string& filepath, const Experiment& exp);

// Write extrema.txt:
//   For each poly: Kadane result (x_start, x_end, sum), LIS run, prefix-sum demo
void write_extrema_txt(const std::string& filepath, const Experiment& exp);

// Write compare.csv:
//   columns: poly_id, mean_Px, std_Px, comparison vs every other poly
void write_compare_csv(const std::string& filepath, const Experiment& exp);

// Print summary to stdout (stage logging)
void print_experiment_summary(const Experiment& exp);

#include "io.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ─── helpers ────────────────────────────────────────────────

static std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        // trim carriage-return (Windows line endings)
        if (!token.empty() && token.back() == '\r')
            token.pop_back();
        tokens.push_back(token);
    }
    return tokens;
}

// ============================================================
//  F20 — parse_fraction_token
// ============================================================

Fraction parse_fraction_token(const std::string& token) {
    // Empty or whitespace → 0/1
    if (token.empty() || token == " ")
        return make_fraction(0, 1);

    size_t slash = token.find('/');
    if (slash == std::string::npos) {
        // Plain integer
        int num = std::stoi(token);
        return make_fraction(num, 1);
    }

    int num = std::stoi(token.substr(0, slash));
    int den = std::stoi(token.substr(slash + 1));
    // make_fraction will throw if den == 0
    return make_fraction(num, den);
}

// ============================================================
//  F21 — parse_polynomial_row
// ============================================================

Polynomial parse_polynomial_row(const std::string& csv_line) {
    auto tokens = split_csv_line(csv_line);
    if (tokens.size() < 3)
        throw std::invalid_argument("parse_polynomial_row: too few columns in: " + csv_line);

    Polynomial poly;
    poly.id     = tokens[0];
    poly.degree = std::stoi(tokens[1]);

    int num_coeffs = poly.degree + 1;  // need a0 … a_degree
    Fraction zero  = make_fraction(0, 1);

    for (int i = 0; i < num_coeffs; ++i) {
        int col = i + 2;  // offset: id=0, degree=1, coeffs start at 2
        if (col < static_cast<int>(tokens.size()) && !tokens[col].empty()) {
            // throws invalid_argument if den == 0
            poly.coeffs.push_back(parse_fraction_token(tokens[col]));
        } else {
            // Missing column → treat as 0/1
            poly.coeffs.push_back(zero);
        }
    }
    return poly;
}

// ============================================================
//  F22 — read_polynomials_csv
// ============================================================

std::vector<Polynomial> read_polynomials_csv(const std::string& filepath) {
    std::cout << "[IO] Reading polynomials from: " << filepath << "\n";

    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("read_polynomials_csv: cannot open " + filepath);

    std::vector<Polynomial> polys;
    std::string line;
    int row = 0;

    while (std::getline(file, line)) {
        ++row;
        if (row == 1) continue;  // skip header
        if (line.empty() || line == "\r") continue;

        try {
            polys.push_back(parse_polynomial_row(line));
        } catch (const std::exception& e) {
            // Bad fraction (den=0) or parse error → skip row, warn
            std::cerr << "[IO] WARNING row " << row << " skipped — " << e.what() << "\n";
        }
    }

    std::cout << "[IO] Loaded " << polys.size() << " valid polynomial(s)\n";
    return polys;
}

// ============================================================
//  F23 — read_x_values_csv
// ============================================================

std::vector<double> read_x_values_csv(const std::string& filepath) {
    std::cout << "[IO] Reading x values from: " << filepath << "\n";

    std::ifstream file(filepath);
    if (!file.is_open())
        throw std::runtime_error("read_x_values_csv: cannot open " + filepath);

    std::vector<double> xs;
    std::string line;
    int row = 0;

    while (std::getline(file, line)) {
        ++row;
        if (row == 1) continue;  // skip header
        if (line.empty() || line == "\r") continue;

        if (!line.empty() && line.back() == '\r') line.pop_back();
        try {
            xs.push_back(std::stod(line));
        } catch (...) {
            std::cerr << "[IO] WARNING: invalid x value at row " << row << ": " << line << "\n";
        }
    }

    std::cout << "[IO] Loaded " << xs.size() << " x value(s)\n";
    return xs;
}

// ============================================================
//  F24 — write_poly_report_csv
// ============================================================

void write_poly_report_csv(const std::string& filepath, const Experiment& exp) {
    std::cout << "[IO] Writing poly_report to: " << filepath << "\n";

    std::ofstream out(filepath);
    if (!out.is_open())
        throw std::runtime_error("write_poly_report_csv: cannot open " + filepath);

    out << std::fixed << std::setprecision(6);
    out << "poly_id,x_val,P(x),P_prime(x)\n";

    for (const Polynomial& p : exp.polys) {
        Polynomial dp = differentiate_polynomial(p);
        for (double x : exp.x_vals) {
            double px  = evaluate_polynomial(p,  x);
            double dpx = evaluate_polynomial(dp, x);
            out << p.id << "," << x << "," << px << "," << dpx << "\n";
        }
    }

    std::cout << "[IO] poly_report written\n";
}

// ============================================================
//  F25 — write_extrema_txt
// ============================================================

void write_extrema_txt(const std::string& filepath, const Experiment& exp) {
    std::cout << "[IO] Writing extrema to: " << filepath << "\n";

    std::ofstream out(filepath);
    if (!out.is_open())
        throw std::runtime_error("write_extrema_txt: cannot open " + filepath);

    out << std::fixed << std::setprecision(6);

    for (const Polynomial& p : exp.polys) {
        std::vector<double> px_arr = build_px_array(p, exp.x_vals);
        std::vector<double> prefix = build_prefix_sum(px_arr);

        // Kadane
        auto [l, r] = kadane_max_subarray(px_arr);
        double kadane_sum = (l >= 0) ? query_range_sum(prefix, l, r) : 0.0;

        // Longest increasing run
        std::vector<int> lis_idx = longest_increasing_run(px_arr);

        out << "=== " << p.id << " ===\n";

        // Kadane result
        if (l >= 0) {
            out << "  [Kadane] max-sum subarray x["
                << l << ".." << r << "] = ["
                << exp.x_vals[l] << ", " << exp.x_vals[r] << "]"
                << "  sum=" << kadane_sum << "\n";
        } else {
            out << "  [Kadane] no valid subarray\n";
        }

        // Prefix-sum demo: sum of entire range
        double total = prefix.empty() ? 0.0 : query_range_sum(prefix, 0, (int)prefix.size()-1);
        out << "  [PrefixSum] total sum over all x = " << total << "\n";

        // LIS run
        out << "  [LIS-run] longest increasing run length=" << lis_idx.size();
        if (!lis_idx.empty()) {
            out << "  x_indices=[";
            for (int i = 0; i < (int)lis_idx.size(); ++i) {
                out << exp.x_vals[lis_idx[i]];
                if (i + 1 < (int)lis_idx.size()) out << ", ";
            }
            out << "]";
        }
        out << "\n\n";
    }

    std::cout << "[IO] extrema.txt written\n";
}

// ============================================================
//  F26 — write_compare_csv
// ============================================================

void write_compare_csv(const std::string& filepath, const Experiment& exp) {
    std::cout << "[IO] Writing compare to: " << filepath << "\n";

    std::ofstream out(filepath);
    if (!out.is_open())
        throw std::runtime_error("write_compare_csv: cannot open " + filepath);

    out << std::fixed << std::setprecision(6);
    out << "poly_id,mean_Px,std_Px,zscore_sample_x0\n";

    for (const Polynomial& p : exp.polys) {
        std::vector<double> px_arr   = build_px_array(p, exp.x_vals);
        double mean                  = compute_mean(px_arr);
        double std_val               = compute_std(px_arr, mean);
        std::vector<double> zscores  = zscore_normalize(px_arr);

        double z0 = zscores.empty() ? 0.0 : zscores[0];
        out << p.id << "," << mean << "," << std_val << "," << z0 << "\n";
    }

    // Pairwise comparison section
    out << "\npoly_a,poly_b,comparison\n";
    for (int i = 0; i < (int)exp.polys.size(); ++i) {
        for (int j = i + 1; j < (int)exp.polys.size(); ++j) {
            std::string cmp = compare_polynomials(exp.polys[i], exp.polys[j], exp.x_vals);
            out << exp.polys[i].id << "," << exp.polys[j].id << "," << cmp << "\n";
        }
    }

    std::cout << "[IO] compare.csv written\n";
}

// ============================================================
//  F27 — print_experiment_summary
// ============================================================

void print_experiment_summary(const Experiment& exp) {
    std::cout << "\n[IO] === Experiment Summary: " << exp.name << " ===\n";
    std::cout << "[IO]   Polynomials loaded : " << exp.polys.size()   << "\n";
    std::cout << "[IO]   x values loaded    : " << exp.x_vals.size()  << "\n";
    std::cout << "[IO]   x range            : ["
              << (exp.x_vals.empty() ? 0.0 : exp.x_vals.front()) << ", "
              << (exp.x_vals.empty() ? 0.0 : exp.x_vals.back())  << "]\n";
    for (const Polynomial& p : exp.polys)
        std::cout << "[IO]   " << polynomial_to_string(p) << "\n";
    std::cout << "[IO] ====================================\n\n";
}

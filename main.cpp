#include "io.h"
#include "processing.h"
#include <iostream>

// ============================================================
//  PIPELINE — main.cpp
//
//  Order:
//   [1] Read polynomials.csv
//   [2] Read x_values.csv
//   [3] Assemble Experiment
//   [4] Print summary
//   [5] Write poly_report.csv
//   [6] Write extrema.txt
//   [7] Write compare.csv
// ============================================================

int main() {
    // ── Config (no hardcoded paths beyond project-local defaults) ──
    const std::string poly_input   = "polynomials-3.csv";
    const std::string x_input      = "x_values-3.csv";
    const std::string report_out   = "poly_report.csv";
    const std::string extrema_out  = "extrema.txt";
    const std::string compare_out  = "compare.csv";

    try {
        // [1] Read input files
        std::vector<Polynomial> polys = read_polynomials_csv(poly_input);
        std::vector<double>     xs    = read_x_values_csv(x_input);

        // [2] Assemble Experiment
        Experiment exp;
        exp.name   = "BTL-3";
        exp.polys  = polys;
        exp.x_vals = xs;

        // [3] Print summary to console
        print_experiment_summary(exp);

        // [4] Generate outputs
        write_poly_report_csv(report_out,  exp);
        write_extrema_txt    (extrema_out, exp);
        write_compare_csv    (compare_out, exp);

        std::cout << "\n[DONE] All outputs written successfully.\n";

    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    }

    return 0;
}

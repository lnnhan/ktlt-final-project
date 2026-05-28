# Decompose — BTL Đa Thức Hệ Số Phân Số

> Project: Thư viện đa thức hệ số phân số & Mô phỏng quỹ đạo  
> MSSV:
> 
> 23280055 - Lưu Nguyễn Ngọc Hân
> 24280049 - Hoàng Gia Bảo  
> Kiến trúc: `io.h/.cpp` · `processing.h/.cpp` · `main.cpp`

---

## 📁 Cấu trúc File Output

```
BTL/
├── io.h / io.cpp              ← Đọc CSV, ghi output
├── processing.h / processing.cpp  ← Logic toán học
├── main.cpp                   ← Pipeline điều phối
├── polynomials-3.csv          ← Input (đã có)
├── x_values-3.csv             ← Input (đã có)
├── poly_report.csv            ← Output: P(x), P'(x) từng đa thức
├── extrema.txt                ← Output: đoạn x cực trị (Kadane)
└── compare.csv                ← Output: so sánh các đa thức
```

---

## 🏗️ Data Contracts (Struct)

```cpp
struct Fraction {
    int num, den;   // mẫu luôn > 0, đã rút gọn qua gcd
};

struct Polynomial {
    string id;
    int degree;
    vector<Fraction> coeffs;  // coeffs[0]=a0, coeffs[1]=a1, ...
};

struct Experiment {
    string name;
    vector<Polynomial> polys;
    vector<double> x_vals;
};
```

---

## 📐 Decomposition Chi Tiết

### ── LAYER 0: Struct & Fraction (`processing.h/.cpp`) ──

| # | Hàm | Input | Output | Ghi chú |
|---|-----|-------|--------|---------|
| F01 | `int gcd(int a, int b)` | 2 số nguyên | ước chung lớn nhất | Euclid algorithm |
| F02 | `Fraction make_fraction(int num, int den)` | num, den | Fraction đã tối giản | den=0 → throw; mẫu âm → đổi dấu |
| F03 | `Fraction add_fractions(Fraction a, Fraction b)` | 2 Fraction | Fraction tổng | a/b + c/d = (ad+bc)/bd, rút gọn |
| F04 | `Fraction multiply_fractions(Fraction a, Fraction b)` | 2 Fraction | Fraction tích | a/b * c/d = ac/bd, rút gọn |
| F05 | `bool equal_fractions(Fraction a, Fraction b)` | 2 Fraction | bool | a/b == c/d |
| F06 | `string fraction_to_string(Fraction f)` | Fraction | "num/den" hoặc "num" | dùng in ra CSV |

---

### ── LAYER 1: Polynomial (`processing.h/.cpp`) ──

| # | Hàm | Input | Output | Ghi chú |
|---|-----|-------|--------|---------|
| F07 | `double evaluate_polynomial(Polynomial p, double x)` | Polynomial, x | double P(x) | **Horner**: P = a[n]; for i=n-1..0: P = P*x + a[i] |
| F08 | `Polynomial differentiate_polynomial(Polynomial p)` | Polynomial | Polynomial P'(x) | P'(x): coeffs[i] = (i+1) * coeffs[i+1]; degree - 1 |
| F09 | `Polynomial add_polynomials(Polynomial a, Polynomial b)` | 2 Polynomial | Polynomial tổng | cộng từng hệ số bậc tương ứng (Fraction add) |
| F10 | `string polynomial_to_string(Polynomial p)` | Polynomial | string | dùng debug/log |

---

### ── LAYER 2: Array Algorithms (`processing.h/.cpp`) ──

| # | Hàm | Input | Output | Ghi chú |
|---|-----|-------|--------|---------|
| F11 | `vector<double> build_px_array(Polynomial p, vector<double> xs)` | Polynomial, x_vals | vector P(xi) | map xi → P(xi) |
| F12 | `pair<int,int> kadane_max_subarray(vector<double> arr)` | vector<double> | {l, r} chỉ số | tổng P(x) lớn nhất đoạn [l,r] |
| F13 | `vector<int> longest_increasing_subsequence(vector<double> arr)` | vector<double> | vector chỉ số | chuỗi xi liên tiếp P(xi) tăng dần (LIS tư duy) |
| F14 | `vector<double> build_prefix_sum(vector<double> arr)` | vector<double> | vector prefix | prefix[i] = P(x0)+...+P(xi) |
| F15 | `double query_range_sum(vector<double> prefix, int l, int r)` | prefix, l, r | double | tổng [l,r] O(1) |

---

### ── LAYER 3: Statistics (`processing.h/.cpp`) ──

| # | Hàm | Input | Output | Ghi chú |
|---|-----|-------|--------|---------|
| F16 | `double compute_mean(vector<double> arr)` | vector<double> | double mean | trung bình dãy P(xi) |
| F17 | `double compute_std(vector<double> arr, double mean)` | vector, mean | double std | độ lệch chuẩn |
| F18 | `vector<double> zscore_normalize(vector<double> arr)` | vector<double> | vector z-score | (xi - mean) / std |
| F19 | `string compare_polynomials(Polynomial a, Polynomial b, vector<double> xs)` | 2 poly, xs | "a > b" / "a < b" / "a == b" | so sánh theo mean P(xi) |

---

### ── LAYER 4: IO (`io.h/.cpp`) ──

| # | Hàm | Input | Output | Ghi chú |
|---|-----|-------|--------|---------|
| F20 | `Fraction parse_fraction_token(string token)` | "num/den" hoặc "num" hoặc "" | Fraction | "" hoặc missing → 0/1; "n/0" → throw |
| F21 | `Polynomial parse_polynomial_row(string csvLine)` | 1 dòng CSV | Polynomial | tách id, degree, rồi parse từng hệ số |
| F22 | `vector<Polynomial> read_polynomials_csv(string filepath)` | đường dẫn file | vector<Polynomial> | đọc toàn bộ polynomials.csv, skip header |
| F23 | `vector<double> read_x_values_csv(string filepath)` | đường dẫn file | vector<double> | đọc x_values.csv, skip header |
| F24 | `void write_poly_report_csv(string filepath, Experiment exp)` | path, Experiment | file | ghi P(x) và P'(x) tại mọi xi cho mọi poly |
| F25 | `void write_extrema_txt(string filepath, Experiment exp, vector<pair<int,int>> results)` | path, results | file | ghi đoạn x cực đại (Kadane) cho từng poly |
| F26 | `void write_compare_csv(string filepath, Experiment exp)` | path, Experiment | file | ghi so sánh mean các đa thức |
| F27 | `void print_experiment_summary(Experiment exp)` | Experiment | stdout | [IO] log tóm tắt dữ liệu đã load |

---

### ── LAYER 5: Pipeline (`main.cpp`) ──

```
[1] Read polynomials.csv   → vector<Polynomial>      (F22)
[2] Read x_values.csv      → vector<double>           (F23)
[3] Build Experiment       → Experiment               (assemble struct)
[4] Print summary          → stdout log               (F27)
[5] For each Polynomial:
      build_px_array()     → vector<double> px        (F11)
      build_prefix_sum()   → vector<double> prefix    (F14)
      kadane()             → {l, r}                   (F12)
      lis()                → vector<int> indices      (F13)
      compute_mean/std()   → stats                    (F16, F17)
      zscore_normalize()   → normalized               (F18)
      differentiate()      → P'(x)                   (F08)
[6] write_poly_report_csv()                            (F24)
[7] write_extrema_txt()                                (F25)
[8] write_compare_csv()                                (F26)
```

---

## ⚠️ Edge Cases phát hiện từ CSV

| Dòng CSV | Vấn đề | Xử lý |
|----------|--------|-------|
| `P03`: `3/0` | Mẫu = 0 | → throw exception, skip polynomial, log warning |
| `P08`: `-2/0` | Mẫu = 0 âm | → throw exception |
| `P14`: `-1/0` | Mẫu = 0 | → throw exception |
| `P19`: `1/0` | Mẫu = 0 | → throw exception |
| `P02, P07, P12` | Đa thức bậc 0 | → hợp lệ, P(x) = constant |
| `P12`: `0,0,0...` | Đa thức zero | → hợp lệ |
| Missing coeffs | Ô trống / "0" | → `Fraction{0, 1}` |
| `P16`: hệ số 0 đứng đầu | P(x) = -7x | → hợp lệ |

---

## 📋 Thứ tự Implement

```
Bước 1: struct Fraction + F01–F06         (processing.cpp)
Bước 2: struct Polynomial + F07–F10       (processing.cpp)
Bước 3: F11–F15 Array Algorithms          (processing.cpp)
Bước 4: F16–F19 Statistics                (processing.cpp)
Bước 5: F20–F27 IO Layer                  (io.cpp)
Bước 6: main.cpp Pipeline                 (main.cpp)
Bước 7: Test ≥3 bộ                        (README + test files)
```

---

## 🔗 Dependency Graph

```
main.cpp
  ├── io.h (F20-F27)
  │     └── processing.h (F01-F06: parse_fraction)
  └── processing.h (F07-F19)
        └── Fraction (F01-F06)
```

# RULE: C++ BTL — DA THUC PHAN SO & QUY DAO (VIBE CODING EDITION)

File nay dinh nghia quy trinh tu duy, tieu chuan code va kien truc he thong danh rieng cho Bai tap lon (BTL) Ky thuat lap trinh.
Project: Thu vien da thuc he so phan so (Fractional Polynomials) & Mo phong quy dao.
Chu so huu: Hoang Gia Bao (MSSV: 24280049)

---

## NGUYEN TAC COT LOI (BTL MANDATORY)

1. Kien truc Module (Tach file):
   - io.h / io.cpp: Dam nhan viec doc/ghi CSV va hien thi console.
   - processing.h / processing.cpp: Chua logic xu ly da thuc, phan so va cac thuat toan day con.
   - main.cpp: Dieu phoi luong xu ly (Pipeline).

2. Cau truc du lieu long nhau (3 cap):
   - struct Fraction { int num, den; } (Toi gian phan so, mau duong).
   - struct Polynomial { string id; int degree; vector<Fraction> coeffs; }
   - struct Experiment { string name; vector<Polynomial> polys; vector<double> x_vals; }
   - Rule: Tuyet doi khong truy cap coeffs truc tiep tu main. Phai qua ham xu ly cua cap Polynomial.

3. Thuat toan bat buoc (Day con/Mang con):
   - Trieu khai it nhat 2 trong: Kadane, LIS (tu duy), Sliding Window, Prefix Sum.
   - Cac ham nay nhan vector<double> (gia tri P(xi)) lam tham so doc lap.

---

## QUY TRINH TIEP CAN (PHASE-BASED)

### PHASE 0 — CLARIFY & CONTRACTS
- Input: Parse chuoi dinh dang num/den tu CSV. Xu ly he so trong thanh 0/1.
- Output: Bao cao CSV, file txt cuc tri, va so sanh da thuc.
- Validation: Check mau so = 0 ngay khi khoi tao Fraction.

### PHASE 1 — CORE LOGIC IMPLEMENTATION
- Fraction Ops: simplify, add, multiply, compare.
- Polynomial Ops:
  - Tinh P(x) bang So do Horner: P(x) = (...((an*x + an-1)x + an-2)x + ... + a0).
  - Tinh dao ham P'(x) tra ve Polynomial.
  - Cong 2 da thuc he so phan so.

### PHASE 2 — ARRAY ALGORITHMS
- Chuyen doi list x trong Experiment thanh mang P(xi).
- Ap dung Prefix Sum de truy van tong P(x) trong doan [l, r] voi O(1).
- Ap dung Kadane de tim doan x lien tiep co tong gia tri da thuc lon nhat.

### PHASE 3 — TESTING & EDGE CASES
- Case bien: Da thuc bac 0, da thuc chi co he so am, x = 0.
- Loi du lieu: File CSV sai dinh dang, mau so bang 0.

---

## TIEU CHUAN KY THUAT (ENGINEERING STANDARDS)

- Clean Code: Khong long sau qua 3 cap (Deep nesting). Dat ten ham: <dong_tu>_<danh_tu>.
- DRY: Logic cong phan so khong duoc viet lap lai trong logic cong da thuc.
- Logging: In ra console stage hien tai (e.g., [IO] Reading polynomials.csv...).
- Memory: Su dung vector thay vi mang tinh de quan ly he so.

---

## CACH VIBE VOI CLAUDE

| Lenh | Hanh dong cua Claude |
|---|---|
| !clarify | Hoi 5 cau hoi ve schema file CSV va moi truong build (MacOS/Xcode/G++). |
| !decompose | Phan ra BTL thanh cac ham can viet trong io.cpp va processing.cpp. |
| !math | Giai thich thuat toan Horner hoac Kadane bang LaTeX truoc khi code. |
| !implement | Viet code theo thu tu: Struct -> Core Math -> Algorithms -> IO. |
| !test | Tao bo du lieu test (CSV content) cho happy path va edge cases. |

---

## ANTI-PATTERNS (KHONG DUOC LAM)

- Khong truy cap truc tiep coeffs cua Polynomial tu main().
- Khong dung pow(x, n) de tinh da thuc (Thay bang Horner).
- Khong gop chung code IO va code logic vao mot file.
- Khong handle truong hop phan so chua rut gon.

---
Vibe coding workflow for BTL Ky thuat lap trinh — MSSV: 24280049

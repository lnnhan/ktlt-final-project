

**YÊU CẦU CHUNG CHO TẤT CẢ ĐỀ TÀI** 

1. **Phương pháp thiết kế (bắt buộc)**
- Phân tích đề → xác định dữ liệu/thao tác → phác main() pipeline → tách hàm → kiểm thử 
- Nộp kèm tài liệu thiết kế: sơ đồ hàm, mô tả input/output từng hàm (1–2 trang) 
2. **Struct lồng nhau (bắt buộc mọi đề)**
- Mỗi đề có ít nhất 2 cấp struct lồng nhau (struct chứa vector<struct khác>) 
- Hàm thao tác phải nhận/trả struct cấp ngoài; không truy cập trực tiếp field cấp trong từ main() 
3. **Dãy con / Mảng con (bắt buộc mọi đề)**
- Mỗi đề phải áp dụng ít nhất 2 trong số: Kadane, LIS (tư duy), Sliding window, Prefix sum 
- Thao tác trên mảng/dãy phải là hàm độc lập, nhận vector<T> làm tham số 
4. **Clean code & Tổ chức module**
- Đặt tên rõ, mỗi hàm một nhiệm vụ, DRY, không lồng sâu quá 3 cấp 
- Tách tối thiểu 3 file: io.h/.cpp – processing.h/.cpp – main.cpp 
5. **Kiểm thử**
- ≥3 bộ test từ file: test thường + case biên + case dữ liệu lớn 
- Mỗi bộ test có expected output, nộp kèm file test và README build/run 

 *[Vật lý & Mô phỏng số]* 

**Thư viện đa thức hệ số phân số và mô phỏng quỹ đạo**

**Mô tả bài toán** 

Biểu diễn đa thức với hệ số là phân số chính xác (Fraction). Chương trình đọc danh sách đa thức từ CSV, tính giá trị tại nhiều điểm, tính đạo hàm, tìm đoạn x có P(x) cực đại và so sánh các đa thức. 

**Thiết kế Struct lồng nhau** 

struct Fraction    { int num, den; }  // hệ số chính xác

struct Polynomial  { string id; int degree; vector<Fraction> coeffs; } struct Experiment  { string name; vector<Polynomial> polys; vector<double> x\_vals; }

- Experiment → Polynomial → vector<Fraction>: struct lồng nhau cấp 3

**Input / Output** 

- Input:  polynomials.csv (id, degree, num0/den0, num1/den1, ...); x\_values.csv 
- Output: poly\_report.csv (P(x), P'(x)); extrema.txt (đoạn x cực trị); compare.csv 

**Yêu cầu kỹ thuật** 



|**Kỹ thuật**|**Yêu cầu cụ thể**|
| - | - |
|Struct lồng nhau|Polynomial chứa vector<Fraction>; Experiment chứa vector<Polynomial>; hàm thao tác tách rõ|
|Phân số (Fraction)|Struct Fraction: chuẩn hoá gcd, mẫu dương; cộng/nhân/so sánh phân số|
|Đa thức (Polynomial)|Tính P(x) bằng Horner; tính P'(x); cộng 2 đa thức (hệ số Fraction)|
|File/CSV|Đọc hệ số dạng 'num/den' hoặc số nguyên; xử lý missing hệ số → 0/1|
|Dãy con – Kadane|Trên mảng P(xi): tìm đoạn x liên tiếp có tổng P(x) lớn nhất|
|Dãy con – tăng dài nhất|Tìm chuỗi xi liên tiếp mà P(xi) tăng dần (LIS tư duy)|
|Prefix sum 1D|Prefix sum trên mảng P(xi): truy vấn tổng đoạn [l,r] O(1)|
|Thống kê|mean/std của dãy P(xi); chuẩn hoá Z-score; so sánh 2 đa thức theo mean|
|M0 + Test|≥3 bộ test; case biên: bậc 0, hệ số âm, mẫu = 0 (lỗi), x = 0|


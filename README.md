# KTLT Final Project - Phân Tích & Xử Lý Đa Thức (Polynomial Processing)

Dự án này là bài tập lớn môn học Kỹ thuật Lập trình, sử dụng ngôn ngữ C++ để xử lý, tính toán và phân tích các dữ liệu đa thức. Mã nguồn được thiết kế theo hướng module hóa, giúp tách biệt rõ ràng giữa các tác vụ xử lý Đọc/Ghi (I/O) và Logic tính toán (Processing).

## Tính Năng Chính
- **Đọc Dữ Liệu:** Trích xuất các hệ số đa thức và giá trị $x$ đầu vào từ các file CSV (`polynomials-*.csv`, `x_values-*.csv`).
- **Xử Lý & Tính Toán:** Đánh giá và tính toán các giá trị của đa thức tại các điểm $x$ cụ thể.
- **Phân Tích Toán Học:** Xác định điểm cực trị của các hàm số (xuất ra file `.txt`) và đối chiếu, so sánh kết quả.
- **Báo Cáo Kết Quả:** Tự động tổng hợp và ghi kết quả tính toán ra các tệp tin báo cáo chuẩn định dạng CSV để dễ dàng thống kê.

## Cấu Trúc Mã Nguồn
Dự án được phân chia thành các module chức năng chuyên biệt để dễ bảo trì:
- `main.cpp`: Tệp thực thi chính, đóng vai trò điều phối luồng hoạt động của chương trình.
- `io.h` & `io.cpp`: Quản lý các thao tác tương tác dữ liệu, chuyên trách việc đọc đầu vào và ghi file đầu ra.
- `processing.h` & `processing.cpp`: Chứa các thuật toán cốt lõi để tính toán giá trị và tìm cực trị của đa thức.
- **Dữ liệu đầu vào (Inputs):** `polynomials-3.csv`, `x_values-3.csv`
- **Kết quả đầu ra (Outputs):** - `poly_report.csv`: Báo cáo tổng hợp các đa thức.
  - `compare.csv`: Bảng dữ liệu so sánh.
  - `extrema.txt`: Tệp lưu trữ kết quả các điểm cực trị.

## Hướng Dẫn Cài Đặt & Chạy Chương Trình

**Yêu cầu:** Hệ thống cần cài đặt trình biên dịch C++ (như GCC, Clang) hỗ trợ chuẩn C++11 trở lên.

1. **Clone repository về máy:**
   ```bash
   git clone [https://github.com/lnnhan/ktlt-final-project.git](https://github.com/lnnhan/ktlt-final-project.git)
   cd ktlt-final-project
2. **Biên dịch chương trình:**
Sử dụng lệnh sau để liên kết các file nguồn lại với nhau:
   ``` bash
   g++ main.cpp io.cpp processing.cpp -o btl
3. **Thực thi chương trình:**
    ```bash
   ./btl
    
   (Sau khi chương trình chạy xong, các tệp báo cáo như poly_report.csv, compare.csv, và extrema.txt sẽ tự động được tạo hoặc cập nhật trực tiếp tại thư mục này).

## Thông tin tác giả 
Sinh viên thực hiện:
- Lưu Nguyễn Ngọc Hân - MSSV: 23280055
- Hoàng Gia Bảo - MSSV: 24280049

Trường: Đại học Khoa học Tự nhiên, ĐHQG-HCM (HCMUS)

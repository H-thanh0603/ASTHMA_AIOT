# ASTHMA_AIOT

Dự án IoT kết hợp AI nhằm hỗ trợ theo dõi tình trạng hô hấp và các yếu tố môi trường có thể ảnh hưởng đến người mắc hen suyễn.

## 1. Giới thiệu

ASTHMA_AIOT sử dụng **ESP32-S3** làm bộ điều khiển trung tâm để thu thập dữ liệu từ các cảm biến.

Dữ liệu thu được có thể được sử dụng để:

- Theo dõi tình trạng sức khỏe người dùng.
- Phát hiện âm thanh ho và khò khè.
- Theo dõi nhịp tim và SpO₂.
- Theo dõi nhiệt độ và độ ẩm môi trường.
- Theo dõi chất lượng không khí.
- Sử dụng AI để hỗ trợ nhận biết tình trạng hô hấp.

Mục tiêu của dự án là xây dựng một thiết bị nhỏ gọn có khả năng **thu thập dữ liệu và xử lý AI trực tiếp trên ESP32-S3 (TinyML/Edge AI)**.

---

## 2. Mục tiêu dự án

Dự án hướng đến việc xây dựng hệ thống có thể:

1. Thu thập dữ liệu từ các cảm biến.
2. Xử lý dữ liệu trên ESP32-S3.
3. Nhận diện âm thanh như:
   - Ho bình thường.
   - Ho.
   - Khò khè.
4. Theo dõi một số chỉ số sức khỏe.
5. Theo dõi các yếu tố môi trường.
6. Đưa ra cảnh báo khi phát hiện điều kiện bất thường.

---

## 3. Phần cứng

Các linh kiện chính của dự án:

| Linh kiện | Chức năng |
|---|---|
| **ESP32-S3** | Bộ điều khiển chính, đọc cảm biến, Wi-Fi và chạy AI |
| **INMP441** | Micro thu âm thanh để phân tích ho/khò khè |
| **MAX30102** | Đo nhịp tim và SpO₂ |
| **OLED 0.96"** | Hiển thị thông tin trực tiếp |
| **BME280** | Đo nhiệt độ, độ ẩm và áp suất |
| **Cảm biến PM2.5** | Theo dõi chất lượng không khí |

---

## 4. Công nghệ sử dụng

- **ESP32-S3**
- **Arduino / PlatformIO**
- **C/C++**
- **TinyML / Edge AI**
- **Wi-Fi**
- **I2C**
- **I2S**
- **Machine Learning**

Dữ liệu âm thanh từ INMP441 có thể được xử lý bằng các đặc trưng như **MFCC** hoặc **Mel-Spectrogram** trước khi đưa vào mô hình AI.

---

## 5. Cấu trúc project

```text
ASTHMA_AIOT/
│
├── firmware/
│   └── tests/
│       ├── test_max30102/
│       ├── test_oled/
│       └── test_inmp441/
│
├── AI_Training_Model/
│   └── ...
│
├── README.md
│
└── ...

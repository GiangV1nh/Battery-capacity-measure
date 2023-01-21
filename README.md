![alt text](https://github.com/GiangV1nh/Battery-capacity-measure/blob/main/h%C3%ACnh/thiet_bi.jpg)
# 1. Giới thiệu
Mạch này đo dung lượng một cell Li-Ion hoặc nhiều cell Li-Ion mắc song song hoặc các loại pin khác có điện áp tối đa dưới 5.5V. Đơn vị dung lượng đo được là mAh.
# 2. Thông số kỹ thuật
- Điện áp nguồn cấp: 9V đến 15V.
- Điện áp pin đo được: 0V đến 5.5V.
- Dòng xả tối đa: 2A.
- Mức điều chỉnh dòng xả tối thiểu: 0.02A.
- EODV: 0V đến 4.2V.
- Mức điều chỉnh EODV tối thiểu: 0.02V.
- Dung lượng tối đa đo đươc: 99,999mAh.
# 3. Hướng dẫn sử dụng
Giao diện hiện thị trên LCD như sau:
|Hình 1 |
| :---: |
|![alt text](https://github.com/GiangV1nh/Battery-capacity-measure/blob/main/h%C3%ACnh/giao_dien_LCD.png) |

Dòng xả và EODV là hai thông số điều chỉnh được. Bên cạnh có dấu *, biểu thị hiện đang điều chỉnh thông số nào. Dấu * ở vị trí nào thì khi vặn encoder sẽ điều chỉnh thông số tại vị trí đó. Để thay đổi vị trí dấu * thì nhấn nhả encoder. Điện áp pin là điện
áp đo được từ pin đang gắn vào mạch. Dung lượng pin tăng dần trong quá trình xác định. Sau khi đặt các thông số dòng xả và EODV, nhấn và giữ encoder trong hai giây
thì mạch bắt đầu xả pin, dấu * biến mất và LED báo hiệu trên mạch sáng. Trong quá trình xả, nhấn nhả encoder thì ngưng xả.

Chuẩn bị cell pin cần đo dung lượng, nạp đầy cell này và lắp vào mạch theo đúng cực tính. Không được lắp ngược lại vì làm cho diode trong MOSFET dẫn, có dòng xả lớn qua mạch gây hỏng. Tiến hành đặt dòng xả và áp EODV, sau đó nhấn và giữ encoder trong hai giây, dấu * biến mất và LED D1 sáng, báo hiệu đang xả pin. Trong quá trình xả, dung lượng pin hiển thị tăng dần. Khi điện áp pin xuống đến ngưỡng EODV, mạch ngưng xả, dung lượng pin được hiển thị trên LCD. Khi đo pin khác hoặc đo lại, cần phải reset mạch cho dung lượng hiển thị trên LCD bằng 0mAh.
Nếu không có pin (điện áp đo được 0V) hoặc điện áp pin thấp hơn hoặc bằng EODV hoặc dòng xả là 0A thì mạch không xả. Trong quá trình xả pin, nếu ngắt pin ra thì mạch tự ngừng xả và ngừng xác định dung lượng.

# Lưu ý
- Không gắn pin ngược cực.
- Không xả pin dưới mức EODV của loại pin đang cần xác định vì như thế sẽ làm hỏng pin.
# 4. Cách chỉnh mạch cho lần đầu dùng
Chuẩn bị nguồn có khả năng cấp dòng lớn hơn 2A. Nếu điện áp nguồn nhỏ hơn 5.5V thì cắm J3 như hình sau:
|Hình 2 |
| :---: |
|![alt text](https://github.com/GiangV1nh/Battery-capacity-measure/blob/main/h%C3%ACnh/J3_1.png) |

Nếu lớn hơn 5.5V thì để J3 như hình sau:
|Hình 3 |
| :---: |
|![alt text](https://github.com/GiangV1nh/Battery-capacity-measure/blob/main/h%C3%ACnh/J3_2.png) |

Nối nguồn vào mạch đúng cực tính, nối tiếp một ampe kế, đặt dòng xả 2A và cho phép xả. Chỉnh VR1 sao cho dòng trên ampe kế là 2A. Vậy là mạch đã chỉnh xong. Đặt J3 trở lại như hình 2.

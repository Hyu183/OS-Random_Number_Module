# OS-Random_Number_Module
## Thành viên nhóm
- Nguyễn Đại Dương - 18120168
- Trần Quang Huy - 18120183
## Danh sách tính năng
- [x] Linux Kernel Module tạo ra số ngẫu nhiên

- [x] Module tạo một character device để cho phép các tiến trình ở userspace có thể open và read các số ngẫu nhiên
## Môi trường
- Module được lập trình và test trên Kernel phiên bản 4.15.0 (Ubuntu 16.04 LTS)
## Cài đặt và chạy thử
### Cài đặt
Mở terminal và di chuyển đến thư mục `src`. Tiến hành chạy lệnh sau để biên dịch module và chương trình test.
```
$ make
```
Sau khi đã biên dịch xong, bắt đầu lắp module vào kernel
```
$ sudo insmod rnum_module.ko
```
Có thể kiểm tra module đã được nạp thành công chưa bằng lệnh sau
```
$ lsmod | grep rnum_module
```
### Chạy thử
Chạy chương trình test bằng lệnh
```
$ sudo ./user_test
```
### Tháo module
Chạy lệnh sau đây để tháo module ra khỏi Kernel
```
$ sudo rmmod rnum_module
```
Dọn dẹp
```
$ make clean
```

## Link Github
Github Project của nhóm tại [đây](https://github.com/l4zyc0d3r/OS-Random_Number_Module)
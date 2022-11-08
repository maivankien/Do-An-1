#include <Wire.h>                 //Thư viện giao tiếp I2C
#include <LiquidCrystal_I2C.h>    //Thư viện LCD
LiquidCrystal_I2C lcd(0x27,16,2); //Thiết lập địa chỉ và loại LCD
byte icon[] = {                   //Tạo ký tự riêng icon
  B01110,B10001,B01110,B00100,B11111,B00100,B01010,B01010
};
void setup(){
  Wire.begin(D4,D3);               //Thiết lập chân kết nối I2C (SDA,SCL);
  lcd.init();                      //Khởi tạo LCD
  lcd.clear();                     //Xóa màn hình
  lcd.backlight();                 //Bật đèn nền
  lcd.createChar(0,icon);          //Lưu ký tự icon vào byte nhớ thứ 0
  lcd.setCursor(0,0);              //Đặt vị trí muốn hiển thị ô thứ 1 trên dòng 1
  lcd.write(0);                    //Ghi byte 0 ra vị trí ô thứ 1 trên dòng 1
  lcd.setCursor(2,0);              //Đặt vị trí ở ô thứ 3 trên dòng 1
  lcd.print("Welcom to");          //Ghi đoạn text "Welcom to"
  lcd.setCursor(0,1);              //Đặt vị trí ở ô thứ 1 trên dòng 2
  lcd.print("E-Smart Channel!");   //Ghi đoạn text "E-smart Channel!" 
}
void loop(){
  lcd.display();
  delay(1000);
  lcd.noDisplay();
  delay(500);
}

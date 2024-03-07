#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Ticker.h>
Ticker ticker;
#include <Wire.h>
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
#include <font_LCD_1602_RUS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); 
LCD_1602_RUS lcd(0x27,16,2);
bool BACKLIGHT = false; 

void newdisplay(String msg){
  if (BACKLIGHT == false){ // Якщо до цього моменту дисплей був вимкнений, він вмикається 
    lcd.backlight();
    BACKLIGHT = true;
    delay(100);
  }

  int textlen = msg.length();// Отримання довжини тексту (n)

  lcd.print(msg); // Вивід тексту на дисплей
  lcd.setCursor(0,0);

  for (int positionCounter = 0; positionCounter < textlen; positionCounter++) { // Принцип роботи цього блоку складається з зміщення тексту вліво n - повторів, де n - кількість символів у тексті.
    lcd.scrollDisplayLeft();
    delay(500); 

    if(positionCounter == textlen - 1){ // Коли увесь текст "заховався" вліво, дисплей очищається, даючи дозвіл на його вимкнення
      lcd.clear();
      BACKLIGHT = false;
    }
  }
    
   
}

void staticdisplay(String msg){

  if (BACKLIGHT == false){
    lcd.backlight();
    BACKLIGHT = true;
    delay(100);
  }

  int textlen = msg.length();

  String firsthalf;
  String secondhalf;

  if (textlen > 16){
    firsthalf =  msg.substring(0, 16);
    secondhalf = msg.substring(16);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(firsthalf);

    lcd.setCursor(0,1);
    lcd.print(secondhalf);
    
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(msg);
  }

    delay(3750); //3500

    BACKLIGHT = false;
}


void AutoLightoff() {
  if (BACKLIGHT == false){
    lcd.noBacklight();
    lcd.clear();
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("===> 192.168.4.1 <==="); // Створення Wi-Fi мережі із заданим ім'ям

  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Wi-Fi Texter");
  delay(2333);
  lcd.clear();
  lcd.noBacklight();
  lcd.setCursor(0, 0);

  // WIFI ACCESS POINT
  

  
 

  // WEB - PAGE
const char *htmlpage =
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <meta charset='UTF-8'>"
"    <title>Вивід тексту на дисплей</title>"
"    <style>"
"        html, body{"
"            background-color: rgb(59, 59, 59);"
"            height: 100%;"
"        }"
"        .main{"
"            position: absolute;"
"            top: 25%;"
"            left: 50%;"
"            height: 12%;"
"           transform: translate(-50%, -50%);"
"        }"
"        input[type=\"text\"]{"
"            border-radius: 9px;"
""
"            width: 75vw;"
"            height: 5vh;"
""
"            font-size: larger;"
"        }"
"        .btinfo{"
"            border-radius: 9px;"
" "
"            width: 15vw;"
"            height: 5vh;"
""
"            position: absolute;"
"            top: 95%;"
"            left: 85%;"
"            transform: translate(-50%, 50%);"
"        }"
"        .bt1{"
"            border-radius: 9px;"
" "
"            width: 25vw;"
"            height: 5vh;"
""
"            position: absolute;"
"            top: 95%;"
"            left: 20%;"
"            transform: translate(-50%, 50%);"
"        }"
"        .bt2{"
"            border-radius: 9px;"
" "
"            width: 25vw;"
"            height: 5vh;"
""
"            position: absolute;"
"            top: 95%;"
"            left: 55%;"
"            transform: translate(-50%, 50%);"
"        }"
"    </style>"
"</head>"
"<body>"
""
"    <div class=\"main\">"
"        <h2></h2>"
"            <input type='text' name='usertext' placeholder=\"Your text here...\">"
"            <br><br>"
"            <input type='button' class='bt1'  value='Відправити ⏪' onclick=\"SendText()\">"
"            "
"            <input type='button' class='bt2' value='Відправити 📤' onclick=\"SendTextStatic()\">"
"            "
"            <input type='button' class='btinfo'  value='❓' onclick=\"ShowInfo()\">"
"    </div>"
""
"    <script>"
"        const ShowInfo = () => alert('❕ Кириличні символи іноді можуть відображатися некоректно\\n \\n⏪ - Відображення плаваючого тексту (Макс. 40 символів) \\n \\n📤 - Відображення статичного тексту (Макс. 32 символів)');"
"        let matfilter = (msg) => { "
"            return msg;"
"        };"
""
""             
"           const changelet = (msg) => {"
"            const letters = {'і': 'i', 'І': 'I', 'с': 'c', 'С': 'C', 'а': 'a', 'А': 'A', 'е': 'e', 'Е': 'E', 'о': 'o', 'О': 'O', 'р': 'p', 'Р': 'P', 'х': 'x', 'Х': 'X', 'Т': 'T', 'В': 'B', 'М': 'M', 'К': 'K'};"
""
"                for (let key in letters){"
"                    var pattern = new RegExp(key, 'g');"
"                    msg = msg.replace(pattern, letters[key]);"
"                }"
""
"                return msg;"
"            };"
""
""
"        const SendText = () => {"
""
"            const text = document.getElementsByName('usertext')[0].value;"
"             if (text.length == 0) {"
"                         alert('⚠️ Це поле не може бути порожнім. ⚠️');"
"                        return"
"              };"
"             if (text.length > 40) {"
"                         alert('⚠️ Повідомлення більше 40 символів ⚠️');"
"                        return"
"              };"
""
"            const words = text.split(' ');"
""
"            const filteredtext = [];"
"            words.forEach(word => {"
""
"                filteredtext.push(matfilter(word));"
"            });"
""
"            let resultText = filteredtext.join(' ');"
"            resultText = changelet(resultText);"
""
"            fetch('/SendText', {"
"                method: 'POST',"
"                headers: {"
"                    'Content-Type': 'application/json;charset=utf-8'"
"                },"
"                body: JSON.stringify(resultText)"
"            });"
""
"            document.getElementsByName('usertext')[0].value = '';"
"            alert('📨 Відправлено:\\n'+resultText);"
"            console.log(`${resultText} [${resultText.length}]`);"
"        };"
""
"        const SendTextStatic = () => {"
""
"            const text = document.getElementsByName('usertext')[0].value;"
"             if (text.length == 0) {"
"                         alert('⚠️ Це поле не може бути порожнім. ⚠️');"
"                        return"
"              };"
"             if (text.length > 32) {"
"                         alert('⚠️ Повідомлення більше 32 символів ⚠️');"
"                        return"
"              }"
""
"            const words = text.split(' ');"
""
"            const filteredtext = [];"
"            words.forEach(word => {"
""
"                filteredtext.push(matfilter(word));"
"            });"
""
"            let resultText = filteredtext.join(' ');"
"            resultText = changelet(resultText);"
""
"            fetch('/SendStaticText', {"
"                method: 'POST',"
"                headers: {"
"                    'Content-Type': 'application/json;charset=utf-8'"
"                },"
"                body: JSON.stringify(resultText)"
"            });"
""
"            document.getElementsByName('usertext')[0].value = '';"
"            alert('📨 Відправлено:\\n'+resultText);"
"            console.log(`${resultText} [${resultText.length}]`);"
"        };"
""
"    </script>"
"</body>"
"</html>";

// WEB - SERVER
  server.on("/",  [htmlpage](){ // Отримання запиту на 192.168.4.1 (Головна сторінка)
    server.send(200, "text/html", htmlpage); // Пристрій(Сервер) відсилає сторінку
  });

  server.on("/SendText", HTTP_POST, []() { // Отримання POST запиту із текстом  (Плаваючий текст)

    if (server.hasArg("plain")){

      String strtext = server.arg("plain"); 
      if (strtext.length() >= 2) {
        strtext = strtext.substring(0, strtext.length() - 1);
      }

        newdisplay(strtext); // Відображення тексту на дисплеї
    }
  });


  server.on("/SendStaticText", HTTP_POST, []() { // Отримання POST запиту із текстом (Статичний текст)

     if (server.hasArg("plain")){
  
      String strtext = server.arg("plain"); 
      if (strtext.length() >= 2) {
        strtext = strtext.substring(1, strtext.length() - 1);
      }
      
      staticdisplay(strtext); // Відображення тексту на дисплеї
    }

  });

  server.begin(); // Запуск веб-серверу

  ticker.attach(15, AutoLightoff); // Таймер кожні 15 секунд перевіряє наявність тексту на дисплеї, в інакшому випадку дисплей вимкнется 

  
}

void loop() {
  server.handleClient();
}

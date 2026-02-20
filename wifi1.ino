#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 5    
#define LED_COUNT 8
// #define SENSOR_PIN 4

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// uint8_t r=0,g=0,b=0;
// uint8_t pattern[LED_COUNT][3];

const char* ssid = "rewrite-c";
const char* password = "welcome.rewrite";

WebServer server(80); // ポート80でWebサーバー起動

void handleLED(){
  int x = server.arg("cursorX").toInt();
  int y = server.arg("cursorY").toInt();
  Serial.print("x:");
  Serial.print(x);
  Serial.print("y:");
  Serial.println(y);

  // int line = 255 / 2;
  // for(int i=0;i<LED_COUNT;i++){
  //   if(x<line && y<line){
  //     strip.setPixelColor(i,255,0,0);
  //   }else if(x<line && y>line){
  //     strip.setPixelColor(i,0,255,0);
  //   }else if(x>line && y<line){
  //     strip.setPixelColor(i,0,0,255);
  //   }else if(x>line && y>line){
  //     strip.setPixelColor(i,255,255,0);
  //   }else{
  //     strip.setPixelColor(i,255,255,255);
  //   }
  // }
  uint8_t r = x;
  uint8_t g = y;
  uint8_t b = (r+g)/2;
  for(int i=0;i<LED_COUNT;i++){
    strip.setPixelColor(i,r,g,b);
  }
  strip.setBrightness(5);
  strip.show();
  server.send(200, "text/plain", "OK");
  
}

void handleRoot() {
  

  server.send(200, "text/html", 
  "<html>"
    "<body>"
      "<h1>Hello from Arduino!</h1>"
      "<button class='btn' id='btn1'>button1</button>"
      "<button class='btn' id='btn2'>button2</button>"
      "<button class='btn' id='btn3'>button3</button>"
      "<script>"
        "let lastTime = 0;"
        "document.addEventListener('mousemove',function(e){"
        "  const now = Date.now();"
        "  if(now - lastTime < 50)return;"
        "  lastTime = now;"

        "  const maxX = window.innerWidth;"
        "  const maxY = window.innerHeight;"
        "  const x = Math.floor(e.clientX / maxX * 255);"
        "  const y = Math.floor(e.clientY / maxY * 255);"
        "  console.log(x,y);"
        "  fetch('/', {method: 'POST',headers: {'Content-Type':'application/x-www-form-urlencoded'},body:'cursorX=' + x + '&cursorY='+ y});"



        "});"
        ""

        "document.querySelector('#btn1').addEventListener('click',function(){"
          ""
        "});"
      "</script>"
    "</body>"
  "</html>"
  );  
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting...");
  }
  Serial.println("Connected!");
  Serial.println(WiFi.localIP());

  strip.begin();
  strip.show();


  server.on("/",HTTP_GET,handleRoot); 
  server.on("/", HTTP_POST, handleLED);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // クライアントのリクエストを処理

  
}

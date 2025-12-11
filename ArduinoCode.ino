#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Pin Definitions
const int ledPin1 = D0;
const int ledPin2 = D1;
const int ledPin3 = D2;
const int ledPin4 = D3;
const int ledPin5 = D5;
const int ledPin6 = D6;
const int soilPin = A0;
const int dhtPin = D7;

#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

// LED States
bool ledState1 = false;
bool ledState2 = false;
bool ledState3 = false;
bool ledState4 = false;
bool ledState5 = false;
bool ledState6 = false;

// Wi-Fi Access Point Credentials
const char *ssid = "SMART_AGRICULTURAL_CONTROL";
const char *password = "123456789";

ESP8266WebServer server(80);

// HTML Web Interface
String htmlContent = R"rawliteral(
<!--
  SMART AGRICULTURAL SYSTEM Web Interface
  Major Sections:
    - HTML Head: Title, Styles
    - Body: Sensor Display, Controls
    - JavaScript: LED Toggle, Brightness, Sensor Data Fetch
-->

<html>
<head>
  <title>SMART AGRICULTURAL SYSTEM</title>
  <style>
    /* ====== Page Layout ====== */
    body { 
      text-align: center; font-family: Arial, sans-serif; padding: 30px; }
    /* ====== Header Styles ====== */
    h1 {color: #333; 
      font-size: 40px;
      text-transform: uppercase;
      font-weight: bold;
      background-color: #fdd835; 
      padding: 20px 30px;             /* Space inside the box */
      border-radius: 10px;            /* Slightly rounded corners */
      border: 2px solid #007BFF;
      display: inline-block;          /* Makes the background fit the text */
      margin: 0 auto; 
    }
    h2 {font-size: 32px;}
    h3 {font-size: 28px; font-weight: bold; }
    /* ====== Button Styles ====== */
    button { 
      padding: 15px 30px; 
      font-size: 22px;
      margin: 15px;
      cursor: pointer;
    }
    .on { background-color: green; color: white; }
    .off { background-color: yellow; color: black; }
    /* ====== Slider Styles ====== */
    .slider {
      width: 400px;
      height:25px;
      margin:10px;
    }
    /* ====== Sensor Display Styles ====== */
    .sensor-label {
      font-size: 28px;
      font-weight: bold;
      margin-top: 30px;
    }
    .sensor-value { 
      font-size: 36px; 
      color: #007BFF; 
    }
    .sensor-box {
      background-color: #e0f7fa; /* Light aqua background */
      padding: 0px;
      margin-top: 0px; 
      margin-right: 0px;
      margin-bottom: 0px;
      margin-left: 0px;
      border: 2px solid #007BFF;
      border-radius: 15px;
      width: 100%;
      max-width: 1000px;
    }
    /* ====== Controls Section ====== */
    .controls {
      margin-top: 50px;
      margin-bottom: 50px;
      padding: 30px;
      /* border: 2px solid #ccc; */
      border: 2px solid #007BFF;
      border-radius: 15px;
      background-color:#FFFACD ;
    }
    span[id^="val"] {font-size: 24px; font-weight: bold; }
  </style>
</head>

<body>
  <!-- ====== Sensor Display Section ====== -->
  <div class="sensor-box">
    <h1>SMART AGRICULTURAL SYSTEM</h1>
    <div class="sensor-label">Temperature: <span class="sensor-value" id="temp">--</span> &deg;C</div>
    <div class="sensor-label">Humidity: <span class="sensor-value" id="hum">--</span> %</div>
    <div class="sensor-label">
      Soil Moisture: <span class="sensor-value" id="soil">--</span> 
    </div>
    <!-- ====== Controls Section ====== -->
    <div class="controls">
      <h2> CONTROLS</h2>
      <!-- Light Control -->
      <h3>Light</h3>
      <button id="btn1" class="on" onclick="toggleLED(1)">Turn On</button><br>
      <input type="range" min="0" max="1023" value="0" class="slider" id="slider1" onchange="updateBrightness(1, this.value)">
      <br><span id="val1">Brightness: 0</span><br><br>
      <!-- Water Control -->
      <h3>Water</h3>
      <button id="btn2" class="on" onclick="toggleLED(2)">Turn On</button><br>
      <input type="range" min="0" max="1023" value="0" class="slider" id="slider2" onchange="updateBrightness(2, this.value)">
      <br><span id="val2">Brightness: 0</span><br><br>
      <!-- Fertilizer Control -->
      <h3>Fertilizer</h3>
      <button id="btn3" class="on" onclick="toggleLED(3)">Turn On</button><br>
      <input type="range" min="0" max="1023" value="0" class="slider" id="slider3" onchange="updateBrightness(3, this.value)">
      <br><span id="val3">Brightness: 0</span><br><br>
      <!-- Pesticides Control -->
      <h3>Pesticides</h3>
      <button id="btn4" class="on" onclick="toggleLED(4)">Turn On</button><br>
      <input type="range" min="0" max="1023" value="0" class="slider" id="slider4" onchange="updateBrightness(4, this.value)">
      <br><span id="val4">Brightness: 0</span><br><br>
      <!-- Remove Excess Water Control -->
      <h3>Remove Excess Water</h3>
      <button id="btn5" class="on" onclick="toggleLED(5)">Turn On</button><br>
      <input type="range" min="0" max="1023" value="0" class="slider" id="slider5" onchange="updateBrightness(5, this.value)">
      <br><span id="val5">Brightness: 0</span><br><br>
      <!-- Bulb 6 Control -->
      <h3>Bulb 6</h3>
      <button id="btn6" class="on" onclick="toggleLED(6)">Turn On</button><br>
      <input type="range" min="0" max="1023" value="0" class="slider" id="slider6" onchange="updateBrightness(6, this.value)">
      <br><span id="val6">Brightness: 0</span><br><br>
      <!-- ====== JavaScript Section ====== -->
      <script>
        // Toggle LED state and update button appearance
        function toggleLED(bulb) {
          fetch('/toggleLED?bulb=' + bulb);
          const btn = document.getElementById('btn' + bulb);
          if (btn.classList.contains('on')) {
            btn.classList.remove('on');
            btn.classList.add('off');
            btn.innerText = 'Turn Off';
          } else {
            btn.classList.remove('off');
            btn.classList.add('on');
            btn.innerText = 'Turn On';
          }
        }
        // Update brightness value and send to server
        function updateBrightness(bulb, value) {
          document.getElementById('val' + bulb).innerText = "Brightness: " + value;
          fetch('/setBrightness?bulb=' + bulb + '&value=' + value);
        }
        // Periodically fetch sensor data and update display
        setInterval(() => {
          fetch('/getSensorData')
            .then(response => response.json())
            .then(data => {
              document.getElementById("temp").innerText = data.temperature;
              document.getElementById("hum").innerText = data.humidity;
              document.getElementById("soil").innerText = data.soil_raw + " (" + data.soil_percent + "%)";
            });
        }, 1000);
      </script>
    </div>
  </div>
</body>
</html>
)rawliteral";

void setup()
{
  // Initialize serial communication for debugging and monitoring
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);

  // analogWriteRange(1023) is specific to ESP8266; remove or guard with #ifdef for portability.
  analogWriteRange(1023);
  dht.begin();

  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/toggleLED", handleToggleLED);
  server.on("/setBrightness", handleSetBrightness);
  server.on("/getSensorData", handleSensorData);

  server.begin();
}

void handleRoot()
{
  String page = htmlContent;

  bool ledStates[] = {ledState1, ledState2, ledState3, ledState4, ledState5, ledState6};
  for (int i = 0; i < 6; i++) {
    if (ledStates[i]) {
      page.replace("id=\"btn" + String(i + 1) + "\" class=\"on\"", "id=\"btn" + String(i + 1) + "\" class=\"off\"");
    }
  }

  server.send(200, "text/html", page);
}

void handleToggleLED()
{
  int bulb = server.arg("bulb").toInt();

  if (bulb == 1)
  {
    ledState1 = !ledState1;
    digitalWrite(ledPin1, ledState1 ? HIGH : LOW);
  }
  else if (bulb == 2)
  {
    ledState2 = !ledState2;
    digitalWrite(ledPin2, ledState2 ? HIGH : LOW);
  }
  else if (bulb == 3)
  {
    ledState3 = !ledState3;
    digitalWrite(ledPin3, ledState3 ? HIGH : LOW);
  }
  else if (bulb == 4)
  {
    ledState4 = !ledState4;
    digitalWrite(ledPin4, ledState4 ? HIGH : LOW);
  }
  else if (bulb == 5)
  {
    ledState5 = !ledState5;
    digitalWrite(ledPin5, ledState5 ? HIGH : LOW);
  }
  else if (bulb == 6)
  {
    ledState6 = !ledState6;
    digitalWrite(ledPin6, ledState6 ? HIGH : LOW);
    server.send(200, "text/plain", "LED toggled");
  }
  else
  {
    server.send(400, "text/plain", "Invalid bulb number");
  }
}

// Removed duplicate definition of handleSetBrightness

void handleSensorData()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilRaw = analogRead(soilPin);
  int soilPercent = map(soilRaw, 0, 1023, 100, 0); // 0 = wet, 100 = dry

  String json = "{";
  if (isnan(temperature))
    json += "\"temperature\":null,";
  else
    json += "\"temperature\":" + String(temperature, 1) + ",";
  if (isnan(humidity))
    json += "\"humidity\":null,";
  else
    json += "\"humidity\":" + String(humidity, 1) + ",";
  json += "\"soil_raw\":" + String(soilRaw) + ",";
  json += "\"soil_percent\":" + String(soilPercent);
  json += "}";

  server.send(200, "application/json", json);
  server.send(200, "application/json", json);
}
void handleSetBrightness()
{
  if (server.hasArg("bulb") && server.hasArg("value"))
  {
    int bulb = server.arg("bulb").toInt();
    int brightness = server.arg("value").toInt();

    // Clamp brightness to valid range
    if (brightness < 0) brightness = 0;
    if (brightness > 1023) brightness = 1023;

    if (bulb == 1)
      analogWrite(ledPin1, brightness);
    else if (bulb == 2)
      analogWrite(ledPin2, brightness);
    else if (bulb == 3)
      analogWrite(ledPin3, brightness);
    else if (bulb == 4)
      analogWrite(ledPin4, brightness);
    else if (bulb == 5)
      analogWrite(ledPin5, brightness);
    else if (bulb == 6)
      analogWrite(ledPin6, brightness);
  }

  server.send(200, "text/plain", "Brightness updated");
}

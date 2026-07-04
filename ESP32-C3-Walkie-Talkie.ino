#include <esp_now.h>
#include <WiFi.h>
#include <ESP_I2S.h> 
#include <esp_sleep.h> 
#include <Preferences.h>
#include <WebServer.h>
#include <DNSServer.h> // NEW: Required for Captive Portal

// --------------------------------------------------------
// HARDWARE PIN DEFINITIONS
// --------------------------------------------------------
#define I2S_BCLK       4  
#define I2S_LRC        5  
#define I2S_DIN_MIC    6  
#define I2S_DOUT_AMP   7  
#define PTT_BUTTON     8  

I2SClass I2S;
Preferences preferences;
WebServer server(80);
DNSServer dnsServer; // NEW: DNS Server object

const byte DNS_PORT = 53; // Standard DNS Port

// --------------------------------------------------------
// NETWORK, SECURITY & AUDIO
// --------------------------------------------------------
String secretPassword = ""; 
bool inApMode = false;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo;

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 240 

// --------------------------------------------------------
// BUTTON STATE TRACKING
// --------------------------------------------------------
unsigned long lastTapTime = 0;
int tapCount = 0;
bool lastButtonState = LOW; 

// --------------------------------------------------------
// WEB SERVER HTML (Glass UI)
// --------------------------------------------------------
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Walkie-Talkie Setup</title>
  <style>
    body {
      background-color: #0484f8;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    }
    .glass {
      background: rgba(255, 255, 255, 0.15);
      border-radius: 16px;
      box-shadow: 0 4px 30px rgba(0, 0, 0, 0.1);
      backdrop-filter: blur(10px);
      -webkit-backdrop-filter: blur(10px);
      border: 1px solid rgba(255, 255, 255, 0.3);
      padding: 40px;
      text-align: center;
      color: white;
      width: 80%;
      max-width: 400px;
    }
    h2 { margin-top: 0; }
    input[type=text] {
      padding: 12px;
      border-radius: 8px;
      border: none;
      margin-bottom: 20px;
      width: calc(100% - 24px);
      font-size: 16px;
      outline: none;
    }
    input[type=submit] {
      padding: 12px 24px;
      border-radius: 8px;
      border: none;
      background: white;
      color: #0484f8;
      font-size: 16px;
      font-weight: bold;
      cursor: pointer;
      transition: background 0.3s;
    }
    input[type=submit]:hover {
      background: #e0e0e0;
    }
  </style>
</head>
<body>
  <div class="glass">
    <h2>Network Setup</h2>
    <p>Enter your Secret Password below.</p>
    <form action="/save" method="POST">
      <input type="text" name="password" placeholder="Secret Password" required>
      <br>
      <input type="submit" value="Save & Restart">
    </form>
  </div>
</body>
</html>
)rawliteral";

// --------------------------------------------------------
// XOR ENCRYPTION
// --------------------------------------------------------
void applyPassword(uint8_t *data, int length) {
  int passwordLength = secretPassword.length();
  if (passwordLength == 0) return; 
  
  for (int i = 0; i < length; i++) {
    data[i] = data[i] ^ secretPassword[i % passwordLength];
  }
}

// --------------------------------------------------------
// ESP-NOW CALLBACKS
// --------------------------------------------------------
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (digitalRead(PTT_BUTTON) == LOW) { 
    uint8_t audioBuffer[BUFFER_SIZE];
    memcpy(audioBuffer, incomingData, len);
    applyPassword(audioBuffer, len);
    I2S.write(audioBuffer, len);
  }
}

// --------------------------------------------------------
// WEB SERVER ROUTES
// --------------------------------------------------------
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleSave() {
  if (server.hasArg("password")) {
    String newPass = server.arg("password");
    preferences.putString("password", newPass); 
    
    String successHtml = "<html><body style='background-color:#0484f8; color:white; font-family:sans-serif; text-align:center; padding-top:20vh;'><h2>Passcord Saved!</h2><p>Turning off AP and restarting Walkie-Talkie...</p></body></html>";
    server.send(200, "text/html", successHtml);
    
    delay(2000); 
    ESP.restart(); 
  }
}

// --------------------------------------------------------
// SETUP
// --------------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(PTT_BUTTON, INPUT);

  preferences.begin("wt-config", false);
  secretPassword = preferences.getString("password", "");

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_GPIO) {
    while(digitalRead(PTT_BUTTON) == HIGH) { delay(10); }
  }

  // --- AP MODE CHECK (CAPTIVE PORTAL) ---
  if (secretPassword == "" || digitalRead(PTT_BUTTON) == HIGH) {
    inApMode = true;
    Serial.println("Starting Access Point Mode...");
    
    WiFi.softAP("ESP32-C3 Walkie-Talkie Setup", "Password@123");
    
    // Start DNS Server to intercept all requests ("*") and route to our IP
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    
    server.on("/", handleRoot);
    server.on("/save", handleSave);
    
    // Catch-all route to trigger the Captive Portal popup on iOS and Android
    server.onNotFound([]() {
      server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
      server.send(302, "text/plain", "");
    });

    server.begin();
    
    Serial.println("AP IP address: 192.168.4.1");
    return; 
  }

  // --- NORMAL WALKIE-TALKIE SETUP ---
  Serial.println("Starting normal Walkie-Talkie mode...");
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(1);
  WiFi.disconnect();
  WiFi.setTxPower(WIFI_POWER_19_5dBm);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA;
  esp_now_add_peer(&peerInfo);

  I2S.setPins(I2S_BCLK, I2S_LRC, I2S_DOUT_AMP, I2S_DIN_MIC);
  if (!I2S.begin(I2S_MODE_STD, SAMPLE_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_LEFT)) {
    Serial.println("Failed to initialize I2S bus!");
    while(1);
  }
}

// --------------------------------------------------------
// MAIN LOOP
// --------------------------------------------------------
void loop() {
  if (inApMode) {
    // Process DNS requests to keep the Captive Portal alive
    dnsServer.processNextRequest(); 
    server.handleClient();
    return;
  }

  // --- NORMAL WALKIE-TALKIE LOOP ---
  bool currentButtonState = digitalRead(PTT_BUTTON);
  unsigned long currentTime = millis();

  if (lastButtonState == LOW && currentButtonState == HIGH) {
    if (currentTime - lastTapTime <= 1000) {
      tapCount++;
      if (tapCount >= 2) {
        esp_deep_sleep_enable_gpio_wakeup(1ULL << PTT_BUTTON, ESP_GPIO_WAKEUP_GPIO_HIGH);
        esp_deep_sleep_start();
      }
    } else {
      tapCount = 1;
      lastTapTime = currentTime;
    }
    delay(50); 
  }

  lastButtonState = currentButtonState;

  if (currentButtonState == HIGH) {
    uint8_t audioBuffer[BUFFER_SIZE];
    size_t bytesRead = I2S.readBytes((char*)audioBuffer, BUFFER_SIZE);

    if (bytesRead > 0) {
      applyPassword(audioBuffer, bytesRead);
      esp_now_send(broadcastAddress, audioBuffer, bytesRead);
    }
  } else {
    delay(10); 
  }
}
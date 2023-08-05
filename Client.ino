// Include necessary libraries for encryption, WiFi, and HTTP communication
#include <AESLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BearSSLHelpers.h>

// WiFi and Server Configuration
const char* ssid = "SSIDNAME";
const char* password = "PASSWORD";
const char* serverName = "http://192.168.1.1/api";

AESLib aesLib; // AES library instance for encryption

// AES encryption parameters
uint8_t key[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // 128 bit key
uint8_t iv[N_BLOCK] = {0};  // Initialization vector
char data[] = "FIRE DETECTED"; // Message to send to the server

uint8_t encryptedData[32]; // Array to store the encrypted data

// Sequence number for replay protection
uint32_t sequenceNumber = 0;

void setup() {
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate
  WiFi.begin(ssid, password);  // Start WiFi connection attempt

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print("Connecting to WiFi Network: '");
    Serial.print(ssid);
    Serial.println("'");
  }
  Serial.println(" ");
  Serial.print("Successfully Connected to '");
  Serial.print(ssid);
  Serial.println("' WiFi Network");
  Serial.println(" ");
  delay(2000);
  Serial.print("IP address of Client: ");
  Serial.println(WiFi.localIP());
  Serial.println(" ");
  aesLib.gen_iv(iv);  // Generate initialization vector for AES encryption
  delay(2000);
}

void loop() {
  String message = data;
  Serial.println(".........SENDING DATA TO SERVER.........");
  Serial.println(" ");
  Serial.print("   • Clear Data Received: ");
  Serial.println(message);
  delay(2000);

  Serial.print("   • Replay Protection ID ");
  Serial.print(sequenceNumber);
  Serial.print(" Generated");
  Serial.println(" ");
  delay(2000);

  // Add sequence number and length of the message to the actual message
  String messageWithHeader = String(sequenceNumber) + "," + String(message.length()) + "," + message;

  // Encrypt the message
  int encryptedLength = aesLib.encrypt((uint8_t*)messageWithHeader.c_str(), messageWithHeader.length(), encryptedData, key, sizeof(key), iv);
  String encryptedString = "";
  for (int i = 0; i < encryptedLength; i++) {
    encryptedString += String(encryptedData[i]);
    if(i < encryptedLength - 1) {
      encryptedString += ",";  // add comma as separator between bytes
    }
  }

  // Generate HMAC for integrity and authentication
  uint8_t hmacResult[32];
  br_hmac_key_context kc;
  br_hmac_key_init(&kc, &br_sha256_vtable, key, sizeof(key));
  br_hmac_context ctx;
  br_hmac_init(&ctx, &kc, 0);
  br_hmac_update(&ctx, encryptedString.c_str(), encryptedString.length());
  br_hmac_out(&ctx, hmacResult);

  String hmacString = "";
  for (int i = 0; i < sizeof(hmacResult); i++) {
    hmacString += String(hmacResult[i]);
    if(i < sizeof(hmacResult) - 1) {
      hmacString += ",";  // add comma as separator between bytes
    }
  }
  
  // Check for WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    Serial.print("   • Data Encrypted by AES-128");
    Serial.println(" ");
    delay(2000);

    Serial.print("   • Data Integrity and Authentication (HMCA) ID Initialized! ");
    Serial.println(" ");
    delay(2000);

    Serial.print("   • Sending Encrypted Data to Server ...");
    Serial.println(" ");

    // Send the encrypted data and HMAC to the server
    int httpResponseCode = http.POST("data=" + encryptedString + "&hmac=" + hmacString);
    delay(500);
    http.end();

    Serial.print("   • Data Sent!");
    Serial.println(" ");
    delay(2000);
  }

  sequenceNumber++; // Increment the sequence number to prevent replay attacks
  Serial.println(" ");
  Serial.println(" ");
  delay(5000);  // Wait for 5 seconds before next loop iteration
}

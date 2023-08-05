// Required libraries for the ESP8266 WiFi module, web server functionality, AES encryption, and HMAC for validation
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <AESLib.h>
#include <BearSSLHelpers.h>
//ESP-50018E Server

// WiFi credentials
const char* ssid = "SSIDNAME";
const char* password = "PASSWORD";

// Create an AES library instance
AESLib aesLib;

// Define the AES encryption key and initialization vector (IV)
uint8_t key[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // 128 bit key
uint8_t iv[N_BLOCK] = {0};  // initialization vector
uint8_t decryptedData[32];

// This variable helps in preventing replay attacks by tracking the sequence number of the received messages
int32_t lastSequenceNumber = -1; // To keep track of the last sequence number

// Create an instance of the web server on port 80
ESP8266WebServer server(80); // Server on port 80

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to WiFi using provided credentials
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print("Connecting to WiFi Network: '");
    Serial.print(ssid);
    Serial.println("'");
  }
  Serial.println(" ");
  Serial.print("Successfully Connected to '");
  Serial.print(ssid);
  Serial.print("' WiFi Network");
  Serial.println(" ");
  Serial.println(" ");
  delay(2000);

  // Display the local IP address of the server
  Serial.print("IP address of Server: ");
  Serial.println(WiFi.localIP());
  Serial.println(" ");

  // Generate an initialization vector (IV) for AES encryption
  aesLib.gen_iv(iv);
  delay(2000);
  
// Define a handler for POST requests on the "/api" endpoint
  server.on("/api", HTTP_POST, []() {
    Serial.println(".........RECEIVEING DATA FROM CLIENT.........");
    Serial.println(" ");

// Check if the request has "data" and "hmac" arguments
    if(server.hasArg("data") && server.hasArg("hmac")) {
      String encryptedData = server.arg("data");
      String hmacReceived = server.arg("hmac");

      Serial.print("   • AES-128 Encrypted Data Received");
      Serial.println(" ");
      delay(1000);

      Serial.print("   • Replay Protection ID Received");
      Serial.println(" ");
      delay(1000);

      // The logic here involves HMAC validation to ensure data integrity and authenticity
      // Generate the HMAC for the received data
      uint8_t hmacResult[32];
      br_hmac_key_context kc;
      br_hmac_key_init(&kc, &br_sha256_vtable, key, sizeof(key));
      br_hmac_context ctx;
      br_hmac_init(&ctx, &kc, 0);
      br_hmac_update(&ctx, encryptedData.c_str(), encryptedData.length());
      br_hmac_out(&ctx, hmacResult);

// Convert the calculated HMAC bytes to a comma-separated string for comparison
      String hmacString = "";
      for (int i = 0; i < sizeof(hmacResult); i++) {
        hmacString += String(hmacResult[i]);
        if(i < sizeof(hmacResult) - 1) {
          hmacString += ",";  // add comma as separator
          
        }
        
      }
      Serial.print("   • Validating HMAC");
      Serial.println(" ");
      delay(1000);
      
// If the calculated HMAC does not match the received HMAC, the validation fails
      if(hmacString != hmacReceived) {
        Serial.println("HMAC Validation Failed.");
        server.send(200, "text/plain", "POST received");
        return;
      }
      
// Convert the comma-separated encrypted data string to a byte array
      uint8_t encryptedDataArray[32];
      int i = 0;
      int start = 0;
      int end = encryptedData.indexOf(",");
      while (end != -1) {
        encryptedDataArray[i++] = atoi(encryptedData.substring(start, end).c_str());
        start = end + 1;
        end = encryptedData.indexOf(",", start);
      }
      encryptedDataArray[i] = atoi(encryptedData.substring(start).c_str());

// Decrypt the received data
      int decryptedLength = aesLib.decrypt(encryptedDataArray, i+1, decryptedData, key, sizeof(key), iv);
      String decryptedString = "";
      for (int i = 0; i < decryptedLength; i++) {
        decryptedString += char(decryptedData[i]);
      }
      
 // Parse the decrypted data to extract the sequence number and the actual message
      int separatorPosition = decryptedString.indexOf(',');
      uint32_t sequenceNumber = decryptedString.substring(0, separatorPosition).toInt();
      // Find the second comma, which separates the length and the actual message
      int secondCommaPosition = decryptedString.indexOf(',', separatorPosition+1);
      int messageLength = decryptedString.substring(separatorPosition+1, secondCommaPosition).toInt();

      Serial.print("   • Decrypting Data");
      Serial.println(" ");
      delay(1000);


      String message = decryptedString.substring(secondCommaPosition+1, secondCommaPosition+1+messageLength);

// Check for replay attacks based on the sequence number
      if((int32_t)sequenceNumber <= lastSequenceNumber) {
        Serial.println(" ");
        Serial.println("   !!! Replay Attack Detected !!!");
        Serial.println(" ");
        Serial.println(" ");
      } else {
        Serial.println(" ");
        Serial.println("   • Received Message: " + message);
        Serial.println(" ");
        Serial.println(" ");
        lastSequenceNumber = sequenceNumber;
      }
    }

// Send a response back to the client indicating the POST request has been received
    server.send(200, "text/plain", "POST received");
  });

// Start the server
  server.begin(); // Starts the Server
}

void loop() {
// Handle incoming client requests
  server.handleClient();
}

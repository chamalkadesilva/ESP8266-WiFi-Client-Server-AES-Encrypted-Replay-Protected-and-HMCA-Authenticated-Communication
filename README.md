# ESP8266-WiFi-Client-Server-AES-Encrypted-Replay-Protected-and-HMCA-Authenticated-Communication
Communicate securely between two ESP8266 microcontrollers using AES encryption, HMAC validation, and sequence numbers for replay protection.

**Features**
AES-128 Encryption: Provides confidentiality for the data exchanged between the client and server.
HMAC Validation: Ensures the integrity and authenticity of the received data.
Replay Attack Prevention: Uses sequence numbers to prevent attackers from resending previously captured data.
WiFi Communication: Both client and server use WiFi for communication, making the setup wireless and convenient.

**Repository Structure**
client.ino
server.ino
README.md

**Setup**
**Hardware Requirements:**
Two ESP8266 microcontrollers.
A WiFi network.
**Software Requirements:**
Arduino IDE with ESP8266 board support installed.
Required libraries: AESLib, ESP8266WiFi, ESP8266HTTPClient (for client), ESP8266WebServer (for server), and BearSSLHelpers.
**Configuration:**
Set your WiFi SSID and password in both the client and server scripts.
Make sure both devices are within the range of the specified WiFi network.

**Usage**
**Upload Code:**
Upload the client code to one ESP8266 and the server code to the other.
**Power On:**
Power on both devices. The client will automatically send encrypted messages to the server at regular intervals.
**Monitor:**
Open the serial monitors for both devices in the Arduino IDE to view the logs.

**Security Considerations**
Confidentiality of the AES key: Make sure the AES key remains secret. Implement key rotation or consider a key exchange protocol for enhanced security.
HMAC Validation: Always validate the HMAC before processing received data to ensure its integrity and authenticity.
Replay Protection: Monitor sequence numbers consistently to prevent replay attacks.

**Troubleshooting**
Connectivity Issues: Ensure both ESP8266 devices are within the WiFi range and credentials are correctly configured.
Encryption Errors: If you're experiencing encryption or decryption issues, double-check the AES key and initialization vector for consistency between the client and server.

**Contributing**
Contributions are welcome! If you have suggestions for improvement or have identified bugs, please:
Fork the repository.
Create a branch for your changes.
Submit a pull request.
For major changes, please open an issue first to discuss your proposed changes.

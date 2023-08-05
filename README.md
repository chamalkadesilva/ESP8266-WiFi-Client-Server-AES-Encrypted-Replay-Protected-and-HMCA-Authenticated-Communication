# ESP8266-WiFi-Client-Server-AES-Encrypted-Replay-Protected-and-HMCA-Authenticated-Communication
Communicate securely between two ESP8266 microcontrollers using AES encryption, HMAC validation, and sequence numbers for replay protection.

<H2>Features</H2>
<ul>
<li><b>AES-128 Encryption:</b> Provides confidentiality for the data exchanged between the client and server.</li>
<li><b>HMAC Validation:</b> Ensures the integrity and authenticity of the received data.</li>
<li><b>Replay Attack Prevention:</b> Uses sequence numbers to prevent attackers from resending previously captured data.</li>
<li><b>WiFi Communication:</b> Both client and server use WiFi for communication, making the setup wireless and convenient.</li>
</ul>

<H2>Repository Structure</H2>
<ul>
  <li>client.ino</li>
  <li>server.ino</li>
  <li>README.md</li>
  <li>libraries.zip</li>
</ul>

<H2>Setup</H2>
<b>Hardware Requirements:</b>
Two ESP8266 microcontrollers.
A WiFi network.
<br><br>
<b>Software Requirements:</b>
Arduino IDE with ESP8266 board support installed.
Required libraries: AESLib, ESP8266WiFi, ESP8266HTTPClient (for client), ESP8266WebServer (for server), and BearSSLHelpers.
<br><br>
<b>Configuration:</b>
Set your WiFi SSID and password in both the client and server scripts.
Make sure both devices are within the range of the specified WiFi network.

<H2>Usage</H2>
<b>Upload Code:</b>
Upload the client code to one ESP8266 and the server code to the other.
<br><br>
<b>Power On:</b>
Power on both devices. The client will automatically send encrypted messages to the server at regular intervals.
<br><br>
<b>Monitor:</b>
Open the serial monitors for both devices in the Arduino IDE to view the logs.

<H2>Security Considerations</H2>
<b>Confidentiality of the AES key:</b> Make sure the AES key remains secret. Implement key rotation or consider a key exchange protocol for enhanced security.
<br><br>
<b>HMAC Validation:</b> Always validate the HMAC before processing received data to ensure its integrity and authenticity.
<br><br>
<b>Replay Protection:</b> Monitor sequence numbers consistently to prevent replay attacks.

<H2>Troubleshooting</H2>
<b>Connectivity Issues:</b> Ensure both ESP8266 devices are within the WiFi range and credentials are correctly configured.
<br><br>
<b>Encryption Errors:</b> If you're experiencing encryption or decryption issues, double-check the AES key and initialization vector for consistency between the client and server.

<H2>Contributing</H2>
Contributions are welcome!<br>
If you have suggestions for improvement or have identified bugs,<br>
Please:<br>
Fork the repository.<br>
Create a branch for your changes.<br>
Submit a pull request.<br><br>
For major changes, please open an issue first to discuss your proposed changes.

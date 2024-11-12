#include <Wire.h>            // Include the Wire library for I2C communication
#include <BH1750.h>          // Include the BH1750 light sensor library
#include <WiFiNINA.h>        // Include the WiFiNINA library for Wi-Fi functionality

*/
#include <WiFiNINA.h>
// Wi-Fi credentials
char ssid[] = "Akal";           // SSID of the Wi-Fi network
char pass[] = "123456789";      // Password for the Wi-Fi network

// Webhook URL for sending an IFTTT notification when the light level exceeds the threshold
const char* webhookUrl = "/trigger/sunlight_alert/with/key/inE9LYrR33VIVO4bHklquzgYacVVtpPMaozwNRMB0Tn";

// Create instances for the light sensor and Wi-Fi client
BH1750 lightMeter;             
WiFiClient client;

const int LIGHT_SENSOR_PIN = 3; 
// Light level threshold to trigger the notification (in lux)
const float LIGHT_THRESHOLD = 1000.0; 

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.begin(9600);           // Initialize serial communication at 9600 baud
  while (!Serial);              // Wait until the serial port is ready

  // here we are initilising the input pin for the sensor
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  
  // Here we are going for connecting the 
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  Wire.begin();                 // Initialize I2C communication
  if (!lightMeter.begin()) {    // Initialize the BH1750 sensor
    Serial.println("Error initializing BH1750 sensor!");  // If the sensor fails to initialize, print an error message
    while (1);  // Infinite loop to halt the program
  }
  Serial.println("\nConnected to WiFi");

  // Test connection to the IFTTT server
  if (client.connect("maker.ifttt.com", 80)) {
    Serial.println("Connected to IFTTT server");
    client.stop();  
  } else {
    Serial.println("Connection to IFTTT server failed");
  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);        // Attempt to connect to the specified Wi-Fi network
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {  // Wait until connected to Wi-Fi
    delay(1000);                 // Wait 1 second before checking again
    Serial.print(".");           // Print a dot while waiting
  }
  Serial.println("\nConnected to WiFi");  // Once connected, print success message
}

void loop() {
  // Read the light sensor value (HIGH for light, LOW for dark)
  int lightValue = HIGH;
  Serial.print("Light Sensor State: ");
  Serial.println(lightValue == HIGH ? "Bright" : "Dark");
  // Read the light level from the BH1750 sensor
  float lux = lightMeter.readLightLevel();   
  Serial.print("Light Level: ");           
  Serial.print(lux);                  
  Serial.println(" lux");            // Print the current light level (in lux)

  // Check if the light value is high (light detected)
  if (lightValue == HIGH) {
    sendEMAIL_ID();
  // Check if the light level exceeds the threshold
  if (lux >= LIGHT_THRESHOLD) {
    sendEmailNotification();         // If the threshold is exceeded, send the notification
  } 

  delay(600); 
  delay(600);  // Wait 600 milliseconds before reading the light level again
}

void sendEMAIL_ID() {
  
// Function to send a notification when the light level exceeds the threshold
void sendEmailNotification() {
  // Attempt to connect to the IFTTT server
  if (client.connect("maker.ifttt.com", 80)) {
    Serial.println("Sending notification...");
    client.println(); 
    Serial.println("Sending notification...");  // Print a message indicating that the notification is being sent
    // Send a GET request to the IFTTT webhook URL
    client.print(String("GET ") + webhookUrl + " HTTP/1.1\r\n" +
                 "Host: maker.ifttt.com\r\n" +       // Set the host header for the IFTTT server
                 "Connection: close\r\n\r\n");       // Close the connection after the request

    // Read the server response
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
    // Wait for the response from the server
    while (client.connected()) {          // While connected to the server
      if (client.available()) {            // If there is data available to read
        char c = client.read();            // Read a byte from the server
        Serial.print(c);                   // Print the response from the server
      }
    }
    
    client.stop();
    Serial.println();
    Serial.println("Disconnected from server");
    client.stop();  // Disconnect from the IFTTT server
    Serial.println("\nNotification sent and disconnected from server");  // Print confirmation message
    delay(10000);  // Wait 10 seconds before sending another notification
  } else {
    Serial.println("Connection to IFTTT server failed");  // If the connection fails, print an error message
  }
   else
   {
      Serial.println("There is some error while connecting");
   }
}

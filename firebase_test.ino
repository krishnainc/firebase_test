#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Crypto Sonic’s iPhone"
#define WIFI_PASSWORD "crypto.sonic"

// Insert Firebase project API Key
#define API_KEY "AIzaSyABVXvGqOybYs7B25F5aHbelS4UfoBt-0E"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://groot-smart-hub-default-rtdb.firebaseio.com/" 

//relay
const int RelayPin1 = 5;
const int RelayPin2 = 4;
const int RelayPin3 = 0;


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue1;
int intValue2;
int intValue3;
bool signupOK = false;

void setup() {
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getInt(&fbdo, "/Irrigationpump/status")) {
      if (fbdo.dataType() == "int") {
        intValue1 = fbdo.intData();
        Serial.println(intValue1);            
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    
    if (Firebase.RTDB.getInt(&fbdo, "/Mixingpump/status")) {
      if (fbdo.dataType() == "int") {
        intValue2 = fbdo.intData();
        Serial.println(intValue2); 
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

    if (Firebase.RTDB.getInt(&fbdo, "/Dosingpump/status")) {
      if (fbdo.dataType() == "int") {
        intValue3 = fbdo.intData();
        Serial.println(intValue3);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

     if(intValue1 == 1)
        {
          digitalWrite(RelayPin1, LOW);
          Serial.println("On 1");          
        }
        else if(intValue1 == 0)
        {
          digitalWrite(RelayPin1,HIGH);  
          Serial.println("Off 1");         
        }     

      if(intValue2 == 1)
        {
          digitalWrite(RelayPin2, LOW);
          Serial.println("On 2");          
        }
        else if(intValue2 == 0)
        {
          digitalWrite(RelayPin2, HIGH);  
          Serial.println("Off 2");         
        } 

      if(intValue3 == 1)
        {
          digitalWrite(RelayPin3, LOW);
          Serial.println("On 3");          
        }
        else if(intValue3 == 0)
        {
          digitalWrite(RelayPin3, HIGH);  
          Serial.println("Off 3");         
        }          
                             
  }
}
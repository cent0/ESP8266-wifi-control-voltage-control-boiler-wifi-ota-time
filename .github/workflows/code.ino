
          
      /*
   I have made this project using

   ESP8266 module,PIR module and a piezo buzzer.

   Watch out it's DIY project video on my YouTube Channel

   Search for "techiesms" on YouTube.
   modded by Michele Della Guardia.
   add 0 100V analog input
   detect voltage array and add panel if not have power
   switch boiler  600W automatic
   switch boiler 1200W automatic
   switch heater automatic
   switch relay add soler panel
   add ota update
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <time.h>




//#define pir 2
                                                   
const int ANALOG_PIN = A0; // The only analog pin on the Thing
//const int DIGITAL_PIN = 12; // Digital pin to be read                                       FILO       VIOLA                  INPUT
const int ledPin =  12;        // led 1200W      D1                   //5>   10 1 12                  FILO       ARANCIONE / GIALLO     LED          
const int ledPin2 =  14;      // led stufetta                                            FILO       VERDE     / BLU        LED               
const int ledPin3 = 13;      //  led 600W                                                 FILO       ROSSO     / MARRONE
const int Relay = 5;          //                                     //2>    9 3  15  5               FILO       GIALLO                 COMANDO RELE
//const int buttonPin = 4;     // the number of the pushbutton pin D2     //                    FILO       MARRONE                input sole alto o basso
//const int buttonPin2 = 16;     // the number of the pushbutton pin

int timezone = 2 * 3600;                  // TIME
int dst = 0;                              // TIME

WiFiClient client;
// domoticz

int idx = 17; //IDX for this virtual sensor, found in Setup -> Devices
const char* host = "192.168.xx.xx";                                     //FIX YOUR DEVICE IP
const int   port = 8080;
//const int   watchdog = 60000; // Fréquence d'envoi des données à Domoticz - Frequency of sending data to Off Stufetta
const int   watchdog2 = 30000; // Fréquence d'envoi des données à Domoticz - Frequency of sending data to Domoticz
unsigned long previousMillis = millis(); 
//unsigned long previousMillis2 = millis();

int buttonState = 0;         // variable for reading the pushbutton status 
int LedPinStatus  = digitalRead(ledPin)  ;
int LedPin2Status = digitalRead(ledPin2) ;
int LedPin3Status = digitalRead(ledPin3) ;    //  status 600W

const char* ssid ="xxx";                     //YOUR WIFI LAN HOME
const char* password ="xxx";                 //YOUR WIFI PASSWORD

// Create an instance of the server
// specify the port to listen on as an argument

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";              // string domotiz

void setup() {
 
server.begin();

       Serial.begin(9600);
       // Serial.setDebugOutput(true);
       Serial.println();
       Serial.println();
       Serial.println();
       for(uint8_t t = 4; t > 0; t--) {
          Serial.printf("[SETUP] WAIT %d...\n", t);
          Serial.flush();
          delay(500);
          }
   WiFi.mode(WIFI_STA);
   WiFi.disconnect(true);
   WiFi.begin(ssid,password);
   while (WiFi.status() != WL_CONNECTED) {
   delay(5000);
   //Serial.print(".");

   // pinMode(pir,INPUT);
   // initialize the LED pin as an output:
   pinMode(ledPin, OUTPUT);
   pinMode(ledPin2, OUTPUT);
   pinMode(ledPin3, OUTPUT);
   pinMode(Relay, OUTPUT);
   digitalWrite(Relay, HIGH);                           //  setta subito alto
   
   // initialize the pushbutton pin as an input:
  // pinMode(buttonPin, INPUT);
   delay(500);
   Serial.println("Connected");
   // Start the server
   server.begin();
   Serial.println("Server started");
   // Print the IP address
   Serial.println(WiFi.localIP()); 

   configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr))
  {
  Serial.print("*");
  
  delay (1000);
  }
  }   
}



HTTPClient http;
void sendDomoticz(String url){
  Serial.print("connecting to ");
  Serial.println(host);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  http.begin(host,port,url);
  int httpCode = http.GET();
    if (httpCode) {
      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println("Domoticz response "); 
        Serial.println(payload);
        delay(1000);
      }
      }
      }








void controllo(void) {
//___________________________________________________________________________________________________________
   
  
    // wait for WiFi connection
    if((WiFi.status() == WL_CONNECTED)) {

HTTPClient http;
  //// Check if a client has connected
 //// WiFiClient client = server.available();
int sensorValue = analogRead(A0);
float voltage = sensorValue * (105 / 1023.0);
float data = voltage;
float t = data;
   //---------------------------------------------------------------------------------------------------------------------------------------------------- 

WiFiClient client;

                                        
                                          // read the input on analog pin 0:
                                          // read the state of the pushbutton value:
//                                           buttonState   = digitalRead(buttonPin);
                                           LedPinStatus  = digitalRead(ledPin);                                  
                                           LedPin2Status = digitalRead(ledPin2);
                                           LedPin3Status = digitalRead(ledPin3);
                                           //If connection successful show IP address in serial monitor

                                        // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 100V):
                                   //     float voltage = sensorValue * (109.0 / 1023.0);
                                        // print out the value you read:
                                     //   Serial.println("_____________________________");
                                        Serial.print  ("VOLTAGGIO INVERTER = ");
                                        Serial.print(voltage);
                                        Serial.println("V");
                                          Serial.print("SCALA SENSORE      = ");
                                        Serial.println(sensorValue);
                                          Serial.print("FOTOCELLULA SOLE   = ");
                                        Serial.println(buttonState);
                                          Serial.print("BOILER 1200W  = ");
                                        Serial.println(LedPinStatus);
                                          Serial.print("BOILER  600W  = ");
                                        Serial.println(LedPin3Status);
                                          Serial.print("STUFETTA      = ");
                                        Serial.println(LedPin2Status);
                                     //   Serial.println("______________________________");
  
                                        delay(1000);        // delay in between reads for stability

                                      // read the state of the pushbutton value: 
                                  //      sensorValue = analogRead(A0);

                                      
                                      // check if the pushbutton is pressed. If it is, the sensorValue is HIGH:
                                     

//float humidity = voltage;
  
        
                        unsigned long currentMillis = millis();
                   if ( currentMillis - previousMillis > watchdog2 ) {
                         previousMillis = currentMillis;
                                                                  
                     String url = "/json.htm?type=command&param=udevice&idx=17&nvalue=0&svalue=";
                     url += String(t); url += ";";
                     sendDomoticz(url);
                                      }
                                      delay(1000);
                           //            server.handleClient();    // ????????????

                                      
                                      

}
}
 void controllo7(void){                     //    ##########################    TIME   TIME #############################################################
 time_t now = time(nullptr);
 struct tm* p_tm = localtime(&now);

 Serial.print("ORE                = ");
 Serial.print(p_tm->tm_hour);
 Serial.print(":");
 Serial.print(p_tm->tm_min);
 Serial.print(":");
 Serial.println(p_tm->tm_sec);


// if (p_tm->tm_sec >= 10 && p_tm->tm_sec <= 30){ 
//  digitalWrite(Relay, LOW);
 
// }
// else 
// {digitalWrite(Relay, HIGH); }

}


void controllo8(void) {              // OTA O T A     #####################################################################      O  T   A      O   T    A


  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

 
    if (WiFi.waitForConnectResult() != WL_CONNECTED ){
    (ArduinoOTA.onStart([]() {
       Serial.println("\nStart");
  

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
  
    }));}
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  
  
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}


 void controllo9(void) {                            // #####################################  WEB  ########################################################

int sensorValue = analogRead(A0);
float voltage = sensorValue * (105 / 1023.0);
float data = voltage;
//int float Voltage = data;
float t = data;
   
   //the HTML of the web page

page = "<h1>Inverter Voltage By Uno(5)Zero</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p><h1>Voltaggio:</h1> <h1>"+String(data)+"</h1>";

   //make the LED pin output and initially turned off

   server.on("/", [](){
    server.send(200, "text/html", page);
   });
   server.on("/LEDOn", [](){
    server.send(200, "text/html", page);
   // digitalWrite(ledPin, HIGH);          ?????    
      
                                                                                                           
   });
   server.on("/LEDOff", [](){
    server.send(200, "text/html", page);
   // digitalWrite(ledPin, LOW);           ?????
                                                                                                            
    
   });
   data = analogRead(A0);
   server.begin();
   
   server.handleClient();
   delay(500);
}

   


 void controllo1(void) {             // ###############################################     BOILER   ###################################################################                        
  
int sensorValue = analogRead(A0);
float voltage = sensorValue * (105 / 1023.0);

time_t now = time(nullptr);
struct tm* p_tm = localtime(&now);
if (sensorValue > 520 and p_tm->tm_hour >=9 and p_tm->tm_hour <= 18) {
                          
                                          int sensorValue = analogRead(A0);
                                          float voltage = sensorValue * (105 / 1023.0);

                                  
                                       if  (sensorValue > 470 and sensorValue <=680)  {         // { if  (sensorValue > 470 and sensorValue <=780) { 
                                        int sensorValue = analogRead(A0);
                                        float voltage = sensorValue * (105 / 1023.0);
                                        delay(500);
                                        digitalWrite(Relay, LOW);                     //  rele BASSO ACCENDE PANNELLI
                                                                                                                 
                                          }
                                                                     else {
                                                                      int sensorValue = analogRead(A0);
                                                                      float voltage = sensorValue * (105 / 1023.0);

                                                                      if  (sensorValue >681 )      //      if  (sensorValue >800 )   650 66,71V    //  610
                                                        { 
                                                                      digitalWrite(Relay, HIGH);    //  rele ALTO SPEGNE PANNELLI
                                                                      
                                                                            } 



                                         
                                         //  if(sensorValue > 795  and buttonState == HIGH) {      //750   76,97V      // 800
                                        // if(sensorValue > 795 ) {                                //750   76,97V      // 800
                                           if(sensorValue >= 770 ) {                               //750   76,97V      // 800
                                            if(sensorValue >= 710 )                                //770   79,03V
                                                                                                   //800   82,11V
                                                                                 
                                                                  delay(1000);
                                                       //         if (LedPinStatus == LOW) { 
                                                                      // turn LED on:
                                                                         digitalWrite(ledPin3, LOW);    
                                                                         digitalWrite(ledPin, HIGH);
                                                                        
                                                                    //  Serial.println();
                                                                   //   Serial.println(" __  |\  | On On On On On On");
                                                                        Serial.println("|  | | \ | accensione Scalabagno        1200w >780 >10h ");
                                                                   //   Serial.println("|__| |  \| ON ON ON ON");
                                                                        Serial.println();
                                                                       Serial.println("cottrollo 1 ");
                                                                       Serial.println();
                                                                       Serial.println("ATTESA CHE L'inverter RISPONDI IN POTENZA");
                                                                       Serial.println("10 Secondi...");
                                                                       delay(500); 
                                                                  // requesting required link
                                                                  http.begin("http://192.168.xx.xx/api/relay/0?apikey=xxxxxxxxxxxxxxxx&value=1"); //HTTP   REPLACE IP relay AND APYKEY
                                                                  // start connection and send HTTP header
                                                                  int httpCode = http.GET();
                                                                  http.end(); 
                                                                  
                                                                  delay(3000);        // delay in between reads for stability
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);

                                                                    digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                                    digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                                   
                                                                                        
                                                          }
                                                                                                
                                                                                                       
        
                                                         // }  else if (buttonState == LOW)
                                                         // }  else if (LedPinStatus == HIGH and sensorValue < 600 )      //  530 = 54,97V     610
                                                         // }  else if (sensorValue < 590 and buttonState == LOW)         //  530 = 54,97V     610
                                                                }   if (sensorValue <= 640 && LedPinStatus==HIGH)         //  520 = 53,37V
                                                                     digitalWrite(Relay, LOW);                     //  rele BASSO ACCENDE PANNELLI
                                                                 {                                                        //  495 = 50,80V
                                                                                                                          //  490 = 50,29V
                                                               //  delay(6000);
                                                                 Serial.println("SE minore di <640 e lo scaldabagno 1200 è acceso accendi 600W");
                                                                 Serial.println();                        
                                                                 digitalWrite(ledPin, LOW);
                                                                 digitalWrite(ledPin3, HIGH);
                                                                 
                                                                 http.begin("http://192.168.xx.xx/api/relay/1?apikey=xxxxxxxxxxxxxxxx&value=1"); //HTTP    REPLACE IP relay AND APYKEY
                                                                 int httpCode = http.GET();
                                                                 http.end();

                                                                 digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);

                                                                    digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);
                                                                    digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);
                                                                                                         
                                         
                                                                               
                                                           //  if(sensorValue <= 730 ) {     //77.39   754    ??????
                                                            
                                                         //    Serial.println("cottrollo 1 ");
                                                        //      Serial.println();
                                                          // Serial.println("|  ||__ |__ Scaldabagno 600W ON <=730 >=800 tengo acceso 600 1200 NON riesce ");      // on
                                                        //    Serial.println();
                                                           // start connection and send HTTP header
                                                      //                 http.begin("http://192.168.xx.xx/api/relay/1?apikey=xxxxxxxxxxxxxxxx&value=1"); //HTTP   REPLACE IP relay AND APYKEY
                                                                       
                                                                       
                                                                       
                                                    //                       int httpCode = http.GET();
                                                  //                           http.end();
                                                                         }
                                                                      
                                                                    //       delay(3000);        // delay in between reads for stability
                                                                                         
   
                                                                           }
                                                                         }
                                                                                                                    
                                                                //        }
                                                                                                                    

 void controllo3(void) {   //####################################################RELAY  PANNELLI   CONTROLLO 3   ##########################
                                                                                                                 
                                                                                                               
                                      int sensorValue = analogRead(A0);                                                //  
                                      float voltage = sensorValue * (105 / 1023.0);

                                      if  (sensorValue > 470){
                                        int sensorValue = analogRead(A0);
                                        float voltage = sensorValue * (105 / 1023.0);
                                        Serial.println("controllo 3 ");
                                        Serial.print  ("VOLTAGGIO INVERTER = ");
                                        Serial.print(voltage);
                                        Serial.println("V");
                                          Serial.print("SCALA SENSORE      = ");
                                        Serial.println(sensorValue);
                                                                                                                  
                                        Serial.println();
                                       if  (sensorValue > 470 and sensorValue <=680)  {         // { if  (sensorValue > 470 and sensorValue <=780) { 
                                        int sensorValue = analogRead(A0);
                                        float voltage = sensorValue * (105 / 1023.0);
                                        delay(500);
                                        digitalWrite(Relay, LOW);                     //  rele BASSO ACCENDE PANNELLI
                                        Serial.println("controllo 3 ");
                                        Serial.println();  
                                        Serial.print(voltage);
                                        Serial.println();  
                                        Serial.println("######## pannelli ON ######## tensione comepresa tra 47-69V");
                                        Serial.println(Relay); 
                                        delay(500);                                                                                   
                                          }
                                                                     else {
                                                                      int sensorValue = analogRead(A0);
                                                                      float voltage = sensorValue * (105 / 1023.0);

                                                                      if  (sensorValue >681 )      //      if  (sensorValue >800 )   650 66,71V    //  610
                                                        { 
                                                                      digitalWrite(Relay, HIGH);
                                                                      Serial.println();
                                                                      Serial.println("@@@@@@@ pannelli OFF @@@@@@   >810    se maggiore di 50v");
                                                                      delay(500);        // delay in between reads for stability
                                                                            } 
                                                                            } 
                                                                            } 
                                                                                 }


void controllo10(void) {                                  // @@@@@@@@@@@@@@@  1200w @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  int sensorValue = analogRead(A0);                                                  
  float voltage = sensorValue * (105 / 1023.0);
if  (voltage > 72.00 and LedPin3Status ==HIGH ){
                                               


                                               
                                               digitalWrite(ledPin, HIGH);
                                               digitalWrite(ledPin3, LOW);
                                               //  Serial.println();
                                               //   Serial.println(" __  |\  | On On On On On On");
                                               Serial.println("|  | | \ | accensione Scalabagno  1200w >760 proviamo ad aumentare....");
                                               //   Serial.println("|__| |  \| ON ON ON ON");
                                                    Serial.println();
                                                     Serial.println("controllo 10 ");
                                                     Serial.println();
                                                     Serial.println("ATTESA CHE L'inverter RISPONDI IN POTENZA");
                                                     Serial.println("10 Secondi......");
                                                     delay(500); 
                                                    // requesting required link
                                                    http.begin("http://192.168.xx.xx/api/relay/0?apikey=xxxxxxxxxxxxxxxx&value=1"); //HTTP  REPLACE IP relay AND APYKEY
                                                    // start connection and send HTTP header
                                                    int httpCode = http.GET();
                                                    http.end(); 
                                                    
                                               //     delay(3000);        // delay in between reads for stability
                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(500);

                                                                    digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                                    digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin, HIGH);
                                                                   delay(200);
                                                   
                                                   
                                                   }
                                                                                  
                  }


void controllo12(void) {  //###############################################  1200W OFF  ############################################################################################
                                                                                                                
                     int sensorValue = analogRead(A0);                                                  
                    float voltage = sensorValue * (105 / 1023.0);
                  Serial.println("controllo12");
                  Serial.print  ("VOLTAGGIO INVERTER = ");
                  Serial.print(voltage);
                  Serial.println("V");
                  Serial.print("SCALA SENSORE      = ");
                  Serial.println(sensorValue);
                     
           
                                               
                                               if  (voltage <=63.00 and ledPin == HIGH){
                                                                       
                                                    digitalWrite(ledPin, LOW);
                                                    digitalWrite(ledPin3, HIGH);
                                                    Serial.println("controllo 12 <=635");
                                                    Serial.println();
                                                    Serial.println("|  ||__ |__ Scaldabagno 1200W <= 63.00V OFF on 600");      // on
                                                   
                                                // start connection and send HTTP header
                                                http.begin("http://192.168.xx.xx/api/relay/1?apikey=xxxxxxxxxxxxxxxx&value=1"); //HTTP  REPLACE IP relay AND APYKEY
                                                int httpCode = http.GET();
                                                http.end();
                                                delay(3000);
                                                digitalWrite(ledPin, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(500);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(500);

                                                                    digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);
                                                                    digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);
                                                                   digitalWrite(ledPin3, LOW); 
                                                                   delay(200);   
                                                                   digitalWrite(ledPin3, HIGH);
                                                                   delay(200);

                                                                      }
                              }

void controllo4(void) {  //##############################################  600W 0FF ####################################################################################
                                                                                                                
                     int sensorValue = analogRead(A0);                                                  
                    float voltage = sensorValue * (105 / 1023.0);
                  Serial.println("controllo4");
                  Serial.print  ("VOLTAGGIO INVERTER = ");
                  Serial.print(voltage);
                  Serial.println("V");
                  Serial.print("SCALA SENSORE      = ");
                  Serial.println(sensorValue);
                   controllo6();  
                //     if  (sensorValue > 520){
                                               
                                          if  (voltage <=63.0){
                                                    
                                              digitalWrite(ledPin3, LOW);
                                              Serial.println("                   controllo     <=    63.0");
                                              Serial.println();
                                              Serial.println("|  ||__ |__ Scaldabagno 600W OFF <=   63.0V ");      // on 54.91V
                                              Serial.println();
                                       //     Serial.println("|  ||__ |__ Scaldabagno 1200W <640 OFF");      // on
                                                   
                                          // start connection and send HTTP header
                                 
                                          //    http.begin("http://192.168.xx.xx/api/relay/0?apikey=xxxxxxxxxxxxxxxx&value=0"); //HTTP REPLACE IP relay AND APYKEY
                                          //    delay(500);
                                                http.begin("http://192.168.xx.xx/api/relay/1?apikey=xxxxxxxxxxxxxxxx&value=0"); //HTTP REPLACE IP relay AND APYKEY
                                                int httpCode = http.GET();
                                                http.end();
                                                delay(1000);

                                                                        
                                                            //           }
                                                                        }
                                                                        }
void controllo6(void) {            //ççççççççççççççççççççççççççççççççççççççç   RELAY PANNELLI   çççççççççççççççççççççççççççççççççççççççççççççççççççççç
                                        
                      int sensorValue = analogRead(A0);
                      float voltage = sensorValue * (105 / 1023.0);
                      time_t now = time(nullptr);
                      struct tm* p_tm = localtime(&now);
                      if  (sensorValue > 470 && sensorValue <= 680 && p_tm->tm_hour >=6 && p_tm->tm_hour <= 19){
                         //  delay(6000);        // delay in between reads for stability
                           digitalWrite(Relay, LOW);
                                                 
                                                 
                         Serial.println();
                         Serial.println("controllo 6 ");
                         Serial.println("                      @@@@@@@ pannelli ON    @@@@@@   acceso tra 6 e 17  >470 <780 "); 
                         
                         Serial.println();
                                    
                                    }
                                                                                                             
                                                                                                             
                                                             else {
                                                                int sensorValue = analogRead(A0);
                                                                float voltage = sensorValue * (105 / 1023.0);

                                                                 if  (sensorValue >681)  //650 66,71V    //  610
                                                                 { 
                                                                  digitalWrite(Relay, HIGH);
                                                                  Serial.println("controllo 6 ");
                                                                  Serial.println();
                                                                  Serial.println("@@@@@@@ pannelli OFF >810 @@@@@@");
                                                                  Serial.println();
                                                                  
                                                                 } 
                                                             }
                                                             
                           
                          }
                                                                                
                                                                                                                    
 void controllo5(void) {         //#################################################    600W on     ######################################################################################                            
  int sensorValue = analogRead(A0);
float voltage = sensorValue * (105 / 1023.0);
Serial.println("cottrollo 5 ");
Serial.println();
Serial.print(voltage);
Serial.println();
if  (voltage < 63.00 and ledPin == HIGH)
                                                      {                                  
                                                        digitalWrite(ledPin3, LOW);                          
                                                        // requesting required link 
                                                       
                                                        Serial.println("controllo 5 600W OFF <63.00v");
                                            //            Serial.println();
                                            //            Serial.println("|  ||__ |__ Scaldabagno 600W on   >640 e <740");      // on
                                                        
                                            //            // start connection and send HTTP header
                                           //              http.begin("http://192.168.xx.xx/api/relay/1?apikey=xxxxxxxxxxxxxxxxx&value=1"); //HTTP REPLACE IP relay AND APYKEY
                                           //             int httpCode = http.GET();
                                           //             http.end();
                                          //              delay(1000); 
                                      //                   }                            
                                      //        else 
                                     //                 { 
                                                        digitalWrite(ledPin3, LOW);
                                                        Serial.println("|  ||__ |__ Scaldabagno 600W off   >640 e <740   NON VERIFICATO  SPEGNI ");      // on
                                                         http.begin("http://192.168.xx.xx/api/relay/1?apikey=xxxxxxxxxxxxxxxxx&value=0"); //HTTP   REPLACE IP relay AND APYKEY
                                                        int httpCode = http.GET();
                                                        http.end();
                                                        delay(1000);       
                                                      }
                       
                       }
void controllo14(void){
int sensorValue = analogRead(A0);
                                                                float voltage = sensorValue * (105 / 1023.0);

                                                                 if  (sensorValue >681)  //650 66,71V    //  610
                                                                 { 
                                                                  digitalWrite(Relay, HIGH);
                                                                  }
                       }

void controllo11(void){
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
if (p_tm->tm_hour >=21 and p_tm->tm_min >=55 and p_tm->tm_hour <= 22){
   
digitalWrite(Relay, HIGH);
   delay(1000);  
   digitalWrite(Relay, LOW);
   digitalWrite(Relay, HIGH);
   delay(200);  
   digitalWrite(Relay, LOW);
   digitalWrite(Relay, HIGH);
   delay(200);  
   digitalWrite(Relay, LOW);
    digitalWrite(Relay, LOW);
   digitalWrite(Relay, HIGH);
   delay(200);  
   digitalWrite(Relay, LOW);
   digitalWrite(Relay, HIGH);
   delay(200);  
   digitalWrite(Relay, LOW);

}
   
}

void controllo2(void) {                //          S T U F E T T A ip stufeta >>>> >>>>>>>>>  > >>>>  >>>>  >>> >>>>>  >>>>< < > > > > < < >> < <> > >  
  
int sensorValue = analogRead(A0);
float voltage = sensorValue * (105 / 1023.0);
                                                 if  (sensorValue > 570) {                                                          
                                                                                                              
                                                            if  (sensorValue > 670 and LedPin2Status == LOW  ){     //  650
                                                            //  unsigned long currentMillis2 = millis();
                                                      //        if ( currentMillis2 - previousMillis2 > watchdog ) {
                                                            //         previousMillis2 = currentMillis2;
                                                              digitalWrite(ledPin2, HIGH);
                                                                     http.begin("http://192.168.xx.xx/api/relay/0?apikey=xxxxxxxxxxxxxxxxx&value=1"); //HTTP REPLACE IP stufetta relay AND APYKEY
                                                          //             Serial.println(" __  |\  | On 2 On 2");
                                                                       Serial.println("|  | | \ | stufetta ON");
                                                          //             Serial.println("|__| |  \| On 2 On 2");
                                                          //             Serial.println();
                                                                     
                                                                     // start connection and send HTTP header
                                                                     int httpCode = http.GET();

                                                                     http.end(); 

                                                           
                                                                                  
                                                                  delay(1000);        // delay in between reads for stability
                                          
                                                         //          }
                                                                  }
                                                           else {

                                                            if  (sensorValue < 640 and LedPin2Status == HIGH)  //650 66,71V    //  610 ????????????????????
                                              { 
                                                            digitalWrite(ledPin2, LOW);
                                    
                                                           Serial.println("cottrollo 2 ");
                                                           Serial.println(); 
                                                 
                                                            http.begin("http://192.168.xx.xx/api/relay/0?apikey=xxxxxxxxxxxxxxxxx&value=0"); //HTTP   REPLACE IP stufetta relay AN
                                                         //            Serial.println(" __  __  __ Off 2 Off 2");
                                                                    Serial.println("|  ||__ |__stufetta <640 OFF");
                                                        //            Serial.println("|__||   |   Off 2 Off 2");
                                                        //             Serial.println();
                                                            
                                                            // start connection and send HTTP header
                                                            int httpCode = http.GET();
                                                            http.end(); 

                                                                  delay(1000);        // delay in between reads for stability
                                                            }}
                                        }
                                        }
                                                    
                                                                                                                       
                                                                                                                  
                                                                                          
                                                                                          
                                                                                          

                                                                                                
     //                    int httpCode = http.GET();                                                           

   //  client.print("192.168.xx.xx:8080/json.htm?type=command&param=udevice&idx=17&nvalue=0&svalue=");    //  IP DOMOTICZ
   //    client.print(data);
                                                                                     

 //                                                                                                              http.end(); 
                                                                                            
                                                                                            
                                   
                       //  _____________*é*é*é*éé*é_______________   //       
                       //  é*é*é*é*é*é*é*é__________*é*é*éé**éé**éé* //      
                                   


  // Domoticz format /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=HUM;HUM_STAT
void loop()
{
  controllo();
   controllo6();     // rele timer
  controllo1();      // boiler 
   controllo14();
  controllo5();
   controllo14();
 // controllo3();   // rele pannelli
  controllo4();     //  1200 off <640
   controllo14();
  controllo2();     //  600 off  <640
  controllo14();
  controllo6();     // rele timer
controllo7();       // orologio
    controllo14();
    controllo8();   // ota
    controllo14();
    controllo9();   //  web
    controllo14();
    controllo10();  // 1200W se 600è poco
    controllo14();
    controllo12();    //  1200 off <680
//    controllo13();

//    controllo11(); //  test controllo
    ArduinoOTA.handle();


      }        
          

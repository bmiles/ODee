#include <SPI.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Cosm.h>

unsigned long previousTime = 0;
int led = 10;
int detector = A2;
int baselinePin = 3;
long interval = 20000; //10 second delay
//int currentRead = 0;
//int previousRead = 1024; //previous detector read from last cycle
float normalisedRead = .00000;
int baseRead1 = 0;
int baseRead2 = 0;
int baseRead3 = 0;
float baseline = 0;
float od = .00000;
float sensorVal = 0;
int lightDC = 0; //duty cycle flag for LED output

char ssid[] = "xxx"; //  your network SSID (name)
char pass[] = "xxx";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Your Cosm key to let you upload data
char cosmKey[] = "xxx";

// Define the strings for our datastream IDs
char sensorId[] = "Test2";
CosmDatastream datastreams[] = {
  CosmDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed, you need to input FeedID from Cosm
CosmFeed feed(00000, datastreams, 1 /* number of datastreams */);

WiFiClient client;
CosmClient cosmclient(client);

void setup() {

pinMode(baselinePin,INPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Starting single datastream upload to Cosm...");
  Serial.println();

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();


}

void loop() {

   //baseline absorbance measurement
   //Serial.println(digitalRead(baselinePin));
  if (digitalRead(baselinePin) == HIGH) {
    delay(10000); //delay to allow ambient to reach equilibrium
    analogWrite(led,64);
    baseRead1 = analogRead(detector);
    delay(1000);
    Serial.println("Baseline measurement 1 " + baseRead1);
    baseRead2 = analogRead(detector);
    delay(1000);
    Serial.println("Baseline measurement 2 " + baseRead2);
    baseRead3 = analogRead(detector);
    delay(1000);
    Serial.println("Baseline measurement 3 " + baseRead3);
    baseline = ((baseRead1+baseRead2+baseRead3)/3.0);
    Serial.println(baseline);
    analogWrite(led,0);
  }


  unsigned long currentTime = millis();
  analogWrite(led,64);
  sensorVal = analogRead(detector);
  if (currentTime - previousTime > interval){
    previousTime = currentTime;
    Serial.println(sensorVal);

    if (sensorVal < 256){
    analogWrite(led,255);
    delay(2000);
    normalisedRead = analogRead(detector)/4.0;
    lightDC = 100;
    }
    else if (sensorVal < 512 && sensorVal >= 256){
    analogWrite(led,128);
    delay(2000);
    normalisedRead = analogRead(detector)/2.0;
    lightDC = 50;
    }
    else if (sensorVal >= 512){
    analogWrite(led,64);
    delay(2000);
    normalisedRead = analogRead(detector);
    lightDC = 25;
    };

    Serial.print("baseline ");
    Serial.println(baseline);
    Serial.print("LED duty cycle ");
    Serial.println(lightDC);
    Serial.print("analog read ");
    Serial.println(analogRead(detector));
    Serial.print("normalised read ");
    Serial.println(normalisedRead);
    od = - log(normalisedRead/baseline);
    Serial.print("OD600 = ");
    Serial.println(od);

  datastreams[0].setFloat(od);

    Serial.print("Read sensor value ");
    Serial.println(datastreams[0].getFloat());

    Serial.println("Uploading it to Cosm");
    int ret = cosmclient.put(feed, cosmKey);
    Serial.print("cosmclient.put returned ");
    Serial.println(ret);

    Serial.println();
  }
  else{
    analogWrite(led,0);
  }

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

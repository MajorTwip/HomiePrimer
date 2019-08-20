#include <Arduino.h>
#include <Homie.h>
#include <FS.h>
#define CONFIGFILE "/homie/config.json"
const char* config = "{ \
  \"name\": \"CSH Primer\", \
  \"wifi\": { \
    \"ssid\": \"YOUR_SSID\", \
    \"password\": \"YOUR_WIFI_PW\" \
  }, \
  \"mqtt\": { \
    \"host\": \"YOUR_MQTT_SERVER\" \
  }, \
  \"ota\": { \
    \"enabled\": true \
  }, \
  \"settings\": { \
  } \
}";

boolean OVERWRITECONFIG = true;

void writeconf(){
  SPIFFS.remove(CONFIGFILE);
  File conf = SPIFFS.open(CONFIGFILE,"a");
  if(!conf){
    Serial<<"Error creating Configfile"<<endl;
    return;
  }
  conf.print(config);
  conf.close();
}

void setup()
{
  Serial.begin(9600);
  bool spiffsloaded;
  spiffsloaded = SPIFFS.begin();

  if (!spiffsloaded)
  {
    Serial << "Opening FS unsecessful, formating" << endl;
    SPIFFS.format();
    spiffsloaded = SPIFFS.begin();
  };

  if (spiffsloaded)
  {
    Serial << "FS opened, looking for CONFIGFILE" << endl;
    if (!SPIFFS.exists(CONFIGFILE))
    {
      Serial << "No Configfile found, writing" << endl;
      writeconf();
    }
    else
    {
      if (OVERWRITECONFIG)
      {
        Serial<<"Ordered to overwrite config"<<endl;
        writeconf();
      }else{
        Serial << "Configfile found, booting" << endl;
      }
    }
  }
  Homie.getMqttClient().setKeepAlive(120);
  Homie_setFirmware("csh_primer", "0.0.13");
  Homie_setBrand("Comstock Homecontrol");
  Homie.setup();
}

void loop()
{
  Homie.loop();
}
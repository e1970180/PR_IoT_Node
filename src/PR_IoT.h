#ifndef PR_IoT_h
#define PR_IoT_h

    #include <ESP8266WiFi.h>            //https://github.com/esp8266/Arduino
    #include <PubSubClient.h>           //https://github.com/knolleary/pubsubclient

    extern  WiFiClient      ESP8266Client;
    extern  PubSubClient    MQTTclient;

    extern  const char*     MQTT_clientID;

    extern  struct IoT_Msg  incomeMsg;

	#include <PR_IoT_TimeClass.h>
    #include "PR_IoT_Node.h"
    #include "PR_IoT_DeviceClass.h"								 				

    #define     PR_DEBUG_ON
    #define     PR_DBG_SERIALDEVICE Serial
    #include 	<PR_DBG.h>   
    
	//extern  const 	char* 	WiFi_ssid;
	//extern  const 	char* 	WiFi_password;
	extern  IPAddress   	MQTT_server;
	extern  const 	int   	MQTT_port;  
													
	extern  const 	char* 	MQTT_username;
	extern  const 	char* 	MQTT_password;
	
	extern  const	String	siteSubTopic;

#endif

#ifndef PR_IoT_NodeMQTTClass_h
#define PR_IoT_NodeMQTTClass_h

    #include <arduino.h>
    #include <string.h>
	#include <Esp.h>                    // core
	#include <EEPROM.h>
	
	#include <ESP8266WiFi.h>            //https://github.com/esp8266/Arduino
    #include <PubSubClient.h>           //https://github.com/knolleary/pubsubclient
	
    #include "PR_IoT.h"
	
	#define	PR_IoT_MQTT_NAMEPW_MAX_LEN				16
	#define	PR_IoT_MQTT_EEPROM_ADDR_CREDITALS		0
	#define	EEPROM_CREDITALS_SALT					12321
	
    struct  IoT_Msg {
		//String	location;
		//String	nodeName;
		//String	control;
        String  deviceName;
        String  deviceTopic;
        String  payload;
        bool    newMsgFlag;
    }; //TODO replace by class and add myString.reserve(233);

	class	PR_IoT_MQTTcreditals	{
		public:
			int16_t		salt; 
			IPAddress	serverIP;
			uint16_t	port;
			char		username[PR_IoT_MQTT_NAMEPW_MAX_LEN+1];
			char		password[PR_IoT_MQTT_NAMEPW_MAX_LEN+1];

			bool		save();
			bool		restore();
	};
	
	
    class PR_IoT_NodeMQTTClass {
        public:
			PR_IoT_NodeMQTTClass(String location, String nodeName);	
			
			PR_IoT_MQTTcreditals	creditals;
			
			bool    connect(const char* clientID);
			void	represent();
			bool    subscribe(); 
			
			bool 	postMsg(const String deviceName, const String subTopic,const String payload);
			void 	callback(const char* topic, byte* payload, unsigned int length);
    
			bool	isOnline()				{	return _online;			}
			void	setOnline(bool state)	{	_online = state; 		}
			String	getNodeSubTopic()		{	return _nodeSubTopic;	}
			//const 	char*	getclientID()	{	return _clientID;		}
			
		protected:
			//const 	char*	_clientID;
			bool			_online 		= false;
			const String	_location;
			const String	_nodeName;
			const String	_nodeSubTopic;
			const String	_Cmd 			= "COMMAND";
			const String	_Status 		= "STATUS";
	
	};

    extern  struct IoT_Msg      	inMsg;
	extern	PR_IoT_NodeMQTTClass	NodeMQTT;
	extern  void 					parceRootSubTopic (String *topic, String *rootSubTopic);
    extern  void    				callbackMQTT(const char* topic, byte* payload, unsigned int length);
#endif    

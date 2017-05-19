#ifndef PR_IoT_NodeMQTTClass_h
#define PR_IoT_NodeMQTTClass_h

    #include <arduino.h>
    #include <string.h>
	#include <Esp.h>                    // core

    #include "PR_IoT.h"

    struct  IoT_Msg {
		//String	location;
		//String	nodeName;
		//String	control;
        String  deviceName;
        String  deviceTopic;
        String  payload;
        bool    newMsgFlag;
    }; //TODO replace by class and add myString.reserve(233);

    class PR_IoT_NodeMQTTClass {
        public:
			PR_IoT_NodeMQTTClass(String location, String nodeName);	
			
			bool    connect();
			void	represent();
			bool    subscribe(); 
			
			bool 	postMsg(const String deviceName, const String subTopic,const String payload);
			void 	callback(const char* topic, byte* payload, unsigned int length);
    
			bool	isOnline()				{	return _online;			}
			void	setOnline(bool state)	{	_online = state; 		}
			String	getNodeSubTopic()		{	return _nodeSubTopic;	}
			const 	char*	getclientID()	{	return _clientID;		}
			
		protected:
			const 	char*	_clientID;
			bool			_online;
			const String	_location;
			const String	_nodeName;
			const String	_nodeSubTopic;
			const String	_Cmd = "COMMAND";
			const String	_Status = "STATUS";
	
	};

    extern  struct IoT_Msg      inMsg;
	extern	PR_IoT_NodeMQTTClass	NodeMQTT;
	extern  void 	parceRootSubTopic (String *topic, String *rootSubTopic);
    extern  void    callbackMQTT(const char* topic, byte* payload, unsigned int length);
#endif    

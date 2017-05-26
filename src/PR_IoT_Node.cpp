#include "PR_IoT_Node.h"   
    
    struct IoT_Msg  	inMsg;
	

	PR_IoT_NodeMQTTClass::PR_IoT_NodeMQTTClass(String location, String nodeName)
		:	
			//_clientID(clientID),
			_online(false),
			_location(location),
			_nodeName(nodeName),
			_nodeSubTopic( siteSubTopic +"/"+ _location +"/"+ _nodeName +"/")
			
		{} 
	
    bool 	PR_IoT_NodeMQTTClass::connect(const char* clientID) {
		
		if ( MQTTclient.connected() ) return (_online = true);
		
		PR_DBGT("MQTT connection FAILED. Reason=") PR_DBGVLN(MQTTclient.state())	
		
		_online = MQTTclient.connect(clientID);	//RE-connect
			
		if (_online)	{			//on sucessful reconnect
			represent();
			subscribe();
		}
		return _online;
    }

	void 	PR_IoT_NodeMQTTClass::represent() {
	    MQTTclient.publish((_nodeSubTopic + _Status).c_str(), "HELLO");  //announce the node to the system
	}
    

    bool    PR_IoT_NodeMQTTClass::subscribe() {   	//listen commands to the node
													//site/location/node/+/"conmmand"/# 
													//site/sys/command/# 
		PR_DBGT("subscribe site/location/node/# ")
		bool subscResult = MQTTclient.subscribe( (_nodeSubTopic + "+/COMMAND/#").c_str() );
		subscResult = subscResult && MQTTclient.subscribe( (siteSubTopic + "/sys/COMMAND/#").c_str() );
		
		if (subscResult) PR_DBGTLN(" Success") else PR_DBGTLN(" Fail")

		return subscResult;
	}

    void    callbackMQTT(const char* topic, byte* payload, unsigned int length) {
        NodeMQTT.callback(topic, payload, length);
        }

    void 	PR_IoT_NodeMQTTClass::callback(const char* topic, byte* payload, unsigned int length) {

	// parcing full topic
//				site/location/node/device/"COMMAND"/#
//				site/location/node/device/"STATUS"/#
//				site/sys/"COMMAND"/# 
//
    	inMsg.newMsgFlag = false;

		payload[length] = '\0';					
    	inMsg.payload = String((char*)payload);
		
		inMsg.deviceTopic = topic;

		if ( inMsg.deviceTopic.startsWith(_nodeSubTopic) ) inMsg.deviceTopic.remove(0, _nodeSubTopic.length() );	//remove "site/location/node/"
		else if ( inMsg.deviceTopic.startsWith(siteSubTopic) ) inMsg.deviceTopic.remove(0, siteSubTopic.length() ); //remove "site/"
		else return;
		
		//here deviceTopic has "device/"COMMAND"/#"
		parceRootSubTopic(&inMsg.deviceTopic , &inMsg.deviceName); //topic -> ( topic, rootSubTopic )

		if ( inMsg.deviceTopic.startsWith(_Cmd) ) inMsg.deviceTopic.remove(0, _Cmd.length() ); else return;			//remove "/COMMAND"
		if ( inMsg.deviceTopic.startsWith("/") ) inMsg.deviceTopic.remove(0, 1);
		
		inMsg.newMsgFlag = true;
 
		PR_DBGTLN("Msg parced")
		PR_DBGVLN(inMsg.deviceName)
		PR_DBGVLN(inMsg.deviceTopic)
		PR_DBGVLN(inMsg.payload)
		PR_DBGTLN("----")
		
    } //callbackMQTT

    bool    PR_IoT_NodeMQTTClass::postMsg(const String deviceName, const String subTopic, const String payload) {
        
		String tmp =  _nodeSubTopic + deviceName + "/" + _Status;
		if (subTopic != "") tmp = tmp + "/" + subTopic;
		
		bool result = MQTTclient.publish( tmp.c_str(), payload.c_str() );
        
				PR_DBGT("MQTTclient post msg .. /status/") PR_DBGV(tmp) PR_DBGT(" ") PR_DBGV(payload) PR_DBGTLN(" ")
				if (result) PR_DBGTLN("OK") else PR_DBGTLN("FAIL")
        
		return result;       
    }
    
	void    parceRootSubTopic (String *topic, String *rootSubTopic) {
		int position = topic->indexOf('/');
		if (position>=0) {
			*rootSubTopic = topic->substring(0, position);
			topic->remove(0, position+1);
		} 
		else {
			*rootSubTopic = *topic;
			*topic = "";
		}
	}
	
	
	


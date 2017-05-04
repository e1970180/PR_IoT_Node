#include "PR_IoT_Node.h"   
    
    struct IoT_Msg  	inMsg;
	

	PR_IoT_NodeMQTTClass::PR_IoT_NodeMQTTClass(String location, String nodeName)
		:	
			_clientID("unit1"),
			_online(false),
			_location(location),
			_nodeName(nodeName),
			_nodeSubTopic( siteSubTopic +"/"+ _location +"/"+ _nodeName ),
			_nodeSubTopicCmd  ( siteSubTopic +"/"+ _location +"/"+ _nodeName +"/command/" ),
			_nodeSubTopicState( siteSubTopic +"/"+ _location +"/"+ _nodeName +"/state/" )
		{} 
	
    bool 	PR_IoT_NodeMQTTClass::connect() {
		
		if ( MQTTclient.connected() ) return (_online = true);
		
		PR_DBGT("MQTT connection FAILED. Reason=") PR_DBGVLN(MQTTclient.state())	
		
		_online = MQTTclient.connect(_clientID);	//RE-connect
			
		if (_online)	{			//on sucessful reconnect
			represent();
			subscribe();
		}
		return _online;
    }

	void 	PR_IoT_NodeMQTTClass::represent() {
	    MQTTclient.publish((_nodeSubTopicState + "sys").c_str(), "HELLO");  //announce the node to the system
	}
    

    bool    PR_IoT_NodeMQTTClass::subscribe() {   	//listen commands to the node
													//site/location/node/"conmmand"/# 
													//site/sys/command/# 
		PR_DBGT("subscribe site/location/node/conmmand/# ")
		bool subscResult = MQTTclient.subscribe( (_nodeSubTopicCmd + "#").c_str() );
		subscResult = subscResult && MQTTclient.subscribe( (siteSubTopic + "/sys/command/#").c_str() );
		
		if (subscResult) PR_DBGTLN(" Success") else PR_DBGTLN(" Fail")

		return subscResult;
	}

    void    callbackMQTT(const char* topic, byte* payload, unsigned int length) {
        NodeMQTT.callback(topic, payload, length);
        }

    void 	PR_IoT_NodeMQTTClass::callback(const char* topic, byte* payload, unsigned int length) {

	// parcing full topic
//				site/location/node/"command"/device/#
//				site/location/node/"state"/device/#
//				site/sys/command/# 
//
    	inMsg.newMsgFlag = false;

		payload[length] = '\0';					
    	inMsg.payload = String((char*)payload);
		
		inMsg.deviceTopic = topic;
		
		if ( inMsg.deviceTopic.startsWith(_nodeSubTopicCmd) ) {						//device message
			
			inMsg.deviceTopic.remove(0, _nodeSubTopicCmd.length() );
			parceRootSubTopic(&inMsg.deviceTopic , &inMsg.deviceName); //topic -> ( toppic, rootSubTopic ) 
			inMsg.newMsgFlag = true;
		} 
		else if ( inMsg.deviceTopic.startsWith(siteSubTopic + "/sys/command") ) {	//sys message
				
			inMsg.deviceTopic.remove(0, (siteSubTopic + "/sys/command/").length() );
			inMsg.deviceName = "sys";
			inMsg.newMsgFlag = true;
		} 
		PR_DBGTLN("Msg parced")
		PR_DBGVLN(inMsg.deviceName)
		PR_DBGVLN(inMsg.deviceTopic)
		PR_DBGVLN(inMsg.payload)
		PR_DBGTLN("----")
		
    } //callbackMQTT

    bool    PR_IoT_NodeMQTTClass::postMsg(const String deviceTopic, const String payload) {
               
		bool result = MQTTclient.publish( (_nodeSubTopicState + deviceTopic).c_str(), payload.c_str() );
        
				PR_DBGT("MQTTclient post msg .. /status/") PR_DBGV(deviceTopic) PR_DBGT(" ") PR_DBGV(payload) PR_DBGTLN(" ")
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
	
	
	


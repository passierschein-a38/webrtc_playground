
#include <iostream>

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/base/scoped_ptr.h"
#include "talk/base/refcount.h"

bool g_running = true;

class StdOutObserver : public webrtc::PeerConnectionObserver
{

public: 

	virtual void OnError(){
		std::cout << "OnError" << std::endl;
	};

	// Triggered when the SignalingState changed.
	virtual void OnSignalingChange(
		webrtc::PeerConnectionInterface::SignalingState new_state) {	
		std::cout << "OnSignalingChange" << new_state << std::endl;
	}

	// Triggered when SignalingState or IceState have changed.
	// TODO(bemasc): Remove once callers transition to OnSignalingChange.
	virtual void OnStateChange(StateType state_changed) {
		std::cout << "OnStateChange: " << state_changed << std::endl;
	}

	// Triggered when media is received on a new stream from remote peer.
	virtual void OnAddStream(webrtc::MediaStreamInterface* stream){
		std::cout << "OnAddStream: " << stream << std::endl;
	}

	// Triggered when a remote peer close a stream.
	virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream){
		std::cout << "OnRemoveStream: " << stream << std::endl;
	}

	// Triggered when a remote peer open a data channel.
	// TODO(perkj): Make pure virtual.
	virtual void OnDataChannel(webrtc::DataChannelInterface* data_channel){
		std::cout << "OnDataChannel: " << data_channel << std::endl;
	}

	// Triggered when renegotiation is needed, for example the ICE has restarted.
	virtual void OnRenegotiationNeeded(){
		std::cout << "OnRenegotiationNeeded: " << std::endl;
	}

	// Called any time the IceConnectionState changes
	virtual void OnIceConnectionChange(
		webrtc::PeerConnectionInterface::IceConnectionState new_state) {		
		std::cout << "OnIceConnectionChange: " << std::endl;
	}

	// Called any time the IceGatheringState changes
	virtual void OnIceGatheringChange(
		webrtc::PeerConnectionInterface::IceGatheringState new_state) {
		std::cout << "OnIceGatheringChange: " <<  new_state << std::endl;
	}

	// New Ice candidate have been found.
	virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate){
		std::cout << "OnIceCandidate: " << candidate << std::endl;
	}

	// TODO(bemasc): Remove this once callers transition to OnIceGatheringChange.
	// All Ice candidates have been found.
	virtual void OnIceComplete() {
		std::cout << "OnIceComplete: " << std::endl;
	}
};


class StdOutSDObserver : public  webrtc::CreateSessionDescriptionObserver
{
public:

	virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc){
		std::string msg;
		desc->ToString(&msg);
		std::cout << msg << std::endl;
	}

	virtual void OnFailure(const std::string& error){
		std::cout << error << std::endl;
	}
};


int main(int argc, char** argv)
{

	//-- Media Constrains
	webrtc::MediaConstraintsInterface* media_constrains = NULL;

	//-- Ice Server Setup
	webrtc::PeerConnectionInterface::IceServer ice_server;
	ice_server.uri = "stun:stun.l.google.com:19302";

	webrtc::PeerConnectionInterface::RTCConfiguration rtc_config;
	rtc_config.servers.push_back( ice_server );

	//-- Create peer connection factory
	talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
		peer_connection_factory = webrtc::CreatePeerConnectionFactory();

	StdOutObserver peer_connection_observer;

	talk_base::scoped_refptr<webrtc::PeerConnectionInterface>
		peer_connection = peer_connection_factory->CreatePeerConnection( rtc_config,
																	     media_constrains,
																		 NULL,
																		 NULL,
																		 &peer_connection_observer);

	talk_base::RefCountedObject<StdOutSDObserver>* create_observer = new talk_base::RefCountedObject<StdOutSDObserver>();

	peer_connection->CreateOffer( create_observer,
							  	  media_constrains );
	
	while (g_running){
		Sleep(50);
	}

	return 0;
}

#include <iostream>
#include <vector>
#include <fstream>

#include "talk/examples/native_to_browser/mediastream.h"
#include "talk/examples/native_to_browser/adapter/peerconnection.h"
#include "talk/examples/native_to_browser/observer/setsessiondescription.h"
#include "talk/examples/native_to_browser/observer/createsessiondescription.h"
#include "talk/examples/native_to_browser/observer/peerconnection.h"

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"

#include "talk/base/refcount.h"
#include "talk/base/ssladapter.h"
#include "talk/base/win32socketinit.h"
#include "talk/base/win32socketserver.h"
#include "third_party/jsoncpp/source/include/json/json.h"

HANDLE h_event = CreateEvent(0, TRUE, FALSE, L"wait_event");

talk_base::scoped_ptr<webrtc::SessionDescriptionInterface> session_description;

std::vector<std::string> local_ice;
talk_base::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection;



/*
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

		std::string ice;
		if (candidate->ToString(&ice)){
			Json::StyledWriter writer;
			Json::Value json;
			json["candidate"] = ice;
			local_ice.push_back( writer.write(json) );
		}
		
	}

	// TODO(bemasc): Remove this once callers transition to OnIceGatheringChange.
	// All Ice candidates have been found.
	virtual void OnIceComplete() {
		std::cout << "OnIceComplete: " << std::endl;

		//-- write local ice to file
		{
			std::ofstream file;
			file.open("e:/tmp/native_ice", std::ios::binary | std::ios::out);

			//-- write out the ice candidates as json
			std::vector<std::string>::const_iterator it = local_ice.begin();

			while (it != local_ice.end()){
				file << *it;
				++it;
			}

			file.close();
		}

		local_ice.clear();		
	}
};


class StdOutSDObserver : public  webrtc::CreateSessionDescriptionObserver
{
public:

	virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc){

		ASSERT(peer_connection.get());

		talk_base::RefCountedObject<n2b::observer::SetSessionDescription>* observer = new talk_base::RefCountedObject<n2b::observer::SetSessionDescription>();
		
		observer->offer_created.connect(this, &StdOutSDObserver::offer_created);

		peer_connection->SetLocalDescription(observer.get(), desc); //-- trigger ice

		std::string msg;
		desc->ToString(&msg);
		std::cout << msg << std::endl;
		
		//-- write local desc to file
		{
			std::ofstream file;
			file.open("e:/tmp/native_offer", std::ios::binary | std::ios::out  | std::ios::trunc );
			file << msg;
			file.close();
		}
	}

	virtual void OnFailure(const std::string& error){
		std::cout << error << std::endl;
		//-- stop here do not set the event SetEvent(h_event);
	}

	virtual void offer_created(webrtc::SessionDescriptionInterface* offer){

		//do something

	}
};*/

int main(int argc, char** argv)
{

	//-- Setup
	talk_base::EnsureWinsockInit();
	talk_base::Win32Thread w32_thread;
	talk_base::ThreadManager::Instance()->SetCurrentThread(&w32_thread);
	talk_base::InitializeSSL();

	//-- Media Constrains
	webrtc::MediaConstraintsInterface* media_constrains = NULL;

	//-- Ice Server Setup
	webrtc::PeerConnectionInterface::IceServer ice_server;
	ice_server.uri = "stun:stun.l.google.com:19302";

	webrtc::PeerConnectionInterface::RTCConfiguration rtc_config;
	rtc_config.servers.push_back( ice_server );

	//-- Create peer connection factory
	n2b::adapter::PeerConnectionPtr pc = n2b::adapter::PeerConnection::create(rtc_config, media_constrains);

	const std::string audio_track = "audioV0";
	pc->local_media_stream->addAudio(audio_track, media_constrains);
	
	

	std::string cmd;

	while (cmd != "x"){

		std::cout << "create_offer" << std::endl;
		std::cout << "handle_anwser" << std::endl;
		std::cout << "x - exit" << std::endl;

		std::cin >> cmd;

		if (cmd == "create_offer"){
			pc->createOffer(media_constrains);
		}

		if (cmd == "x"){
			break;
		}
	}
		
	//-- clean shutdown
	pc.release();

	return 0;
}
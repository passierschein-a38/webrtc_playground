
#include <iostream>
#include <fstream>

#include "talk/examples/native_to_browser/file_transport.h"
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
#include "talk/base/json.h"

void set_remote_description(n2b::adapter::PeerConnectionPtr& pc);
void read_offer_create_answer(n2b::adapter::PeerConnectionPtr& pc);

int main(int argc, char** argv)
{

	//-- Setup
	talk_base::EnsureWinsockInit();
	talk_base::Win32Thread w32_thread;
	talk_base::ThreadManager::Instance()->SetCurrentThread(&w32_thread);
	talk_base::InitializeSSL();

	//-- Media Constraints
	n2b::Constraints media_constraints;
	media_constraints.AddMandatory(webrtc::MediaConstraintsInterface::kOfferToReceiveAudio, webrtc::MediaConstraintsInterface::kValueTrue);
	media_constraints.AddMandatory(webrtc::MediaConstraintsInterface::kOfferToReceiveVideo, webrtc::MediaConstraintsInterface::kValueTrue);
	media_constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp, webrtc::MediaConstraintsInterface::kValueTrue);

	//-- Ice Server Setup
	webrtc::PeerConnectionInterface::IceServer ice_server;
	ice_server.uri = "stun:stun.l.google.com:19302";

	webrtc::PeerConnectionInterface::RTCConfiguration rtc_config;
	rtc_config.servers.push_back( ice_server );
	
	const std::string local_id = "native";
	const std::string remote_id = "browser";

	//-- Create peer connection factory	
	n2b::FileTransportPtr pFileTransport = new talk_base::RefCountedObject<n2b::FileTransport>("e:/signaling/", local_id, remote_id);
	n2b::adapter::PeerConnectionPtr pc = n2b::adapter::PeerConnection::create(n2b::TransportPtr(pFileTransport), rtc_config, &media_constraints);
	
	n2b::Constraints video_constraints;
	video_constraints.AddMandatory(webrtc::MediaConstraintsInterface::kMinHeight, 240);
	video_constraints.AddMandatory(webrtc::MediaConstraintsInterface::kMinWidth, 320);
	video_constraints.AddOptional(webrtc::MediaConstraintsInterface::kMinHeight, 480);
	video_constraints.AddMandatory(webrtc::MediaConstraintsInterface::kMinWidth, 640);
	
	const std::string video_track = "videoV0";
	pc->local_media_stream->addVideo(video_track, &video_constraints);

	n2b::Constraints audio_constraints;

	const std::string audio_track = "audioV0";
	pc->local_media_stream->addAudio(audio_track, &audio_constraints);
	
	std::string cmd;
	
	while (cmd != "x"){

		std::cout << "o - create a offer" << std::endl; 
		std::cout << "a - create a answer" << std::endl;
		std::cout << "rd - set remote description" << std::endl;
		std::cout << "x - exit" << std::endl;

		std::cin >> cmd;

		if (cmd == "o"){
			pc->createOffer(&media_constraints);
		}

		if (cmd == "rd"){
			pFileTransport->readRemoteSessionDescription();
			pFileTransport->readRemoteIceCandidates();
		}
		
		if (cmd == "a"){
			pFileTransport->readRemoteSessionDescription();
			pc->createAnswer(&media_constraints);
			pFileTransport->readRemoteIceCandidates();
		}

		if (cmd == "x"){
			break;
		}
	}
		
	//-- clean shutdown
	pc.release();

	return 0;
}


#include <iostream>

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

	//-- Media Constraints
	n2b::Constraints constraints;
	constraints.AddMandatory(webrtc::MediaConstraintsInterface::kOfferToReceiveAudio, webrtc::MediaConstraintsInterface::kValueTrue);
	constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp, webrtc::MediaConstraintsInterface::kValueTrue);	

	//-- Create peer connection factory
	n2b::adapter::PeerConnectionPtr pc = n2b::adapter::PeerConnection::create(rtc_config, &constraints);

	const std::string audio_track = "audioV0";
	pc->local_media_stream->addAudio(audio_track, media_constrains);
		
	std::string cmd;

	while (cmd != "x"){

		std::cout << "create_offer" << std::endl;
		std::cout << "handle_answer" << std::endl;
		std::cout << "x - exit" << std::endl;

		std::cin >> cmd;

		if (cmd == "create_offer"){
			pc->createOffer(media_constrains);
		}

		if (cmd == "handle_answer"){
			pc->handleAnswer();
		}

		if (cmd == "x"){
			break;
		}
	}
		
	//-- clean shutdown
	pc.release();

	return 0;
}
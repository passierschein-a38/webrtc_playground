#include <iostream>
#include <fstream>
#include <sstream>

#include "talk/examples/native_to_browser/adapter/peerconnection.h"
#include "talk/base/json.h"

namespace{
	const char kCandidateSdpMidName[] = "sdpMid";
	const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
	const char kCandidateSdpName[] = "candidate";
	const char kSessionDescriptionTypeName[] = "type";
	const char kSessionDescriptionSdpName[] = "sdp";
}

namespace n2b{ 
	namespace adapter {

		
PeerConnectionPtr PeerConnection::create(const webrtc::PeerConnectionInterface::RTCConfiguration cfg,
						      			 const webrtc::MediaConstraintsInterface* constraints)
{
	
	PeerConnectionPtr pc = new talk_base::RefCountedObject < PeerConnection > ;
	
	//-- create factory
	pc->_peer_connection_factory = webrtc::CreatePeerConnectionFactory();
	ASSERT(pc->_peer_connection_factory.get());

	//-- create observer
	pc->_peer_connection_observer = new talk_base::RefCountedObject<n2b::observer::PeerConnection>();
	ASSERT(pc->_peer_connection_observer.get());
	
	pc->_create_session_observer = new talk_base::RefCountedObject<n2b::observer::CreateSessionDescription>();
	ASSERT(pc->_create_session_observer.get());

	pc->_set_local_session_observer = new talk_base::RefCountedObject<n2b::observer::SetSessionDescription>();
	ASSERT(pc->_set_local_session_observer.get());

	pc->_set_remote_session_observer = new talk_base::RefCountedObject<n2b::observer::SetSessionDescription>();
	ASSERT(pc->_set_remote_session_observer.get());

	//-- create peer connection
	pc->_peer_connection = pc->_peer_connection_factory->CreatePeerConnection( cfg, 
																			   constraints, 
																			   NULL, 
																			   NULL, 
																			   pc->_peer_connection_observer.get() );

	ASSERT(pc->_peer_connection.get());


	//-- create local media stream
	pc->local_media_stream = new talk_base::RefCountedObject<n2b::MediaStream>( "local_media", pc->_peer_connection_factory );
	ASSERT(pc->local_media_stream.get());
	
	//-- connect signals and slots
	ASSERT(pc->setupSignals());
	
	return pc;
}

PeerConnection::~PeerConnection(){
	
}

bool PeerConnection::setupSignals()
{
	_peer_connection_observer->onIceStart.connect(this, &PeerConnection::onIceStart);
	_peer_connection_observer->onIceComplete.connect(this, &PeerConnection::OnIceComplete);
	_peer_connection_observer->onIceCandidate.connect(this, &PeerConnection::onIceCandidate);

	_create_session_observer->onSuccess.connect(this, &PeerConnection::onSessionDescription);

	_set_remote_session_observer->onSuccess.connect(this, &PeerConnection::onSetRemoteDescription);

	return true;	
}

void PeerConnection::onIceStart()
{
	std::cout << "onIceStart" << std::endl;
}

void PeerConnection::OnIceComplete()
{
	std::cout << "OnIceComplete" << std::endl;

	{//--write local ice to file
		std::ofstream file;
		file.open("e:/signaling/native_ice", std::ios::binary | std::ios::out | std::ios::trunc);
		

		Json::StyledWriter writer;
		Json::Value json;

		std::vector<std::string>::const_iterator it = local_ice.begin();

		while (it != local_ice.end()){
			json.append(*it);			
			++it;
		}

		file << writer.write(json);
		file.close();

		local_ice.clear();
	}
}

void PeerConnection::onIceCandidate(const webrtc::IceCandidateInterface* ice)
{
	if (!ice){
		return;
	}

	std::string sdp;
	if (!ice->ToString(&sdp)){
		std::cout << "Failed to get sdp" << std::endl;
	}
		
	Json::StyledWriter writer;
	Json::Value json;

	json[kCandidateSdpMidName] = ice->sdp_mid();
	json[kCandidateSdpMlineIndexName] = ice->sdp_mline_index(); 
	json[kCandidateSdpName] = "a=" + sdp;

	std::string candidate = writer.write(json);

	std::cout << "onIceCandidate" << std::endl;
	std::cout << candidate;
	std::cout << std::endl;

	local_ice.push_back(candidate);
}

void PeerConnection::createOffer(webrtc::MediaConstraintsInterface* constraints)
{
	std::cout << "createOffer" << std::endl;
	_peer_connection->CreateOffer(_create_session_observer.get(), constraints);
}

void PeerConnection::handleAnswer()
{
	//-- read answer and native ice candidates

	std::stringstream stream;
	std:: string line;
	std::ifstream file("e:/signaling/browser_answer");

	if (file.is_open()){
		while (std::getline(file, line)){
			stream << line;
		}
		file.close();
	}

	Json::Value json;
	Json::Reader reader;
	reader.parse(stream.str(), json, true);

	const std::string type = json["type"].asString();
	const std::string sdp = json["sdp"].asString();

	webrtc::SessionDescriptionInterface* answer = webrtc::CreateSessionDescription( type, sdp );
	
	if (!answer){
		return;
	}

	_peer_connection->SetRemoteDescription(_set_remote_session_observer.get(), answer);
}

void PeerConnection::onSetRemoteDescription() //called when successfully setup the remote session description
{
	//-- setup remote ice

	//-- read answer and native ice candidates

	std::stringstream stream;
	std::string line;
	std::ifstream file("e:/signaling/browser_ice", std::ios::binary);

	if (file.is_open()){
		while (std::getline(file, line)){
			stream << line;
		}
		file.close();
	}

	Json::Value json;
	Json::Reader reader;
	reader.parse(stream.str(), json, true);

	Json::Value::iterator it = json.begin();

	while (it != json.end()){

		Json::Value v = (*it);
		++it;

		const std::string sdp_mid	= v[kCandidateSdpMidName].asString();
		const int sdp_mline_index	= v[kCandidateSdpMlineIndexName].asInt();
		const std::string sdp		= v[kCandidateSdpName].asString();

		webrtc::IceCandidateInterface* ice = webrtc::CreateIceCandidate(sdp_mid, sdp_mline_index, sdp);

		if (!ice){
			continue;
		}

		_peer_connection->AddIceCandidate(ice);
	}
}

void PeerConnection::onSessionDescription(webrtc::SessionDescriptionInterface* offer)
{
	std::cout << "onSessionDescription" << std::endl;
	std::string str;
	offer->ToString(&str);

	std::cout << "SetLocalDescription" << std::endl;
	_peer_connection->SetLocalDescription(_set_local_session_observer.get(), offer);

	Json::StyledWriter writer;
	Json::Value json;
	json[kSessionDescriptionTypeName] = offer->type();
	
	std::string sdp;
	offer->ToString(&sdp);
	json[kSessionDescriptionSdpName] = sdp;

	{//--write current offer to local file
		std::ofstream file;
		file.open("e:/signaling/native_offer", std::ios::binary | std::ios::out | std::ios::trunc );
		file << writer.write(json);
		file.close();
	}
}

	}
}
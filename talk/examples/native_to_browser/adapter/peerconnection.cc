#include <iostream>
#include <fstream>
#include "talk/examples/native_to_browser/adapter/peerconnection.h"

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

	pc->_set_session_observer = new talk_base::RefCountedObject<n2b::observer::SetSessionDescription>();
	ASSERT(pc->_set_session_observer.get());

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
		
		std::vector<std::string>::const_iterator it = local_ice.begin();

		while (it != local_ice.end())
		{
			file << *it << std::endl;
			++it;
		}

		file.close();

		local_ice.clear();
	}
}

void PeerConnection::onIceCandidate(const webrtc::IceCandidateInterface* ice)
{
	if (!ice){
		return;
	}

	std::string str;
	ice->ToString(&str);

	local_ice.push_back(str);

	std::cout << "onIceCandidate" << std::endl;
	std::cout << str;
	std::cout << std::endl;
}

void PeerConnection::createOffer(webrtc::MediaConstraintsInterface* constraints)
{
	std::cout << "createOffer" << std::endl;
	_peer_connection->CreateOffer(_create_session_observer.get(), constraints);
}

void PeerConnection::onSessionDescription(webrtc::SessionDescriptionInterface* offer)
{
	std::cout << "onSessionDescription" << std::endl;
	std::string str;
	offer->ToString(&str);

	std::cout << "SetLocalDescription" << std::endl;
	_peer_connection->SetLocalDescription(_set_session_observer.get(), offer);

	{//--write current offer to local file
		std::ofstream file;
		file.open("e:/signaling/native_offer", std::ios::binary | std::ios::out | std::ios::trunc );
		file << str;
		file.close();
	}
}

	}
}
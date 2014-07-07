#include <iostream>
#include <fstream>
#include <sstream>

#include "talk/examples/native_to_browser/adapter/peerconnection.h"
#include "talk/base/json.h"

namespace n2b{ 
	namespace adapter {

		
PeerConnectionPtr PeerConnection::create( n2b::TransportPtr& transport, 
										  const webrtc::PeerConnectionInterface::RTCConfiguration cfg,
						      			  const webrtc::MediaConstraintsInterface* constraints)
{
	
	PeerConnectionPtr pc = new talk_base::RefCountedObject < PeerConnection > ;
	
	//-- setup transport
	pc->_transport = transport;
	ASSERT(pc->_transport.get());

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
	
	ASSERT(pc->_peer_connection->AddStream(pc->local_media_stream->_media_stream, constraints));

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

	_transport->onRemoteIceCandidate.connect(this, &PeerConnection::onRemoteIceCandidate);
	_transport->onRemoteSessionDescription.connect(this, &PeerConnection::onRemoteSessionDescription);

	return true;	
}

void PeerConnection::onIceStart()
{
	std::cout << "onIceStart" << std::endl;
}

void PeerConnection::OnIceComplete()
{
	_transport->sendIceCandidate(NULL);
}

void PeerConnection::onIceCandidate(const webrtc::IceCandidateInterface* ice)
{
	if (!ice){
		return;
	}

	_transport->sendIceCandidate(ice);
}

void PeerConnection::onRemoteIceCandidate(webrtc::IceCandidateInterface* candidate)
{
	ASSERT(_peer_connection);
	ASSERT(candidate);

	_peer_connection->AddIceCandidate(candidate);
}

void PeerConnection::createOffer(webrtc::MediaConstraintsInterface* constraints)
{
	_peer_connection->CreateOffer(_create_session_observer.get(), constraints);
}

void PeerConnection::createAnswer(webrtc::MediaConstraintsInterface* constraints)
{
	_peer_connection->CreateAnswer(_create_session_observer.get(), constraints);
}

void PeerConnection::onRemoteSessionDescription(webrtc::SessionDescriptionInterface* desc)
{
	ASSERT(_peer_connection);
	ASSERT(desc);	

	_peer_connection->SetRemoteDescription(_set_remote_session_observer.get(), desc);
}

void PeerConnection::onSessionDescription(webrtc::SessionDescriptionInterface* desc)
{
	ASSERT(desc);
	
	//-- maye send the description after successfully setup the local description 
	_peer_connection->SetLocalDescription(_set_local_session_observer.get(), desc);
	_transport->sendSessionDescription(desc);
}

	}
}
#include <iostream>
#include "talk/examples/native_to_browser/observer/peerconnection.h"


namespace n2b {
	namespace observer{

void PeerConnection::OnError()
{

}

void PeerConnection::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{

}


void PeerConnection::OnStateChange(StateType state_changed)
{

}

void PeerConnection::OnAddStream(webrtc::MediaStreamInterface* stream)
{

}

void PeerConnection::OnRemoveStream(webrtc::MediaStreamInterface* stream)
{
	 
}

void PeerConnection::OnDataChannel(webrtc::DataChannelInterface* data_channel)
{

}

void PeerConnection::OnRenegotiationNeeded()
{

}

void PeerConnection::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
{

}

void PeerConnection::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
	switch (new_state)
	{
	case webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringGathering:
		onIceStart.emit();
		break;
	case  webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringComplete:
		onIceComplete.emit();
		break;
	default:
		break;
	}

}

void PeerConnection::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
	std::cout << "OnIceCandidate" << std::endl;
	onIceCandidate.emit(candidate);
}

void PeerConnection::OnIceComplete()
{

}

	}
}


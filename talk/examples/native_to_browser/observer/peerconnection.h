#ifndef NATIVE_TO_BROWSER_OBSERVER_PEERCONNECTION_H
#define NATIVE_TO_BROWSER_OBSERVER_PEERCONNECTION_H 1

#include "talk/base/sigslot.h"
#include "talk/app/webrtc/peerconnectioninterface.h"

namespace n2b {
	namespace observer{

class PeerConnection : public webrtc::PeerConnectionObserver,
					   public talk_base::RefCountInterface
{
public:

	virtual ~PeerConnection(){};

	virtual void OnError();
	virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state);
	virtual void OnStateChange(StateType state_changed);
	virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
	virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
	virtual void OnDataChannel(webrtc::DataChannelInterface* data_channel);
	virtual void OnRenegotiationNeeded();
	virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state);
	virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state);
	virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
	virtual void OnIceComplete();

	sigslot::signal0<> onIceStart;
	sigslot::signal0<> onIceComplete;
	sigslot::signal1<const webrtc::IceCandidateInterface*> onIceCandidate;
	

}; //end class PeerConnection

	} //end namespace observer
} //end namespace n2b


#endif // NATIVE_TO_BROWSER_OBSERVER_PEERCONNECTION_H
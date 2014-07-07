#ifndef NATIVE_TO_BROWSER_TRANSPORT_H 
#define NATIVE_TO_BROWSER_TRANSPORT_H 1

#include "talk/app/webrtc/peerconnectioninterface.h"

namespace n2b{

class Transport;
typedef talk_base::scoped_refptr<Transport> TransportPtr;

class Transport : public talk_base::RefCountInterface
{
public:

	//-- send local session description to peer
	virtual void sendSessionDescription(const webrtc::SessionDescriptionInterface* desc) = 0;

	//-- send local ice candidate to peer
	virtual void sendIceCandidate(const webrtc::IceCandidateInterface* candidate) = 0;

	//-- event trigger for incomming remote session description
	//-- the transport implementation has to trigger this events
	sigslot::signal1<webrtc::SessionDescriptionInterface*> onRemoteSessionDescription;

	//-- event trigger for incomming remote ice candidate
	//-- the transport implementation has to trigger this events
	sigslot::signal1<webrtc::IceCandidateInterface*> onRemoteIceCandidate;

};//end class Transport

}//end namespace n2b

#endif //NATIVE_TO_BROWSER_TRANSPORT_H 

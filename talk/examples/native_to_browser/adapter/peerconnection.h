#ifndef NATIVE_TO_BROWSER_ADAPTER_PEERCONNECTION_H
#define NATIVE_TO_BROWSER_ADAPTER_PEERCONNECTION_H 1

#include <map>
#include "talk/base/scoped_ptr.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/examples/native_to_browser/observer/peerconnection.h"
#include "talk/examples/native_to_browser/observer/createsessiondescription.h"
#include "talk/examples/native_to_browser/observer/setsessiondescription.h"
#include "talk/examples/native_to_browser/mediastream.h"

namespace n2b{
	namespace adapter{

class PeerConnection;
typedef talk_base::scoped_refptr<PeerConnection> PeerConnectionPtr;

class PeerConnection : public talk_base::RefCountInterface,
					   public sigslot::has_slots<>
{
public:
	
	virtual ~PeerConnection();

	static PeerConnectionPtr create(const webrtc::PeerConnectionInterface::RTCConfiguration cfg,
		const webrtc::MediaConstraintsInterface* constraints);

	MediaStreamPtr local_media_stream;

	void createOffer(webrtc::MediaConstraintsInterface* constraints);

private:
	
	bool setupSignals();

	//-- observer::PeerConnection slots
	void onIceStart();
	void OnIceComplete();
	void onIceCandidate(const webrtc::IceCandidateInterface*);

	//-- observer::CreateSessionDescription ( offer )
	void onSessionDescription(webrtc::SessionDescriptionInterface*);
		
	talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _peer_connection_factory;
	talk_base::scoped_refptr<webrtc::PeerConnectionInterface> _peer_connection;

	talk_base::scoped_refptr<observer::PeerConnection> _peer_connection_observer;
	talk_base::scoped_refptr<observer::CreateSessionDescription> _create_session_observer;
	talk_base::scoped_refptr<observer::SetSessionDescription> _set_session_observer;	

	std::vector<std::string> local_ice;
};

	}
}



#endif //NATIVE_TO_BROWSER_ADAPTER_PEERCONNECTION_H
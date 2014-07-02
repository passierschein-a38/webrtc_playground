#ifndef NATIVE_TO_BROWSER_OBSERVER_CREATESESSIONDESCRIPTION_H
#define NATIVE_TO_BROWSER_OBSERVER_CREATESESSIONDESCRIPTION_H 1

#include "talk/base/sigslot.h"
#include "talk/app/webrtc/peerconnectioninterface.h"

namespace n2b {
	namespace observer{

class CreateSessionDescription : public webrtc::CreateSessionDescriptionObserver
{	
public:

	virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
	virtual void OnFailure(const std::string& error);

	sigslot::signal1<webrtc::SessionDescriptionInterface*> onSuccess;
	sigslot::signal1<std::string> onFailure;

}; //end class CreateSessionDescription

	} //end namespace observer
} //end namespace n2b



#endif // NATIVE_TO_BROWSER_OBSERVER_CREATESESSIONDESCRIPTION_H
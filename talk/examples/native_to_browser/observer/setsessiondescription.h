#ifndef NATIVE_TO_BROWSER_OBSERVER_SETSESSIONDESCRIPTION_H
#define NATIVE_TO_BROWSER_OBSERVER_SETSESSIONDESCRIPTION_H 1

#include "talk/base/sigslot.h"
#include "talk/app/webrtc/peerconnectioninterface.h"

namespace n2b {
	namespace observer{

class SetSessionDescription : public webrtc::SetSessionDescriptionObserver
{	
public:

	virtual void OnSuccess();
	virtual void OnFailure(const std::string& error);

	sigslot::signal0<> success;
	sigslot::signal1<std::string> failed;

}; //end class SetSessionDescription

	} //end namespace observer
} //end namespace n2b



#endif // NATIVE_TO_BROWSER_OBSERVER_SETSESSIONDESCRIPTION_H
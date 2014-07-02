#include "talk/examples/native_to_browser/observer/createsessiondescription.h"

namespace n2b {
	namespace observer{

	void CreateSessionDescription::OnSuccess(webrtc::SessionDescriptionInterface* desc){
		onSuccess.emit(desc);
	}

	void CreateSessionDescription::OnFailure(const std::string& error){
		onFailure.emit(error);
	}

		
	} //end namespace observer
} //end namespace n2b
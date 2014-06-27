
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/base/scoped_ptr.h"

int main(int argc, char** argv)
{
	talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
		peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();
			

	return 0;
}
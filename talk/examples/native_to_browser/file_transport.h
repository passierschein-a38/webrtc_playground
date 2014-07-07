#ifndef NATIVE_TO_BROWSER_FILE_TRANSPORT_H 
#define NATIVE_TO_BROWSER_FILE_TRANSPORT_H 1

#include <string>
#include <map>

#include "talk/base/sigslot.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/examples/native_to_browser/transport.h"


namespace n2b{

class FileTransport;
typedef talk_base::scoped_refptr<FileTransport> FileTransportPtr;

class FileTransport : public n2b::Transport
{
public:

	explicit FileTransport(const std::string& direcotry,
							const std::string& local_id,
							const std::string& remote_id);

	virtual ~FileTransport();

	//-- send local session description to peer
	virtual void sendSessionDescription(const webrtc::SessionDescriptionInterface* desc);

	//-- send local ice candidate to peer
	virtual void sendIceCandidate(const webrtc::IceCandidateInterface* candidate);

public:

	void readRemoteSessionDescription();
	void readRemoteIceCandidates();

private:

	std::vector<std::string> _local_ice;

	std::string _local_id;
	std::string _remote_id;

};//end class FileTransport

}//end namespace n2b

#endif //NATIVE_TO_BROWSER_FILE_TRANSPORT_H 

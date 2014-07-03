#ifndef NATIVE_TO_BROWSER_MEIDA_STREAM_H 
#define NATIVE_TO_BROWSER_MEIDA_STREAM_H 1

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"

namespace n2b{

class MediaStream;
typedef talk_base::scoped_refptr<MediaStream> MediaStreamPtr;

class MediaStream : public talk_base::RefCountInterface
{
public:

	explicit MediaStream( const std::string& id, const talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface>& factory);		
	virtual ~MediaStream();

	std::string id() const;

	bool addAudio( const std::string& id, const webrtc::MediaConstraintsInterface* constraints);
	void removeAudio(const std::string& id);

	bool addVideo(const std::string& id, const webrtc::MediaConstraintsInterface* constraints);
	void removeVideo(const std::string& id);

	talk_base::scoped_refptr<webrtc::MediaStreamInterface> _media_stream;

private:
	
	void removeAllTracks();

	std::string _id;
	talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> _factory;

};//end class MediaManager

}//end namespace n2b

#endif //NATIVE_TO_BROWSER_MEIDA_STREAM_H 

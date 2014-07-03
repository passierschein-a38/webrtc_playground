#include "talk/examples/native_to_browser/mediastream.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/app/webrtc/videosourceinterface.h"

namespace n2b{

	
MediaStream::MediaStream(const std::string& id, const talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface>& factory)
{
	ASSERT(factory.get());
	_factory = factory;
	_id = id;
	_media_stream = _factory->CreateLocalMediaStream("local_media");	
}

MediaStream::~MediaStream()
{	
	removeAllTracks();
}

void MediaStream::removeAllTracks()
{
	{//-- audio
		webrtc::AudioTrackVector tracks = _media_stream->GetAudioTracks();
		webrtc::AudioTrackVector::iterator it = tracks.begin();

		while (it != tracks.end()){			
			_media_stream->RemoveTrack(*it);
		}
	}

	{//-- video
		webrtc::VideoTrackVector tracks = _media_stream->GetVideoTracks();
		webrtc::VideoTrackVector::iterator it = tracks.begin();

		while (it != tracks.end()){
			_media_stream->RemoveTrack(*it);
		}
	}

}

bool MediaStream::addAudio( const std::string& id, const webrtc::MediaConstraintsInterface* constraints)
{
	ASSERT(_media_stream.get());
	
	talk_base::scoped_refptr<webrtc::AudioSourceInterface> audio_source = _factory->CreateAudioSource(constraints);
	ASSERT(audio_source.get());

	talk_base::scoped_refptr<webrtc::AudioTrackInterface> audio_track = _factory->CreateAudioTrack(id, audio_source);
	ASSERT(audio_track.get());
	
	return _media_stream->AddTrack(audio_track);
}

void MediaStream::removeAudio(const std::string& id)
{
	ASSERT(_media_stream.get());

	talk_base::scoped_refptr<webrtc::AudioTrackInterface> audio_track =	_media_stream->FindAudioTrack(id);

	if (audio_track.get()){
		_media_stream->RemoveTrack(audio_track);
	}

}

bool MediaStream::addVideo(const std::string& id, const webrtc::MediaConstraintsInterface* constraints)
{		
	ASSERT(_media_stream.get());

	talk_base::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
			cricket::DeviceManagerFactory::Create());

	if(!dev_manager->Init()){
			return false;
	}

	std::vector<cricket::Device> devs;
	if (!dev_manager->GetVideoCaptureDevices(&devs)) {
		return false;
	}
	
	std::vector<cricket::Device>::iterator dev_it = devs.begin();
	cricket::VideoCapturer* capturer = NULL;

	for (; dev_it != devs.end(); ++dev_it) {
		
		capturer = dev_manager->CreateVideoCapturer(*dev_it);
		
		if (capturer != NULL){
			break;
		}
	}

	if (!capturer){
		return false;
	}
	
	talk_base::scoped_refptr<webrtc::VideoSourceInterface> video_source = _factory->CreateVideoSource( capturer, constraints );
	ASSERT(video_source.get());

	talk_base::scoped_refptr<webrtc::VideoTrackInterface> video_track = _factory->CreateVideoTrack(id, video_source );
	ASSERT(video_track.get());

	return _media_stream->AddTrack(video_track);
}

void MediaStream::removeVideo(const std::string& id)
{
	ASSERT(_media_stream.get());

	talk_base::scoped_refptr<webrtc::VideoTrackInterface> video_track = _media_stream->FindVideoTrack(id);

	if (video_track.get()){
		_media_stream->RemoveTrack(video_track);
	}
}

std::string MediaStream::id() const
{
	return _id;
}

}//end namepsace
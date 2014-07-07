#include "talk/examples/native_to_browser/file_transport.h"

#include "talk/base/json.h"
#include <fstream>
#include <iostream>

namespace{
	const char kCandidateSdpMidName[] = "sdpMid";
	const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
	const char kCandidateSdpName[] = "candidate";
	const char kSessionDescriptionTypeName[] = "type";
	const char kSessionDescriptionSdpName[] = "sdp";
}

namespace n2b{

FileTransport::FileTransport(const std::string& direcotry,
							 const std::string& local_id,
							 const std::string& remote_id)
{
	_local_id = direcotry + local_id;
	_remote_id = direcotry + remote_id;
}

FileTransport::~FileTransport()
{}


void FileTransport::sendSessionDescription(const webrtc::SessionDescriptionInterface* desc)
{
	ASSERT(desc);
	
	Json::StyledWriter writer;
	Json::Value json;
	json[kSessionDescriptionTypeName] = desc->type();

	std::string sdp;
	desc->ToString(&sdp);
	json[kSessionDescriptionSdpName] = sdp;

	{//--write current offer to local file
		std::ofstream file;
		file.open( _local_id + "_desc", std::ios::binary | std::ios::out | std::ios::trunc);
		file << writer.write(json);
		file.close();
	}
}

void FileTransport::sendIceCandidate(const webrtc::IceCandidateInterface* candidate)
{

	if (!candidate){
		//-- end of ice candidates

		std::ofstream file;
		file.open( _local_id + "_ice", std::ios::binary | std::ios::out | std::ios::trunc);

		Json::StyledWriter writer;
		Json::Value json;

		std::vector<std::string>::const_iterator it = _local_ice.begin();

		while (it != _local_ice.end()){
			json.append(*it);
			++it;
		}

		file << writer.write(json);
		file.close();

		_local_ice.clear();

		return;
	}

	std::string sdp;
	if (!candidate->ToString(&sdp)){
		std::cout << "Failed to get sdp" << std::endl;
	}

	//Json::StyledWriter writer;
	Json::FastWriter writer;
	Json::Value json;

	json[kCandidateSdpMidName] = candidate->sdp_mid();
	json[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
	json[kCandidateSdpName] = "a=" + sdp;

	const std::string str = writer.write(json);

	std::cout << "onIceCandidate" << std::endl;
	std::cout << str;
	std::cout << std::endl;

	_local_ice.push_back(str);
}

void FileTransport::readRemoteSessionDescription()
{
	//-- read answer and native ice candidates
	std::stringstream stream;
	std::string line;
	std::ifstream file( _remote_id + "_desc");

	if (file.is_open()){
		while (std::getline(file, line)){
			stream << line;
		}
		file.close();
	}

	Json::Value json;
	Json::Reader reader;
	reader.parse(stream.str(), json, true);

	const std::string type = json["type"].asString();
	const std::string sdp = json["sdp"].asString();

	webrtc::SessionDescriptionInterface* desc = webrtc::CreateSessionDescription(type, sdp);

	if (!desc){
		return;
	}

	onRemoteSessionDescription.emit(desc);
}

void FileTransport::readRemoteIceCandidates()
{
	//-- setup remote ice


	//-- read answer and native ice candidates

	std::stringstream stream;
	std::string line;
	std::ifstream file( _remote_id + "_ice", std::ios::binary);

	if (file.is_open()){
		while (std::getline(file, line)){
			stream << line;
		}
		file.close();
	}

	Json::Value json;
	Json::Reader reader;
	reader.parse(stream.str(), json, true);

	Json::Value::iterator it = json.begin();

	while (it != json.end()){

		Json::Value v = (*it);
		++it;

		const std::string sdp_mid = v[kCandidateSdpMidName].asString();
		const int sdp_mline_index = v[kCandidateSdpMlineIndexName].asInt();
		const std::string sdp = v[kCandidateSdpName].asString();

		webrtc::IceCandidateInterface* ice = webrtc::CreateIceCandidate(sdp_mid, sdp_mline_index, sdp);

		if (!ice){
			continue;
		}

		onRemoteIceCandidate.emit(ice);
	}
}

}//end namespace n2b

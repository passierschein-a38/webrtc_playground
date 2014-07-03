#include "talk/examples/native_to_browser/constraints.h"

namespace n2b{

Constraints::Constraints(){

}

Constraints::~Constraints(){

}

const webrtc::MediaConstraintsInterface::Constraints& Constraints::GetMandatory() const{
	return _mandatory;
}

const webrtc::MediaConstraintsInterface::Constraints& Constraints::GetOptional() const{
	return _optional;
}

}

#ifndef NATIVE_TO_BROWSER_CONSTRAINTS_H 
#define NATIVE_TO_BROWSER_CONSTRAINTS_H 1


#include "talk/app/webrtc/mediaconstraintsinterface.h"

namespace n2b{

class Constraints : public webrtc::MediaConstraintsInterface
{
public:

	explicit Constraints();
	virtual ~Constraints();

	virtual const webrtc::MediaConstraintsInterface::Constraints& GetMandatory() const;
	virtual const webrtc::MediaConstraintsInterface::Constraints& GetOptional() const;

	template <class T>
	void AddMandatory(const std::string& key, const T& value) {
		_mandatory.push_back(webrtc::MediaConstraintsInterface::Constraint(key, talk_base::ToString<T>(value)));
	}

	template <class T>
	void AddOptional(const std::string& key, const T& value) {
		_optional.push_back(webrtc::MediaConstraintsInterface::Constraint(key, talk_base::ToString<T>(value)));
	}

	template <class T>
	void SetMandatory(const std::string& key, const T& value) {
		std::string value_str;
		if (_mandatory.FindFirst(key, &value_str)) {
			for (webrtc::MediaConstraintsInterface::Constraints::iterator iter = _mandatory.begin();
				iter != _mandatory.end(); ++iter) {
				if (iter->key == key) {
					_mandatory.erase(iter);
					break;
				}
			}
		}
		_mandatory.push_back(webrtc::MediaConstraintsInterface::Constraint(key, talk_base::ToString<T>(value)));
	}

	template <class T>
	void SetOptional(const std::string& key, const T& value) {
		std::string value_str;
		if (_optional.FindFirst(key, &value_str)) {
			for (webrtc::MediaConstraintsInterface::Constraints::iterator iter = _optional.begin();
				iter != _optional.end(); ++iter) {
				if (iter->key == key) {
					_optional.erase(iter);
					break;
				}
			}
		}
		_optional.push_back(webrtc::MediaConstraintsInterface::Constraint(key, talk_base::ToString<T>(value)));
	}

private:
		
	webrtc::MediaConstraintsInterface::Constraints _mandatory;
	webrtc::MediaConstraintsInterface::Constraints _optional;

};//end class Constraints

}//end namespace n2b

#endif //NATIVE_TO_BROWSER_CONSTRAINTS_H 

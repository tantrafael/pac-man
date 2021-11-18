#pragma once

class SdlWrapper {
public:
	SdlWrapper(int flags);
	~SdlWrapper();
private:
	SdlWrapper(const SdlWrapper&); //Unimplemented
};

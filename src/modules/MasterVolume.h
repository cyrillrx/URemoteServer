#pragma once

#include <iostream>

#include "platform_config.h"
# if defined(WINDOWS_PLATFORM)
#   include <endpointvolume.h>
# endif

class MasterVolume
{
public :
	static MasterVolume* getInstance();
	static void freeInstance();

	bool toggleMute();
	float define(const int& volumePoucentage);
	float turnUp();
	float turnDown();

private :
	static MasterVolume *s_masterVolume;

# if defined(WINDOWS_PLATFORM)
	IAudioEndpointVolume *m_endpointVolume;
	void loadVolumeController();
	void freeVolumeController();
# endif

	MasterVolume();
	~MasterVolume();

	bool isMute();
	void setMute(const bool& mute);

	float getVolume();
	void setVolume(const float& volume);
};

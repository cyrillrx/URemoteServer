#pragma once

#include <iostream>

#include "core/platform_config.h"
# if defined(WINDOWS_PLATFORM)
#   include <windows.h>
#   include <endpointvolume.h>
# endif

class master_volume
{
public :
	static master_volume* getInstance();
	static void freeInstance();

	bool toggleMute();
	float define(const int& volumePoucentage);
	float turnUp();
	float turnDown();

private :
	static master_volume *s_masterVolume;

# if defined(WINDOWS_PLATFORM)
	IAudioEndpointVolume *m_endpointVolume;
	void loadVolumeController();
	void freeVolumeController();
# endif

	master_volume();
	~master_volume();

	bool isMute();
	void setMute(const bool& mute);

	float getVolume();
	void setVolume(const float& volume);
};

#pragma once

#include <iostream>
#include <endpointvolume.h>

class MasterVolume
{
	
private :
	IAudioEndpointVolume *m_EndpointVolume;
	
	void LoadEndpointVolume();
	void FreeEndpointVolume();
	
	bool IsMute();
	void SetMute(bool _mute);

	float GetVolume();
	void SetVolume(float _newVolume);

public:
	MasterVolume(void);
	~MasterVolume(void);
	
	bool ToggleMute();
	float TurnUp();
	float TurnDown();
	
};


#pragma once

#include <iostream>
#include <endpointvolume.h>

class MasterVolume
{
	
private :
	static MasterVolume *s_MasterVolume; 
	IAudioEndpointVolume *m_EndpointVolume;
	
	MasterVolume(void);
	~MasterVolume(void);
	
	void LoadEndpointVolume();
	void FreeEndpointVolume();
	
	bool IsMute();
	void SetMute(bool _mute);

	float GetVolume();
	void SetVolume(float _newVolume);

public :
	static MasterVolume* GetInstance();
	static void FreeInstance();
	
	bool ToggleMute();
	float TurnUp();
	float TurnDown();
	
};
#pragma once

#include <iostream>
#include <endpointvolume.h>

class MasterVolume
{
public :
	static MasterVolume* GetInstance();
	static void FreeInstance();
	
	bool ToggleMute();
	float Define(int volumePoucentage);
	float TurnUp();
	float TurnDown();

private :
	static MasterVolume *s_MasterVolume; 
	IAudioEndpointVolume *m_EndpointVolume;
	
	MasterVolume(void);
	~MasterVolume(void);
	
	void LoadEndpointVolume();
	void FreeEndpointVolume();
	
	bool IsMute();
	void SetMute(bool mute);

	float GetVolume();
	void SetVolume(float volume);	
};
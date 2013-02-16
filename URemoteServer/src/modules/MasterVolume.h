#pragma once

#include <iostream>
#include <endpointvolume.h>

class MasterVolume
{
public :
	static MasterVolume* getInstance();
	static void freeInstance();
	
	bool toggleMute();
	float define(int volumePoucentage);
	float turnUp();
	float turnDown();

private :
	static MasterVolume *s_masterVolume; 
	IAudioEndpointVolume *m_endpointVolume;
	
	MasterVolume(void);
	~MasterVolume(void);
	
	void loadVolumeController();
	void freeVolumeController();
	
	bool isMute();
	void setMute(bool mute);

	float getVolume();
	void setVolume(float volume);	
};
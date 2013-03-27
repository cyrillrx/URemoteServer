#pragma once

#include <iostream>
#include <endpointvolume.h>

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
	IAudioEndpointVolume *m_endpointVolume;
	
	MasterVolume();
	~MasterVolume();
	
	void loadVolumeController();
	void freeVolumeController();
	
	bool isMute();
	void setMute(const bool& mute);

	float getVolume();
	void setVolume(const float& volume);	
};
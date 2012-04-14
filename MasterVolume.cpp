#include "MasterVolume.h"

#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>

#define VOLUME_MAX	1.0f
#define VOLUME_MIN	0.0f
#define VOLUME_STEP	0.05f

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Fonctions publics
//////////////////////////////////////////////////////////////////////////////

//! Cette fonction permet d'initialiser la librairie COM
MasterVolume::MasterVolume(void)
{
	// Initialisation de la librairie com
	if (CoInitialize(NULL) != S_OK) {
		cout << "CoInitialize failed" << endl;
	}
	LoadEndpointVolume();
}

MasterVolume::~MasterVolume(void)
{
	FreeEndpointVolume();
	CoUninitialize();
}

//! Change l'état de mute et renvoie le nouvel état
bool MasterVolume::ToggleMute()
{
	bool newMuteState = !IsMute();
	SetMute(newMuteState);

	return newMuteState;
}

//! Augmente le volume et renvoie la nouvelle valeur
float MasterVolume::TurnUp()
{
	float currentVolume = GetVolume();
	float newVolume = min(VOLUME_MAX, currentVolume + VOLUME_STEP);
	SetVolume(newVolume);

	return newVolume;
}

//! Baisse le volume et renvoie la nouvelle valeur
float MasterVolume::TurnDown()
{
	float currentVolume = GetVolume();
	float newVolume = max(VOLUME_MIN, currentVolume - VOLUME_STEP);
	SetVolume(newVolume);

	return newVolume;
}

//////////////////////////////////////////////////////////////////////////////
// Fonctions privées
//////////////////////////////////////////////////////////////////////////////

//! Initialisation de l'objet permettant le contrôle du volume
void MasterVolume::LoadEndpointVolume()
{
	m_EndpointVolume = NULL;

	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	IMMDevice *defaultDevice = NULL;

	// Récupération de la liste des périphériques audio
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	if (hr != S_OK) {
		cout << "CoCreateInstance failed with error: " << hr << endl;
		return;
	}

	// Récupération du périphérique audio par défaut
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	if (hr != S_OK) {
		cout << "GetDefaultAudioEndpoint failed with error: " << hr << endl;
		return;
	}

	// Libération de la liste des périphériques
	if (deviceEnumerator) {
		deviceEnumerator->Release();
		deviceEnumerator = NULL;
	}

	// Récupération de l'objet permettant le contrôle du volume
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&m_EndpointVolume);
	if (hr != S_OK) {
		cout << "defaultDevice->Activate failed with error: " << hr << endl;
		return;
	}

	if (defaultDevice) {
		defaultDevice->Release();
		defaultDevice = NULL; 
	}
}

//! Libération du pointeur de contrôle du volume
void MasterVolume::FreeEndpointVolume()
{
	if (m_EndpointVolume) {
		m_EndpointVolume->Release();
		m_EndpointVolume = NULL;
	}
}

//! Permet de savoir si mute est activé
bool MasterVolume::IsMute()
{
	BOOL isMute = false;
	HRESULT hr = m_EndpointVolume->GetMute(&isMute);
	if (hr != S_OK) {
		cout << "An error occured while getting muting state !" << endl;
	}
	return (isMute) ? true : false;
}

//! Definit l'état de mute
void MasterVolume::SetMute(bool _isMute)
{
	HRESULT hr = m_EndpointVolume->SetMute(_isMute, NULL);
	if (hr != S_OK) {
		cout << "An error occured while setting muting state !" << endl;
	}
}

// Récupère le volume actuel (valeur entre 0 et 100)
float MasterVolume::GetVolume()
{
	float _currentVolume = 0;

	//HRESULT hr = m_endpointVolume->GetMasterVolumeLevel(&currentVolume);
	//printf("Current volume in dB is: %f\n", currentVolume);

	HRESULT hr = m_EndpointVolume->GetMasterVolumeLevelScalar(&_currentVolume);
	if (hr != S_OK) {
		cout << "An error occured while getting volume !" << endl;
	}
	return _currentVolume;
}

// Définit le nouveau volume
void MasterVolume::SetVolume(float _newVolume)
{
	HRESULT hr = m_EndpointVolume->SetMasterVolumeLevelScalar(_newVolume, NULL);
	if (hr != S_OK) {
		cout << "An error occured while setting volume !" << endl;
	}
}
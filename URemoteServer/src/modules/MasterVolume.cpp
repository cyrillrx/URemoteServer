#include "MasterVolume.h"

#include <iostream>

#include "Utils.h"
#include <windows.h>
#include <mmdeviceapi.h>

const auto VOLUME_MAX(1.0f);
const auto VOLUME_MIN(0.0f);
const auto VOLUME_STEP(0.05f);

MasterVolume* MasterVolume::s_masterVolume = nullptr; 

MasterVolume* MasterVolume::getInstance()
{
	if (!s_masterVolume) {
		s_masterVolume = new MasterVolume();
	}
	return s_masterVolume;
}

void MasterVolume::freeInstance()
{
	delete(s_masterVolume);
	s_masterVolume = nullptr;
}

/**
* Toogle Mute state.
* @return The new state.
*/
bool MasterVolume::toggleMute()
{
	auto newMuteState = !isMute();
	setMute(newMuteState);

	return newMuteState;
}

/** 
* Define the volume as a pecentage.
* @param volume The volume as a percentage.
* @return The new value.
*/
float MasterVolume::define(const int& volumePoucentage)
{
	float volume = (float) volumePoucentage / 100.0f;
	setVolume(volume);

	return volume;
}

/** 
* Increase volume with the defined value VOLUME_STEP.
* @return The new value.
*/
float MasterVolume::turnUp()
{
	auto currentVolume = getVolume();
	float newVolume = min(VOLUME_MAX, currentVolume + VOLUME_STEP);
	setVolume(newVolume);

	return newVolume;
}

/** 
* Decrease volume with the defined value VOLUME_STEP.
* @return The new value.
*/
float MasterVolume::turnDown()
{
	auto currentVolume = getVolume();
	float newVolume = max(VOLUME_MIN, currentVolume - VOLUME_STEP);
	setVolume(newVolume);

	return newVolume;
}

/**
* Default constructor.
*/
MasterVolume::MasterVolume()
{
	// Initialize the COM library.
	if (CoInitialize(nullptr) != S_OK) {
		Utils::getLogger()->error("MasterVolume::MasterVolume(), CoInitialize failed");
	}
	loadVolumeController();
}

/**
* Default destructor.
*/
MasterVolume::~MasterVolume()
{
	freeVolumeController();
	// Free COM Library
	CoUninitialize();
}

/**
* Initialize the EndpointVolume (volume controller).
*/
void MasterVolume::loadVolumeController()
{
	m_endpointVolume = nullptr;

	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = nullptr;
	IMMDevice *defaultDevice = nullptr;

	// Get the list of audio devices
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	if (hr != S_OK) {
		Utils::getLogger()->error("MasterVolume::loadVolumeController(), CoCreateInstance failed with error: " + hr);
		return;
	}

	// Get the default audio device
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	if (hr != S_OK) {
		Utils::getLogger()->error("GetDefaultAudioEndpoint failed with error: " + hr);
		return;
	}

	// Free the device list
	if (deviceEnumerator) {
		deviceEnumerator->Release();
		deviceEnumerator = nullptr;
	}

	// Load EndpointVolume (volume controller)
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, (LPVOID *)&m_endpointVolume);
	if (hr != S_OK) {
		Utils::getLogger()->error("MasterVolume::loadVolumeController(), defaultDevice->Activate failed with error: " + hr);
		return;
	}

	// Release the default device
	if (defaultDevice) {
		defaultDevice->Release();
		defaultDevice = nullptr; 
	}
}

/**
* Free the volume controller.
*/
void MasterVolume::freeVolumeController()
{
	if (m_endpointVolume) {
		m_endpointVolume->Release();
		m_endpointVolume = nullptr;
	}
}

/**
* Get Mute state
*/
bool MasterVolume::isMute()
{
	BOOL isMute = false;
	auto hr = m_endpointVolume->GetMute(&isMute);
	if (hr != S_OK) {
		Utils::getLogger()->error("MasterVolume::isMute(), An error occured while getting muting state !");
	}
	return (isMute) ? true : false;
}

/**
* Set mute state
*/
void MasterVolume::setMute(const bool& isMute)
{
	auto hr = m_endpointVolume->SetMute(isMute, nullptr);
	if (hr != S_OK) {
		Utils::getLogger()->error("MasterVolume::setMute(), An error occured while setting muting state !");
	}
}

/**
* Get the volume (value : 0 to 100)
*/
float MasterVolume::getVolume()
{
	auto currentVolume(0.0f);

	auto hr = m_endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
	if (hr != S_OK) {
		Utils::getLogger()->error("MasterVolume::getVolume(), An error occured while getting volume !");
	}
	return currentVolume;
}

// Définit le nouveau volume
void MasterVolume::setVolume(const float& newVolume)
{
	auto hr = m_endpointVolume->SetMasterVolumeLevelScalar(newVolume, nullptr);
	if (hr != S_OK) {
		Utils::getLogger()->error("MasterVolume::setVolume(), An error occured while setting volume !");
	}
}

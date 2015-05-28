#include "master_volume.h"

#include <iostream>
#include <algorithm>
#include "logger/console_logger.h"

# if defined(WINDOWS_PLATFORM)
#   include <windows.h>
#   include <mmdeviceapi.h>
# endif // defined(WINDOWS_PLATFORM)

const auto VOLUME_MAX(1.0f);
const auto VOLUME_MIN(0.0f);
const auto VOLUME_STEP(0.05f);

auto logger = ConsoleLogger(DEBUG);

master_volume* master_volume::s_masterVolume = nullptr;

master_volume* master_volume::getInstance()
{
	if (!s_masterVolume) {
		s_masterVolume = new master_volume();
	}
	return s_masterVolume;
}

void master_volume::freeInstance()
{
	delete(s_masterVolume);
	s_masterVolume = nullptr;
}

/**
* Toogle Mute state.
* @return The new state.
*/
bool master_volume::toggleMute()
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
float master_volume::define(const int& volumePoucentage)
{
	float volume = (float) volumePoucentage / 100.0f;
	setVolume(volume);

	return volume;
}

/**
* Increase volume with the defined value VOLUME_STEP.
* @return The new value.
*/
float master_volume::turnUp()
{
	const auto currentVolume = getVolume();
	const float newVolume = std::min(VOLUME_MAX, currentVolume + VOLUME_STEP);
	setVolume(newVolume);

	return newVolume;
}

/**
* Decrease volume with the defined value VOLUME_STEP.
* @return The new value.
*/
float master_volume::turnDown()
{
	const auto currentVolume = getVolume();
	const float newVolume = std::max(VOLUME_MIN, currentVolume - VOLUME_STEP);
	setVolume(newVolume);

	return newVolume;
}

/**
* Default constructor.
*/
master_volume::master_volume()
{
# if defined(WINDOWS_PLATFORM)

	// Initialize the COM library.
	if (CoInitialize(nullptr) != S_OK) {
		logger.Error("master_volume::master_volume(), CoInitialize failed");
	}
	loadVolumeController();

# endif // defined(WINDOWS_PLATFORM)
}

/**
* Default destructor.
*/
master_volume::~master_volume()
{
# if defined(WINDOWS_PLATFORM)

	freeVolumeController();
	// Free COM Library
	CoUninitialize();

# endif // defined(WINDOWS_PLATFORM)
}

# if defined(WINDOWS_PLATFORM)

/**
* Initialize the EndpointVolume (volume controller).
*/
void master_volume::loadVolumeController()
{
	m_endpointVolume = nullptr;

	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = nullptr;
	IMMDevice *defaultDevice = nullptr;

	// Get the list of audio devices
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	if (hr != S_OK) {
		logger.Error("master_volume::loadVolumeController(), CoCreateInstance failed with error: " + hr);
		return;
	}

	// Get the default audio device
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	if (hr != S_OK) {
		logger.Error("GetDefaultAudioEndpoint failed with error: " + hr);
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
		logger.Error("master_volume::loadVolumeController(), defaultDevice->Activate failed with error: " + hr);
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
void master_volume::freeVolumeController()
{
	if (m_endpointVolume) {
		m_endpointVolume->Release();
		m_endpointVolume = nullptr;
	}
}
# endif // defined(WINDOWS_PLATFORM)


/**
* Get Mute state
*/
bool master_volume::isMute()
{
# if defined(WINDOWS_PLATFORM)
	BOOL isMute = false;
	auto hr = m_endpointVolume->GetMute(&isMute);
	if (hr != S_OK) {
		logger.Error("master_volume::isMute(), An error occured while getting muting state !");
	}
	return (isMute) ? true : false;
# else
	// TODO: Implement master_volume::isMute() on linux
	return false;
# endif
}

/**
* Set mute state
*/
void master_volume::setMute(const bool& isMute)
{

# if defined(WINDOWS_PLATFORM)
	auto hr = m_endpointVolume->SetMute(isMute, nullptr);
	if (hr != S_OK) {
		logger.Error("master_volume::setMute(), An error occured while setting muting state !");
	}
# else
	// TODO: Implement master_volume::setMute() on linux
# endif

}

/**
* Get the volume (value : 0 to 100)
*/
float master_volume::getVolume()
{

# if defined(WINDOWS_PLATFORM)
	auto currentVolume(0.0f);

	auto hr = m_endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
	if (hr != S_OK) {
		logger.Error("master_volume::getVolume(), An error occured while getting volume !");
	}
	return currentVolume;
# else
	// TODO: Implement master_volume::getVolume() on linux
	return 0.0f;
# endif

}

// D�finit le nouveau volume
void master_volume::setVolume(const float& newVolume)
{

# if defined(WINDOWS_PLATFORM)
	auto hr = m_endpointVolume->SetMasterVolumeLevelScalar(newVolume, nullptr);
	if (hr != S_OK) {
		logger.Error("master_volume::setVolume(), An error occured while setting volume !");
	}
# else
	// TODO: Implement master_volume::setVolume() on linux
# endif
}

#pragma once

#include <string>
#include <map>

namespace voice_recognition
{
	
	// TODO: move in a more global scope (can be use for all listeners) => move in executor ?
	class action
	{
	private:
	};

	//class recognizer
	class listener
	{
	public:
	private:
		std::map<std::string, action> commands;
	};
	
	//void listen_to(const std::string& key_word, const std::string& sentence, bool (*action)());
	// TODO: Create a VoiceRecognitionEntity
	// TODO: Create an VocalCommand that could be associate to an action (callback)
	// TODO: externalize UremoteServer VoiceListener stuffs here
}


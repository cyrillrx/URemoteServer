#include "Exchange.h"

#include <iostream>

#include "google\protobuf\io\zero_copy_stream_impl_lite.h"
#include "modules\FileManager.h"
#include "modules\FileManager.h"
#include "modules\MonUtils.h"
#include "modules\MasterVolume.h"
#include "modules\Keyboard.h"
#include "modules\App.h"
#include "Translator.h"
#include "TextKey.h"
#include "Utils.h"

using namespace network;

SerializedExchange Exchange::handleMessage(AI* ai, SerializedExchange serializedRequest)
{
	auto* request = getRequest(serializedRequest);

	const auto reqType	= request->type();
	const auto reqCode	= request->code();
	const auto reqExtraCode		= request->extracode();
	const auto securityToken	= request->securitytoken();
	const auto strParam	= request->stringparam();
	const auto intParam	= request->intparam();
	
	Utils::getLogger()->info("Server::HandleMessage() Received message : ");
	Utils::getLogger()->info(" - Type		<" + Request_Type_Name(reqType)	+ ">");
	Utils::getLogger()->info(" - Code		<" + Request_Code_Name(reqCode)	+ ">");
	Utils::getLogger()->info(" - ExtraCode	<" + Request_Code_Name(reqExtraCode) + ">");
	Utils::getLogger()->info(" - Token	    <" + securityToken				+ ">");
	Utils::getLogger()->info(" - str param	<" + strParam					+ ">");
	Utils::getLogger()->info(" - int param	<" + std::to_string(intParam)	+ ">");

	auto* reply = new Response();
	reply->set_requesttype(reqType);
	reply->set_requestcode(reqCode);

	if (request->securitytoken() != "1234") {
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		const auto message = Translator::getString(TextKey::XC_UNKNOWN_SECURITY_TOKEN);
		reply->set_message(message);

		ai->say(message);

	} else {
	
		switch (reqType) {
	
		case Request_Type_SIMPLE:
			classicCommand(ai, reply, reqCode);
			break;

		case Request_Type_EXPLORER:
			FileManager::HandleMessage(reply, reqCode, strParam);
			break;
		
		case Request_Type_KEYBOARD:
			Keyboard::HandleMessage(reply, request);
			break;

		case Request_Type_AI:
			aICommand(ai, reply, reqCode);
			break;
		
		case Request_Type_VOLUME:
			volumeCommand(reply, reqCode, intParam);
			break;

		case Request_Type_APP:
			appCommand(reply, reqCode);
			break;

		default:
			reply->set_returncode(Response_ReturnCode_RC_ERROR);
			reply->set_message("Unknown type received : " + reqType);
			break;
		}
	}

	int bufSize = 0;
	char* buf = nullptr;
	auto serializedReply = getSerializeResponse(reply);

	delete(reply);
	reply = nullptr;

	delete(request);
	request = nullptr;

	return serializedReply;
}

Request* Exchange::getRequest(SerializedExchange exchange)
{
	auto* request = new Request();
	// Read varint delimited protobuf object in the buffer
	google::protobuf::io::ArrayInputStream arrayInputStream(exchange.buffer, exchange.bufferSize);
	google::protobuf::io::CodedInputStream codedInputStream(&arrayInputStream);

	google::protobuf::uint32 size;
	codedInputStream.ReadVarint32(&size);
	google::protobuf::io::CodedInputStream::Limit msgLimit = codedInputStream.PushLimit(size);

	request->ParseFromCodedStream(&codedInputStream);
	codedInputStream.PopLimit(msgLimit);

	return request;
}

SerializedExchange Exchange::getSerializeResponse(Response* response)
{
	// Build a buffer that can hold message and room for a 32bit delimiter
	int bufSize	= response->ByteSize() + 4;
	char* buf	= new char [bufSize];

	// Write varint delimiter to the buffer
	google::protobuf::io::ArrayOutputStream arrayOutputStream(buf, bufSize);
	google::protobuf::io::CodedOutputStream codedOutputStream(&arrayOutputStream);
	codedOutputStream.WriteVarint32(response->ByteSize());

	// Write response to the buffer
	response->SerializeToCodedStream(&codedOutputStream);
	
	SerializedExchange exchange;
	exchange.bufferSize	= bufSize;
	exchange.buffer		= buf;
	
	return exchange;
}

/** Handle general commands. */
void Exchange::classicCommand(AI* ai, Response* reply, Request_Code code)
{
	switch (code) {

	case Request_Code_HELLO:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		ai->welcome();
		break;
		
	case Request_Code_TEST:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message(Translator::getString(TextKey::XC_TEST));
		break;

	case Request_Code_KILL_SERVER:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Server killed");
		ai->stopConnection();
		break;

	case Request_Code_SHUTDOWN:
		shutdownPC(ai, reply, 10);
		break;

	case Request_Code_LOCK:
		LockWorkStation();
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("PC locked");
		break;

	case Request_Code_SWITCH_WINDOW:
		if (MonUtils::SwitchWindow()) {
			reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
			reply->set_message("Switch window");
		} else {
			reply->set_returncode(Response_ReturnCode_RC_ERROR);
			reply->set_message("Switch window error");
		}
		break;
			
	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown Simple Command received : " + code);
		break;
	}
}

/** Handle volume commands. */
void Exchange::volumeCommand(Response* reply, Request_Code code, int intParam)
{
	float fVolumeLvl;
	bool isMute;
	
	char buffer[50];
	std::string message;
	int volumePoucentage;

	switch (code) {

	case Request_Code_DEFINE:
		fVolumeLvl = MasterVolume::getInstance()->define(intParam);
		MasterVolume::freeInstance();

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply->set_intvalue(volumePoucentage);
		
		sprintf_s(buffer,  "Volume up to %d%%", volumePoucentage);
		message = buffer;
		break;

	case Request_Code_UP:
		fVolumeLvl = MasterVolume::getInstance()->turnUp();
		MasterVolume::freeInstance();

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply->set_intvalue(volumePoucentage);
		
		sprintf_s(buffer,  "Volume up to %d%%", volumePoucentage);
		message = buffer;
		break;
			
	case Request_Code_DOWN:
		fVolumeLvl = MasterVolume::getInstance()->turnDown();
		MasterVolume::freeInstance();

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply->set_intvalue(volumePoucentage);
		
		sprintf_s(buffer,  "Volume down to %d%%", volumePoucentage);
		message = buffer;
		break;
			
	case Request_Code_MUTE:
		isMute = MasterVolume::getInstance()->toggleMute();
		MasterVolume::freeInstance();
		
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);	
		reply->set_intvalue(isMute);
		message = (isMute) ? "Volume is off." : "Volume is now on.";
		break;
	
	default:
		message = "Unknown Volume command !";
		Utils::getLogger()->error(message);
		break;
	}

	reply->set_message(message);
}

/** Handle AI commands */
void Exchange::aICommand(AI* ai, Response* reply, Request_Code code)
{
	bool isMute;
	char* message;

	switch (code) {

	case Request_Code_MUTE:
		isMute = ai->toggleMute();
			
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = (isMute) ? "AI volume is off." : "AI volume is now on.";
		break;
	
	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown AI command !";
		Utils::getLogger()->error(message);
		break;
	}
	
	reply->set_message(message);
}

/** Handle application commands. */
void Exchange::appCommand(Response* reply, Request_Code code)
{
	std::string message;

	switch (code) {

	// Ouvrir Gom player
	case Request_Code_GOM_PLAYER_RUN:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Show();
		App::FreeGomPlayer();
		break;
		
	// Fermer Gom player
	case Request_Code_GOM_PLAYER_KILL:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Close();
		App::FreeGomPlayer();
		break;

	// Stretch Gom player
	case Request_Code_GOM_PLAYER_STRETCH:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Strech();
		App::FreeGomPlayer();
		break;
			
	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown app command !";
		Utils::getLogger()->error(message);
		break;
	}
	
	reply->set_message(message);
}

/** Send a command to shutdown the computer. */
void Exchange::shutdownPC(AI* ai, Response* reply, int delay)
{
	ai->stopConnection();
	
	const auto message = Translator::getString(TextKey::XC_PC_SHUTDOWN, delay);
	
	const std::string command = "Shutdown.exe -s -t " + std::to_string(delay) + " -c \"" + message + "\"";
	system(command.c_str());
	
	reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
	reply->set_message(message);

	ai->say(message);
}
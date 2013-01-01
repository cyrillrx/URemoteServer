#include "Exchange.h"

#include <iostream>
#include <sstream>
#include <ostream>

#include "google\protobuf\io\zero_copy_stream_impl_lite.h"
#include "modules\FileManager.h"
#include "modules\FileManager.h"
#include "modules\MonUtils.h"
#include "modules\MasterVolume.h"
#include "modules\Keyboard.h"
#include "modules\App.h"
#include "Translator.h"

using namespace std;
using namespace network;

SerializedExchange Exchange::HandleMessage(AI* ai, SerializedExchange serializedRequest, bool &continueToListen)
{
	auto* request = GetRequest(serializedRequest);

	const auto reqType	= request->type();
	const auto reqCode	= request->code();
	const auto reqExtraCode	= request->extracode();
	const auto securityToken	= request->securitytoken();
	const auto strParam	= request->stringparam();
	const auto intParam	= request->intparam();
	
	cout << "Server::HandleMessage() Received message : " << endl;
	cout << " - Type		<" << Request_Type_Name(reqType)	<< ">" << endl;
	cout << " - Code		<" << Request_Code_Name(reqCode)	<< ">" << endl;
	cout << " - ExtraCode	<" << Request_Code_Name(reqExtraCode)	<< ">" << endl;
	cout << " - Token	    <" << securityToken					<< ">" << endl;
	cout << " - str param	<" << strParam						<< ">" << endl;
	cout << " - int param	<" << intParam						<< ">" << endl;

	auto* reply = new Response();
	reply->set_requesttype(reqType);
	reply->set_requestcode(reqCode);

	if (request->securitytoken() != "1234") {
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
	//TODO: define Translator::LANG_FR via IA
	auto translator = Translator::getInstance();
	//cout << translator->GetString("ai_initiated", Translator::LANG_FR) << endl;
		reply->set_message(translator->GetString("unknown_security_token", Translator::LANG_FR));

	} else {
	
		switch (reqType) {
	
		case Request_Type_SIMPLE:
			ClassicCommand(ai, reply, reqCode);
			break;

		case Request_Type_EXPLORER:
			FileManager::HandleMessage(reply, reqCode, strParam);
			break;
		
		case Request_Type_KEYBOARD:
			Keyboard::HandleMessage(reply, request);
			break;

		case Request_Type_AI:
			AICommand(ai, reply, reqCode);
			break;
		
		case Request_Type_VOLUME:
			VolumeCommand(reply, reqCode, intParam);
			break;

		case Request_Type_APP:
			AppCommand(reply, reqCode);
			break;

		default:
			reply->set_returncode(Response_ReturnCode_RC_ERROR);
			reply->set_message("Unknown type received : " + reqType);
			break;
		}
	}

	int bufSize = 0;
	char* buf = nullptr;
	auto serializedReply = GetSerializeResponse(reply);

	delete(reply);
	reply = nullptr;

	delete(request);
	request = nullptr;

	return serializedReply;
}

Request* Exchange::GetRequest(SerializedExchange exchange)
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

SerializedExchange Exchange::GetSerializeResponse(Response* response)
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
void Exchange::ClassicCommand(AI* ai, Response* reply, Request_Code code)
{
	switch (code) {

	case Request_Code_HELLO:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		ai->Welcome();
		break;
		
	case Request_Code_TEST:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		//reply->set_message("The code has been tested");
		//reply->set_message("No way. Go Fuck yourself !");
		reply->set_message("Intruder detected. Get out, or I'll kick your little punk ass !");
		break;

	case Request_Code_KILL_SERVER:
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		reply->set_message("Server killed");
		ai->StopConnection();
		break;

	case Request_Code_SHUTDOWN:
		ShutdownPC(ai, reply, 10);
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
void Exchange::VolumeCommand(Response* reply, Request_Code code, int intParam)
{
	float fVolumeLvl;
	bool isMute;
	
	char buffer[50];
	string message;
	int volumePoucentage;

	switch (code) {

	case Request_Code_DEFINE:
		fVolumeLvl = MasterVolume::GetInstance()->Define(intParam);
		MasterVolume::FreeInstance();

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply->set_intvalue(volumePoucentage);
		
		sprintf_s(buffer,  "Volume up to %d%%", volumePoucentage);
		message = buffer;
		break;

	case Request_Code_UP:
		fVolumeLvl = MasterVolume::GetInstance()->TurnUp();
		MasterVolume::FreeInstance();

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply->set_intvalue(volumePoucentage);
		
		sprintf_s(buffer,  "Volume up to %d%%", volumePoucentage);
		message = buffer;
		break;
			
	case Request_Code_DOWN:
		fVolumeLvl = MasterVolume::GetInstance()->TurnDown();
		MasterVolume::FreeInstance();

		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = (int) (fVolumeLvl * 100);
		reply->set_intvalue(volumePoucentage);
		
		sprintf_s(buffer,  "Volume down to %d%%", volumePoucentage);
		message = buffer;
		break;
			
	case Request_Code_MUTE:
		isMute = MasterVolume::GetInstance()->ToggleMute();
		MasterVolume::FreeInstance();
		
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);	
		reply->set_intvalue(isMute);
		message = (isMute) ? "Volume is off." : "Volume is now on.";
		break;
	
	default:
		message = "Unknown Volume command !";
		cerr << message << endl;
		break;
	}

	reply->set_message(message);
}

/** Handle AI commands */
void Exchange::AICommand(AI* ai, Response* reply, Request_Code code)
{
	bool isMute;
	char* message;

	switch (code) {

	case Request_Code_MUTE:
		isMute = ai->ToggleMute();
			
		reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = (isMute) ? "AI volume is off." : "AI volume is now on.";
		break;
	
	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown AI command !";
		cerr << message << endl;
		break;
	}
	
	reply->set_message(message);
}

/** Handle application commands. */
void Exchange::AppCommand(Response* reply, Request_Code code)
{
	string message;

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
		cerr << message << endl;
		break;
	}
	
	reply->set_message(message);
}

/** Send a command to shutdown the computer. */
void Exchange::ShutdownPC(AI* ai, Response* reply, int delay)
{
	ai->StopConnection();
	
	char command[100];
	sprintf_s(command, "Shutdown.exe -s -t %d -c \"L'ordinateur va s'éteindre dans %d secondes\"", delay, delay);
	system(command);

	reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
	char message[50];
	sprintf_s(message, "PC will shutdown in %d seconds", delay);
	reply->set_message(message);

	ai->Say(message);
}
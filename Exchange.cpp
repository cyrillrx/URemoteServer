#include "Exchange.h"

#include <iostream>
#include <sstream>
#include <ostream>

#include "include\google\protobuf\io\zero_copy_stream_impl_lite.h"
#include "AI.h"
#include "FileManager.h"
#include "MonUtils.h"
#include "MasterVolume.h"
#include "Keyboard.h"
#include "App.h"

// Gestion des applications
const string Exchange::APP_GOM_PLAYER	= "app_gom_player";
const string Exchange::KILL_GOM_PLAYER	= "kill_gom_player";
const string Exchange::GOM_PLAYER_STRETCH	= "gom_player_stretch";

SerializedExchange Exchange::HandleMessage(SerializedExchange _serializedExchange, bool &_continueToListen)
//void* Exchange::HandleMessage(void* _data, bool &_continueToListen)
{
	Request* request = GetRequest(_serializedExchange);

	Request_Type reqType	= request->type();
	Request_Code reqCode	= request->code();
	string strParam	= request->stringparam();
	int intParam	= request->intparam();
	
	cout << "Server::HandleMessage() Received message : " << endl;
	cout << " - Type	<" << Request_Type_Name(reqType)	<< ">" << endl;
	cout << " - Code	<" << Request_Code_Name(reqCode)	<< ">" << endl;
	cout << " - String param <"	<< strParam					<< ">" << endl;
	cout << " - int param <"	<< intParam					<< ">" << endl;

	Response *reply = new Response();
	switch (reqType) {
	
	case Request_Type_SIMPLE:
		ClassicCommand(reply, reqCode);
		break;

	case Request_Type_EXPLORER:
		FileManager::HandleMessage(reply, reqCode, strParam);
		break;
		
	case Request_Type_KEYBOARD:
		Keyboard::HandleMessage(reply, reqCode, strParam);
		break;

	case Request_Type_AI:
		AICommand(reply, reqCode);
		break;
		
	case Request_Type_VOLUME:
		VolumeCommand(reply, reqCode);
		break;

	// Commande des applications
	case Request_Type_APP:
		AppCommand(reply, reqCode);
		break;

	default:
		reply->set_returncode(Response_ReturnCode_RC_ERROR);
		reply->set_message("Unknown type received : " + reqType);
		break;
	}
	
	if (reply->has_message()) {
		AI::GetInstance()->Say(reply->message());
	}

	int bufSize = 0;
	char* buf = NULL;
	SerializedExchange serializedExchange = GetSerializeResponse(reply);

	delete(reply);
	reply = NULL;

	delete(request);
	request = NULL;

	return serializedExchange;
}

Request* Exchange::GetRequest(SerializedExchange _exchange)
{
	Request* request = new Request();
	// Read varint delimited protobuf object in the buffer
	google::protobuf::io::ArrayInputStream arrayInputStream(_exchange.buffer, _exchange.bufferSize);
	google::protobuf::io::CodedInputStream codedInputStream(&arrayInputStream);

	google::protobuf::uint32 size;
	codedInputStream.ReadVarint32(&size);
	google::protobuf::io::CodedInputStream::Limit msgLimit = codedInputStream.PushLimit(size);

	request->ParseFromCodedStream(&codedInputStream);
	codedInputStream.PopLimit(msgLimit);

	return request;
}

SerializedExchange Exchange::GetSerializeResponse(Response* _response)
{
	// Build a buffer that can hold message and room for a 32bit delimiter
	int bufSize	= _response->ByteSize() + 4;
	char* buf	= new char [bufSize];

	// Write varint delimiter to the buffer
	google::protobuf::io::ArrayOutputStream arrayOutputStream(buf, bufSize);
	google::protobuf::io::CodedOutputStream codedOutputStream(&arrayOutputStream);
	codedOutputStream.WriteVarint32(_response->ByteSize());

	// Write response to the buffer
	_response->SerializeToCodedStream(&codedOutputStream);
	
	SerializedExchange exchange;
	exchange.bufferSize	= bufSize;
	exchange.buffer		= buf;
	
	return exchange;
}

//! Traitement d'une commande général
void Exchange::ClassicCommand(Response* _reply, Request_Code _code)
{
	switch (_code) {

	case Request_Code_HELLO:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Hello back.");
		AI::GetInstance()->Welcome();
		break;
		
	case Request_Code_TEST:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		//_reply->set_message("The code has been tested");
		//_reply->set_message("No way. Go Fuck yourself !");
		_reply->set_message("Intruder detected. Get out, or I'll kick your little punk ass !");
		break;

	case Request_Code_KILL_SERVER:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		_reply->set_message("Server killed");
		AI::GetInstance()->StopConnection();
		break;

	case Request_Code_SHUTDOWN:
		ShutdownPC(_reply, 10);
		break;

	case Request_Code_SWITCH_WINDOW:
		if (MonUtils::SwitchWindow()) {
			_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
			_reply->set_message("Switch window");
		} else {
			_reply->set_returncode(Response_ReturnCode_RC_ERROR);
			_reply->set_message("Switch window error");
		}
		break;
			
	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		_reply->set_message("Unknown Simple Command received : " + _code);
		break;
	}
}

//! Traitement d'un commande de volume
void Exchange::VolumeCommand(Response* _reply, Request_Code _code)
{
	float fVolumeLvl;
	bool isMute;
	
	char buffer[50];
	string message;
	int volumePoucentage;

	switch (_code) {

	case Request_Code_UP:
		fVolumeLvl = MasterVolume::GetInstance()->TurnUp();
		MasterVolume::FreeInstance();

		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = fVolumeLvl * 100;
		_reply->set_intvalue(volumePoucentage);
		
		sprintf(buffer,  "Volume up to %d%%", volumePoucentage);
		message = buffer;
		break;
			
	case Request_Code_DOWN:
		fVolumeLvl = MasterVolume::GetInstance()->TurnDown();
		MasterVolume::FreeInstance();

		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		volumePoucentage = fVolumeLvl * 100;
		_reply->set_intvalue(volumePoucentage);
		
		sprintf(buffer,  "Volume down to %d%%", volumePoucentage);
		message = buffer;
		break;
			
	case Request_Code_MUTE:
		isMute = MasterVolume::GetInstance()->ToggleMute();
		MasterVolume::FreeInstance();
		
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);	
		_reply->set_intvalue(isMute);
		message = (isMute) ? "Volume is off." : "Volume is now on.";
		break;
	
	default:
		message = "Unknown Volume command !";
		cerr << message << endl;
		break;
	}

	_reply->set_message(message);
}

/** Handle AI commands */
void Exchange::AICommand(Response* _reply, Request_Code _code)
{
	bool isMute;
	char* message;

	switch (_code) {

	case Request_Code_MUTE:
		isMute = AI::GetInstance()->ToggleMute();
			
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = (isMute) ? "AI volume is off." : "AI volume is now on.";
		break;
	
	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown AI command !";
		cerr << message << endl;
		break;
	}
	
	_reply->set_message(message);
}

/** Handle application commands. */
void Exchange::AppCommand(Response* _reply, Request_Code _code)
{
	string message;

	switch (_code) {

	// Ouvrir Gom player
	case Request_Code_GOM_PLAYER_RUN:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Show();
		App::FreeGomPlayer();
		break;
		
	// Fermer Gom player
	case Request_Code_GOM_PLAYER_KILL:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Close();
		App::FreeGomPlayer();
		break;

	// Stretch Gom player
	case Request_Code_GOM_PLAYER_STRETCH:
		_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
		message = App::GetGomPlayer()->Strech();
		App::FreeGomPlayer();
		break;
			
	default:
		_reply->set_returncode(Response_ReturnCode_RC_ERROR);
		message = "Unknown app command !";
		cerr << message << endl;
		break;
	}
	
	_reply->set_message(message);
}

/** Send a command to shutdown the computer. */
void Exchange::ShutdownPC(Response* _reply, int _delay)
{
	AI::GetInstance()->StopConnection();
	//ShellExecute(NULL, L"shutdown", NULL, L"-s -t 10", NULL, SW_SHOWMAXIMIZED);
	
	char* command = "";
	sprintf(command, "Shutdown.exe -s -t %d -c \"L'ordinateur va s'éteindre dans %d secondes\"", _delay, _delay);
	system(command);

	_reply->set_returncode(Response_ReturnCode_RC_SUCCESS);
	char* message = "";
	sprintf(message, "PC will shutdown in %d seconds", _delay);
	_reply->set_message(message);

	AI::GetInstance()->Say(message);
}

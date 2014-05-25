#ifndef Mud_Sockets_h
#define Mud_Sockets_h

using namespace std;
using namespace v8;

static unordered_map<string, Persistent<Object> > ActiveSockets;
static Persistent<Object> Broadcast;

Handle<Value> SetBroadcast(const Arguments& args)
{
	HandleScope handleScope;
	
	if (args.Length() < 1){
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return handleScope.Close(Undefined());
    }
    if (!args[0]->IsObject()) {
    	ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    	return handleScope.Close(Undefined());
	}

	Broadcast = Persistent<Object>::New(args[0]->ToObject());
	
	return handleScope.Close(Null());
}

Handle<Value> AddSocket(const Arguments& args)
{
	HandleScope handleScope;
	
	if (args.Length() < 1){
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return handleScope.Close(Undefined());
    }
    if (!args[0]->IsObject()) {
    	ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    	return handleScope.Close(Undefined());
	}
	
	Handle<Object> socket = args[0]->ToObject();
	Handle<String> sockuid = socket->Get(String::New("uid"))->ToString();
	ActiveSockets[string(*String::Utf8Value(sockuid))] = Persistent<Object>::New(socket);
	
	return handleScope.Close(Null());
}

#endif
#include "sockets.h"

#include <string>
#include <unordered_map>

using namespace std;
using namespace v8;

static Persistent<Object> BroadcastObj;
static Persistent<Object> WriteObj;

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

	BroadcastObj = Persistent<Object>::New(args[0]->ToObject());

	return handleScope.Close(Null());
}

void Broadcast(string sockuid, string msg)
{
	Local<Context> context = Context::GetCurrent();
	Handle<Object> global = context->Global();

	Handle<Value> broadcastArgv[] = { String::New(sockuid.c_str()), String::New(msg.c_str()) };
	BroadcastObj->CallAsFunction(global, 2, broadcastArgv);
}

Handle<Value> SetWrite(const Arguments& args)
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

	WriteObj = Persistent<Object>::New(args[0]->ToObject());

	return handleScope.Close(Null());
}

void Write(string sockuid, string msg)
{
	Local<Context> context = Context::GetCurrent();
	Handle<Object> global = context->Global();

	Handle<Value> argv[] = { String::New(sockuid.c_str()), String::New(msg.c_str()) };
	WriteObj->CallAsFunction(global, 2, argv);
}
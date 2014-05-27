#include "scrypt.h"

static Persistent<Object> Scrypt;

Handle<Value> SetScrypt(const Arguments& args)
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

	Scrypt = Persistent<Object>::New(args[0]->ToObject());
	
	Handle<Object> hash = Scrypt->Get(String::New("hash"))->ToObject();
	Handle<Object> hashConfig = hash->Get(String::New("config"))->ToObject();
	hashConfig->Set(String::New("keyEncoding"), String::New("utf8"));
	hashConfig->Set(String::New("outputEncoding"), String::New("hex"));
	
	Handle<Object> verify = Scrypt->Get(String::New("verify"))->ToObject();
	Handle<Object> verifyConfig = verify->Get(String::New("config"))->ToObject();
	verifyConfig->Set(String::New("hashEncoding"), String::New("hex"));
	verifyConfig->Set(String::New("keyEncoding"), String::New("utf8"));
	
	return handleScope.Close(Null());
}

string HashPassword(string key)
{
	Local<Context> context = Context::GetCurrent();
	Handle<Object> global = context->Global();
		
	Handle<Object> hasher = Scrypt->Get(String::New("hash"))->ToObject();
	Handle<Object> params = Scrypt->Get(String::New("params"))->ToObject();
	Handle<Value> paramsArgv[] = { Number::New(0.1) };
	Handle<Value> scryptParams = params->CallAsFunction(global, 1, paramsArgv);

	Handle<Value> hashArgv[] = { String::New(key.c_str()), scryptParams };
	Handle<Value> hash = hasher->CallAsFunction(global, 2, hashArgv);
	string result = string(*String::Utf8Value(hash->ToString()));
	
	return result;;
}

bool VerifyPassword(string hash, string key)
{
	Local<Context> context = Context::GetCurrent();
	Handle<Object> global = context->Global();

	Handle<Object> verifyFunc = Scrypt->Get(String::New("verify"))->ToObject();
	Handle<Value> verifyArgv[] = { String::New(hash.c_str()), String::New(key.c_str()) };
	Handle<Value> result = verifyFunc->CallAsFunction(global, 2, verifyArgv);

	return result->ToBoolean()->Value();
}
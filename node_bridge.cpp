#include <node.h>
#include <string.h>
#include <math.h>

extern "C" {
int generateEID(char *ikString, int scaler, int beacon_time_seconds, unsigned char *eid);
void printHex(unsigned char *eid, int offset);
}

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;

void generateEID(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	if (args.Length() < 3) {
		// Throw an Error that is passed back to JavaScript
		isolate->ThrowException(
				Exception::TypeError(
						String::NewFromUtf8(isolate,
								"Wrong number of arguments")));
		return;
	}

	if (!args[0]->IsString() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		isolate->ThrowException(
				Exception::TypeError(
						String::NewFromUtf8(isolate, "Wrong arguments")));
		return;
	}
	v8::String::Utf8Value ikArg(args[0]->ToString());
	std::string ikString = std::string(*ikArg);
	char *ik = (char*) ikString.c_str();
	int scaler = args[1]->NumberValue();
	int beacon_time_seconds = args[2]->NumberValue();

	unsigned char eid[128];
	generateEID(ik, scaler, beacon_time_seconds, eid);
	printHex(eid, 8);
	return;
}

void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "generateEID", generateEID);
}

NODE_MODULE(addon, init)

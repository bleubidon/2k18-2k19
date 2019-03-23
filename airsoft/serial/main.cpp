#include <node.h>
#include "SerialPort.h"

namespace serial
{
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;


SerialPort arduino;

#define MOUT 0
#define MCMD 1
std::vector<std::string> out[2] = {};

std::string outbuf;


v8::Local<v8::String> str(Isolate *iso, const char *str)
{
	return String::NewFromUtf8(iso, str, NewStringType::kNormal).ToLocalChecked();
}

void connect(const FunctionCallbackInfo<Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	if (args.Length() != 1)
	{
		isolate->ThrowException(Exception::TypeError(str(isolate, "connect: Provide 1 arg")));
		return;
	}

	char port[32];
	char *arg = *String::Utf8Value(args[0]);
	sprintf(port, "\\\\.\\%s", arg);

	if (!arduino.connect(port, CBR_9600))
	{
		char err[256];
		sprintf(err, "Cannot establish connection on port '%s'", port);
		isolate->ThrowException(Exception::Error(str(isolate, err)));
	}
}

void disconnect(const FunctionCallbackInfo<Value> &args)
{
	Isolate *isolate = args.GetIsolate();

	outbuf.clear();
	out[MOUT].clear();
	out[MCMD].clear();

	arduino.disconnect();
}

void flush(int mode)
{
	char buf[256];
	int n;
	while ((n = arduino.read(buf, 256)))
		outbuf += std::string(buf, n);

	n = 0;
	while (n < outbuf.size())
	{
		if ((mode == MCMD && outbuf[n] == 0x4) ||
			(mode == MOUT && outbuf[n] == '\n'))
		{
			out[mode].push_back(std::string(outbuf, 0, n));
			outbuf.erase(0, n + 1);
			n = 0;
			mode = MOUT;
		}
		else
			n++;
	}
}

void write(const FunctionCallbackInfo<Value> &args)
{
	Isolate *isolate = args.GetIsolate();
	if (args.Length() != 1)
		isolate->ThrowException(Exception::TypeError(str(isolate, "write: Provide 1 arg")));
	else
	{
		flush(MOUT);

		char *buf = *String::Utf8Value(args[0]);

		if (!arduino.write(buf, strlen(buf)))
			isolate->ThrowException(Exception::Error(str(isolate, "Write failed")));
	}
}

void get_output(const FunctionCallbackInfo<Value> &args)
{
	Isolate *isolate = args.GetIsolate();

	int mode = MOUT;
	flush(mode);
	if (out[mode].size())
	{
		args.GetReturnValue().Set(str(isolate, out[mode][0].c_str()));
		out[mode].erase(out[mode].begin());
	}
}

void get_cmd(const FunctionCallbackInfo<Value> &args)
{
	Isolate *isolate = args.GetIsolate();

	int mode = MCMD;
	flush(mode);
	if (out[mode].size())
	{
		args.GetReturnValue().Set(str(isolate, out[mode][0].c_str()));
		out[mode].erase(out[mode].begin());
	}
}

void Initialize(Local<Object> exports)
{
	NODE_SET_METHOD(exports, "connect", connect);
	NODE_SET_METHOD(exports, "disconnect", disconnect);

	NODE_SET_METHOD(exports, "write", write);

	NODE_SET_METHOD(exports, "read_output", get_output);
	NODE_SET_METHOD(exports, "read_cmd", get_cmd);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

} // namespace serial

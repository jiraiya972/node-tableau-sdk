#include "TableauRow.h"
#include "TableauTableDefinition.h"
#include "TableauException.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <TableauHyperExtract/TableauHyperExtract_cpp.h>
#else
#include "TableauHyperExtract_cpp.h"
#endif

namespace NodeTde {

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::External;
using v8::Number;
using v8::Boolean;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::NewStringType;
using std::wstring;
using std::string;
using nativeRow = Tableau::Row;
using nativeTableDefinition = Tableau::TableDefinition;

Persistent<Function> Row::constructor;

Row::Row(nativeTableDefinition* tableDef) {
  nativeRow_ = new nativeRow(*tableDef);
}

Row::~Row() {
  nativeRow_->Close();
}

void Row::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Row", NewStringType::kNormal).ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setNull", SetNull);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setInteger", SetInteger);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setLongInteger", SetLongInteger);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setDouble", SetDouble);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setBoolean", SetBoolean);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setString", SetString);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setCharString", SetCharString);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setDate", SetDate);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setDateTime", SetDateTime);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setDuration", SetDuration);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setSpatial", SetSpatial);

  constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked());
  exports->Set(context, String::NewFromUtf8(
      isolate, "Row", NewStringType::kNormal).ToLocalChecked(),
               tpl->GetFunction(context).ToLocalChecked()).FromJust();
}

nativeRow* Row::GetRow() {
  return nativeRow_;
}

void Row::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  // Invoked as constructor: `new Row(...)`
  if (args.IsConstructCall()) {
    TableDefinition* nodeTableDef = ObjectWrap::Unwrap<TableDefinition>(args[0]->ToObject(context).ToLocalChecked());
    nativeTableDefinition* tableDef = nodeTableDef->GetTableDefinition().get();

    Row* obj = new Row(tableDef);
    obj->Wrap(args.This());

    args.GetReturnValue().Set(args.This());
  }
  // Invoked as plain function `Row(...)`, turn into construct call.
  else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(context, argc, argv).ToLocalChecked());
  }
}

void Row::Close(const FunctionCallbackInfo<Value>& args) {
  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->Close();
}

void Row::SetNull(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetNull(columnNumber);
}

void Row::SetInteger(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  int32_t integerValue(args[1]->Int32Value(context).ToChecked());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetInteger(columnNumber, integerValue);
}

void Row::SetLongInteger(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  int64_t integerValue(args[1]->IntegerValue(context).ToChecked());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetLongInteger(columnNumber, integerValue);
}

void Row::SetDouble(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  double doubleValue(args[1]->NumberValue(context).ToChecked());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetDouble(columnNumber, doubleValue);
}

void Row::SetBoolean(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  bool boolValue(args[1]->BooleanValue(isolate));

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetBoolean(columnNumber, boolValue);
}

void Row::SetString(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  String::Utf8Value v8String(isolate, args[1]->ToString(context).ToLocalChecked());
  string stringValue = string(*v8String);
  wstring wStringValue(stringValue.length(), L' ');
  std::copy(stringValue.begin(), stringValue.end(), wStringValue.begin());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetString(columnNumber, wStringValue);
}

void Row::SetCharString(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  String::Utf8Value v8String(isolate, args[1]->ToString(context).ToLocalChecked());
  string stringValue = string(*v8String);

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetCharString(columnNumber, stringValue);
}

void Row::SetDate(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  int year(args[1]->IntegerValue(context).ToChecked());
  int month(args[2]->IntegerValue(context).ToChecked());
  int day(args[3]->IntegerValue(context).ToChecked());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  try {
    obj->nativeRow_->SetDate(columnNumber, year, month, day);
  }
  catch (const Tableau::TableauException& e) {
    THROW_TABLEAU_EXCEPTION(e);
  }
}

void Row::SetDateTime(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  int year(args[1]->IntegerValue(context).ToChecked());
  int month(args[2]->IntegerValue(context).ToChecked());
  int day(args[3]->IntegerValue(context).ToChecked());
  int hour(args[4]->IntegerValue(context).ToChecked());
  int min(args[5]->IntegerValue(context).ToChecked());
  int sec(args[6]->IntegerValue(context).ToChecked());
  int frac(args[7]->IntegerValue(context).ToChecked());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  try {
    obj->nativeRow_->SetDateTime(columnNumber, year, month, day, hour, min, sec, frac);
  }
  catch (const Tableau::TableauException& e) {
    THROW_TABLEAU_EXCEPTION(e);
  }
}

void Row::SetDuration(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  int day(args[1]->IntegerValue(context).ToChecked());
  int hour(args[2]->IntegerValue(context).ToChecked());
  int min(args[3]->IntegerValue(context).ToChecked());
  int sec(args[4]->IntegerValue(context).ToChecked());
  int frac(args[5]->IntegerValue(context).ToChecked());

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  obj->nativeRow_->SetDuration(columnNumber, day, hour, min, sec, frac);
}

void Row::SetSpatial(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  int columnNumber(args[0]->IntegerValue(context).ToChecked());
  String::Utf8Value v8String(isolate, args[1]->ToString(context).ToLocalChecked());
  string stringValue = string(*v8String);

  Row* obj = ObjectWrap::Unwrap<Row>(args.Holder());
  try {
    obj->nativeRow_->SetSpatial(columnNumber, stringValue);
  }
  catch (const Tableau::TableauException& e) {
    THROW_TABLEAU_EXCEPTION(e);
  }
}

}

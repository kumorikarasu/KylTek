#include "Main.h"
#include "Functions.h"
int toInt(string _line){
	istringstream buffer(_line);
	int value;
	buffer >> value;
	return value;
}
std::wstring toStr(int _value){
    std::ostringstream<TCHAR> outStream;

    outStream << _value;
	return outStream.str();
}
std::wstring toStr(long long _value){
    std::ostringstream<TCHAR> outStream;

    outStream << _value;
	return outStream.str();
}
std::wstring toStr(float _value){
    std::ostringstream<TCHAR> outStream;

    outStream << _value;
	return outStream.str();
}
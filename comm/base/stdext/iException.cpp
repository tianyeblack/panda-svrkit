#include <sstream>
#include <string.h>

#include "iException.h"

using namespace std;

namespace stdext
{

	Exception :: Exception(const int iErrno, const string sFunc , const string sErrMsg, const string sFile , const int iLine )
	{
		_iErrNo = iErrno;
		_sErrMsg = sErrMsg;
		_sFile = sFile;
		_sFunc = sFunc;
		_iLine = iLine;
	}

	Exception :: Exception(const string sErrMsg, const string sFunc , const string sFile , const int iLine )
	{
		_sErrMsg = sErrMsg;
		_sFile = sFile;
		_sFunc = sFunc;
		_iLine = iLine;
		_iErrNo = 0;
	}

	Exception :: Exception(const int iErrno, const string sFunc , const string sFile , const int iLine )
	{
		_sErrMsg = "";
		_iErrNo = iErrno;
		_sFile = sFile;
		_sFunc = sFunc;
		_iLine = iLine;
	}


	Exception::~Exception(){}

	const string Exception::what()
	{		
		char buf[1024] = {0};
		if( _sErrMsg.empty() && _iErrNo == 0 )
			return string("No enough error info");
			
		else if( _iErrNo < 0 ) 
			return _sErrMsg;
		
		else if( _sErrMsg.empty() && _iErrNo >= 0 ) 
			return string( strerror_r(_iErrNo, buf , 1024));
		
		else  
			return string( strerror_r(_iErrNo, buf , 1024))+" -> "+_sErrMsg;
		
	}

	const string Exception :: GetErrMsg()
	{
		return what();
	}

	const string Exception :: GetMsg()
	{	
		stringstream oss;
		oss<<_sFile<<":"<<_sFunc<<":"<<_iLine<<what();

		return oss.str();
	}

	const int Exception :: GetErrNo()
	{
		return _iErrNo;
	}
}

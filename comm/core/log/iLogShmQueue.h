#pragma once

#include <uinstd.h>

#include "iShm.h"

using namespace unistdext;
using namespace stdext;

#define LogShmQueue_MainVer 0x01U
#define LogShmQueue_SubVer 0x01U
#define LogShmQueue_HeadMagicNum 0x1234U
#define LogShmQueue_FlagOK 0
#define logShmQueue_STX 0x02U
#define logShmQueue_ETX 0x03U

#define LogShmQueue_ErrNo( n ) (ELogShmQueueBaseNo+n)

#define ERR_LogShmQueue_ShmOrHeadNULL LogShmQueue_ErrNo(-1)

namespace Comm
{
	/**
	* @principle : mutil-thread or mutil-process push, single thread pop
	*			
	*/
    class LogShmQueue
    {
	public:

	#pragma pack(1)
		typedef struct tagLogShmQueueHead
		{
			unsigned short hMainVer;
			unsigned short hSubVer;
			unsigned int iMagicNum;
			volatile int iQueueSize;
			volatile int iHeadPos;
			volatile int iTailPos;
			unsigned int iFlag;
			
			char sReserved[512-sizeof(unsigned short)*2 -sizeof(int)*5];
			char sData[1];
			
		}LogShmQueueHead_t;
	#pragma pack()
		
    	public:
		LogShmQueue();

		~LogShmQueue();

		void Init();

		int Push(const char* pcBuf, const int iBufLen );

		int Pop( char* pcBuf );


	private:
		void Reset();
		
	private:
		void WriteOwnLog(const char *sFmt, ...) __attribute__( (__format__( __printf__,1, 2)));

	private:
		Shm* _shm;
		LogShmQueueHead_t* _ptHeader;
    };
}
#include "iLogger.h"

#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#ifdef HAVE_SYS_SYSCALL_H
#include <sys/syscall.h>
#endif

#include "core/concurrent/iThread.h"

namespace Comm
{
	Logger*	Logger::mspInstance = NULL;

	Logger* Logger::getInstance ()
	{
		if (mspInstance == NULL)
			mspInstance = new Logger ();

		return mspInstance;
	}

	void Logger::finalize ()
	{
		if (mspInstance)
			delete mspInstance;

		mspInstance = NULL;
	}

#ifdef USE_THREADS
	Logger::Logger (const e_debug &debug, const std::string &log_file, FILE *pFP) : Lockable(),
#else
	Logger::Logger (const e_debug &debug, const std::string &log_file, FILE *pFP):
#endif
		 m_debug_level	(debug), m_log_file (log_file), mpFP (pFP)
	{

	}

	Logger::Logger (const Logger& rLogger)
		: m_debug_level	(NO_DEBUG), m_log_file (""), mpFP (NULL)
	{
	  throw std::exception ();
	}

	Logger& Logger::operator=(const Logger& rLogger)
	{
	  throw std::exception ();
	}

	Logger::~Logger ()
	{
		closeLogFile ();
	}

	void Logger::setDebugLevel (e_debug debug)
	{
		m_debug_level = (debug <= MAX_LEVEL ? debug:MAX_LEVEL);
	}

	void Logger::setLogFile (const std::string &file)
	{
		m_log_file = file;

		openLogFile ();
	}

	void Logger::setLogPath( const std::string &path )
	{
		m_log_path = path;
	}
	 
	std::string& Logger::getLogFile()
	{
		return m_log_file;
	}

	void Logger::openLogFile ()
	{
		std::string log = m_log_path+"/"+m_log_file;
		mpFP = fopen (log.c_str (), "a+");
		
		if( mpFP == NULL )
		{
			fprintf(stderr, "open log file %s failed, errmsg %s\n", log.c_str(), strerror(errno) );
		}
	}

	void Logger::closeLogFile ()
	{
		if (mpFP)
			fclose (mpFP);

		mpFP = NULL;
	}

	static pthread_key_t g_logSessionKey;
	static pthread_once_t g_logSessionKeyOnce = PTHREAD_ONCE_INIT;

	static void makeThreadLogSessionKey()
	{
		(void)pthread_key_create(&g_logSessionKey, NULL);
	}
		
	void Logger::setSessionId(const std::string &id)
	{
		(void)pthread_once(&g_logSessionKeyOnce, makeThreadLogSessionKey);
		
		LogSession* logSession = (LogSession*)pthread_getspecific(g_logSessionKey);

		if( logSession == NULL)
		{
			logSession = new LogSession;
			logSession->threadId = (unsigned int)pthread_self();
			snprintf(logSession->sessionId, sizeof(logSession->sessionId), "%s", id.c_str());

			if( 0!= pthread_setspecific(g_logSessionKey, (void*)logSession))
			{
				delete logSession;
				logSession = NULL;
			}
		}
		else
		{
			logSession->threadId = (unsigned int)pthread_self();
			snprintf(logSession->sessionId, sizeof(logSession->sessionId), "%s", id.c_str());
		}
	}

	std::string Logger::getSessionId()
	{
		LogSession* logSession = (LogSession*)pthread_getspecific(g_logSessionKey);
		
		if( logSession == NULL )
		{
			return std::string("");
		}
		else if( logSession->threadId == (unsigned int)pthread_self() )
		{
			return std::string(logSession->sessionId);
		}

		return std::string("");
	}

	void Logger::setMoudleName(const std::string &module_name)
	{
	    m_module_name = module_name;
	}


	void Logger::makeLog(char* buffer, e_debug debug, const char* format, va_list ap )
	{
		vsnprintf (buffer, Logger::MSG_BUFF_SIZE, format, ap);

		char log_date[Logger::DATE_BUFF_SIZE];
		time_t str_time;
		struct tm str_tm;
		
		time (&str_time);
		localtime_r (&str_time, &str_tm);
		strftime (log_date, Logger::DATE_BUFF_SIZE, "%d-%m-%YT%H:%M:%S", &str_tm);

#ifdef HAVE_SYS_SYSCALL_H
			pid_t tid = syscall (__NR_gettid);
#else
			pid_t tid = syscall (Logger::GETTID_SYSCALL_ID);
#endif

		if (mpFP)
		{
#ifdef USE_THREADS
			lock ();
			fprintf	(mpFP, "<%d>%s %s <%d,%d> %s: %s\n", 
				debug, m_module_name.c_str(), log_date, getpid(), tid, getSessionId().c_str(), buffer);
			unlock ();
#else
			fprintf	(mpFP, "<%d>%s %s <%d> %s: %s\n", 
				debug, m_module_name.c_str(), log_date, tid, getSessionId().c_str(), buffer);
#endif
			fflush	(mpFP);
		}
		else
		{
#ifdef USE_THREADS
			lock ();
			fprintf	(stderr, "<%d>%s %s <%d,%d> %s: %s\n", 
				debug, m_module_name.c_str(), log_date, getpid(), tid, getSessionId().c_str(), buffer);
			unlock ();
#else
			fprintf	(stderr, "<%d>%s %s <%d> %s: %s\n", 
				debug, m_module_name.c_str(), log_date, tid, getSessionId().c_str(), buffer);
#endif
			fflush	(stderr);
		}

	}

	void Logger::print (e_debug debug, const char *msg, va_list ap)
	{
		if (m_debug_level >= debug)
		{
#ifdef USE_THREADS
			int oldstate = Thread::disableCancel ();
#endif

			char buffer[Logger::MSG_BUFF_SIZE];
			makeLog(buffer, debug, msg, ap);

#ifdef USE_THREADS
			Thread::setCancelState (oldstate);
#endif
		}
	}

	void Logger::print (e_debug debug, const char *msg, ...)
	{
		if (m_debug_level >= debug)
		{
#ifdef USE_THREADS
			int oldstate = Thread::disableCancel ();
#endif

			char buffer[Logger::MSG_BUFF_SIZE];
			va_list ap;
			va_start (ap, msg);

			makeLog(buffer, debug, msg, ap);
			
			va_end (ap);

#ifdef USE_THREADS
			Thread::setCancelState (oldstate);
#endif
		}
	}
}

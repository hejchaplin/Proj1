#pragma once
#include <filesystem>
#include <chrono>
#include <thread>
#include <mutex>

#include "FileUtility.h"
#include "InfoCodes.h"

#include "Array.h"
#include "String.h"

#define CH_INLINE inline

enum class FileStatus
{
	Created,
	Modified,
	Erased
};

class Observer
{
public:
	using FileCallback = void( * )( FileStatus, const CString & );
	using FileAndTimes = CArray<std::pair<CString, uint32_t>>;

	Observer( uint32_t watchInterval )
		: m_Delay( watchInterval ), m_RunThreadSpecific( true ), m_Elapsed( 0.f )
	{

	}

	InfoCode submitFiles( CArray<CString> & filepaths );
	InfoCode startTickSpecified( FileCallback func, float delta );
	InfoCode startThreadSpecified( FileCallback func );
	std::thread createThreadObserver( FileCallback func );
	void setThreadObserverRunning( bool status );

private:
	FileAndTimes	m_Files_Times;
	uint32_t		m_Delay;
	bool			m_RunThreadSpecific;
	float			m_Elapsed;
	std::mutex		m_Lock;

	void start( FileCallback func );
};
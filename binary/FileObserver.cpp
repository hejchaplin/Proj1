#include "FileObserver.h"

InfoCode Observer::submitFiles( CArray<CString> & filepaths )
{
	FileAndTimes temp;

	//get times of all files passed as args
	for ( int i = 0; i < filepaths.length(); ++i )
	{
		struct stat st;
		int32_t error = stat( filepaths[ i ].raw(), &st );
		if ( error != 0 )
		{
			return InfoCode::Path_NoExist;
		}
		auto time = st.st_mtime;
		temp.push( { filepaths[ i ], time } );
	}
	//if there are no errors, we can fill the array
	m_Files_Times = temp;
	return InfoCode::Success;
}

InfoCode Observer::startThreadSpecified( FileCallback func )
{
	m_Lock.lock();
	if ( m_Files_Times.empty() )
	{
		m_Lock.unlock();
		return InfoCode::Zero_Data;
	}

	while ( m_RunThreadSpecific )
	{
		std::this_thread::sleep_for( std::chrono::milliseconds( m_Delay ) );
		start( func );
	}

	m_Lock.unlock();
	return InfoCode::Success;
}

std::thread Observer::createThreadObserver( FileCallback func )
{
	return std::thread( &Observer::startThreadSpecified, this, func );
}

void Observer::setThreadObserverRunning( bool status )
{
	m_RunThreadSpecific = status;
}

InfoCode Observer::startTickSpecified( FileCallback func, float delta )
{
	if ( m_Files_Times.empty() )
	{
		return InfoCode::Zero_Data;
	}

	m_Elapsed += delta;

	if ( m_Elapsed < ( m_Delay / 1000 ) )
	{
		return InfoCode::Success;
	}
	else
	{
		start( func );
		m_Elapsed = 0;
	}

	return InfoCode::Success;
}

void Observer::start( FileCallback func )
{
	for ( int i = m_Files_Times.length() - 1; i >= 0; --i )
	{
		CString filepath = m_Files_Times[ i ].first;
		uint32_t time = m_Files_Times[ i ].second;

		if ( !exists( ( char * )filepath.raw() ) )
		{
			func( FileStatus::Erased, filepath );
			m_Files_Times.remove( i );
		}
		else
		{
			struct stat st;
			int32_t error = stat( filepath.raw(), &st );
			if ( error != 0 )
			{
				continue;
			}
			uint32_t currentFileLastModified = st.st_mtime;
			if ( currentFileLastModified != time )
			{
				func( FileStatus::Modified, filepath );
				m_Files_Times[ i ].second = currentFileLastModified;
			}
		}
	}
}
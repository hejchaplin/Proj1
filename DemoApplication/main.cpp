#include <iostream>
#include <iterator>
//#include "../binary/ResourceManager.h"
#include "../binary/FileObserver.h"
#include "../binary/Packer.h"
#include <ctime>
class Timer
{
	using TimePoint = std::chrono::time_point< std::chrono::steady_clock >;

public:
	void start()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	uint32_t stop() const
	{
		return ( uint32_t )std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::high_resolution_clock::now() - m_Start ).count();
	}

private:
	TimePoint m_Start;
};

void demo( FileStatus status, const CString & file )
{
	switch ( status )
	{
	case FileStatus::Modified:	std::cout << "File modified : " << file.raw() << '\n'; break;
	case FileStatus::Erased:	std::cout << "File erased : " << file.raw() << '\n'; break;
	}
}

int main()
{
	if ( 0 )
	{

#define DEMO_ON_THREAD 1

		Observer obs( 5000 );
		CArray<CString> files;
		files.push( "C:/Users/megadodd/source/GitHub/Project1/Assets/Pictures/Kappa.png" );
		files.push( "C:/Users/megadodd/source/GitHub/Project1/Assets/Text/readme.txt" );
		InfoCode submit_error = obs.submitFiles( files );

#if DEMO_ON_THREAD == 0
		if ( !CH_RESOURCE_FAILED( submit_error ) )
		{
			Timer timer;
			float delta = 0.f;
			float fps = 1000.f / 60.f;
			for ( ;; )
			{
				timer.start();
				obs.startTickSpecified( demo, delta );
				float elapsed = timer.stop();
				if ( elapsed < fps )
				{
					Sleep( fps - elapsed );
					elapsed = fps;
				}
				delta = elapsed / 1000;
			}
		}
#else
		if ( !CH_RESOURCE_FAILED( submit_error ) )
		{
			std::thread worker = obs.createThreadObserver( demo );
			for ( ;; )
			{
				std::cout << "program running...\n";
				std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
			}
		}

#endif
	}

	BinaryPackage package;
	std::string shader = "C:/Users/megadodd/source/GitHub/Project1/Assets/UPackages/unpacked_test/1.shader";
	std::string texture = "C:/Users/megadodd/source/GitHub/Project1/Assets/UPackages/unpacked_test/1.texture";
	std::string outdir = "C:/Users/megadodd/source/GitHub/Project1/Assets/UPackages/DemoPacked";
	if ( 0 )
	{
		auto info = package.submit( ResourceType::Shader, shader );
		info = package.submit( ResourceType::Texture, texture );
		if ( CH_RESOURCE_FAILED( info ) )
		{
			int errorCode = CH_ERRORCODE( info );
			return 0;
		}
		info = package.pack( outdir, 1 );
		if ( CH_RESOURCE_FAILED( info ) )
		{
			int errorCode = CH_ERRORCODE( info );
			return 0;
		}
	}
	if ( 1 )
	{
		std::string file = "C:/Users/megadodd/source/GitHub/Project1/Assets/UPackages/DemoPacked.chpkg";
		auto info = package.unpack( file, "C:/Users/megadodd/source/GitHub/Project1/Assets/UPackages/unpacked_test" );
		if ( CH_RESOURCE_FAILED( info ) )
		{
			int errorCode = CH_ERRORCODE( info );
		}
	}

	return 0;
}
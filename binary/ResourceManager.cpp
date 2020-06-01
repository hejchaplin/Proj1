#include "ResourceManager.h"
#include "FileUtility.h"
#include <sstream>
#include <fstream>



bool hasEnding( const std::string & fullString, const std::string & ending )
{
	if ( fullString.length() >= ending.length() )
	{
		return ( 0 == fullString.compare( fullString.length() - ending.length(), ending.length(), ending ) );
	}
	else
	{
		return false;
	}
}

std::string BinaryPackage::appendPackageExtention( const std::string & filename ) const
{
	std::stringstream ss;
	ss << filename + EXTENSION;
	std::string path = ss.str();
	return path;
}

BinaryPackage::Header BinaryPackage::configureHeader() const
{
	Header header = { 0 };
	header.Version = CURRENT_VERSION;
	header.NumberOfBlobs = m_Resources.size();
	for ( int i = 0; i < m_Resources.size(); ++i )
	{
		header.TotalSize += m_Resources[ i ]->Size;
	}
	return header;
}

void BinaryPackage::incrementTypeCount( ResourceType type )
{
	m_TypeCount[ toUnderlying( type ) ]++;
}

//PUBLIC

InfoCode BinaryPackage::submit( ResourceType type, const std::string & path )
{
	if ( !exists( ( char * )path.c_str() ) )
	{
		return InfoCode::Path_NoExist;
	}

	uint32_t size = 0;
	char * buffer = read( path, &size );

	//create and push the resource
	incrementTypeCount( type );
	uint32_t handle = m_TypeCount[ static_cast<int>( type ) ];
	m_Resources.push_back( new BinaryResource( type, handle, size, buffer ) );

	return InfoCode::Success;
}

InfoCode BinaryPackage::remove( ResourceType type, uint32_t handle )
{
	if ( m_Resources.empty() )
		return InfoCode::Zero_Data;
	else if ( handle >= m_Resources.size() )
		return InfoCode::Outside_Boundry;

	for ( int i = 0; i < m_Resources.size(); ++i )
	{
		auto resource = m_Resources[ i ];
		if ( resource->Handle == handle && resource->Type == type )
		{
			delete resource;
			m_Resources[ i ] = nullptr;
			return InfoCode::Success;
		}
	}

	return InfoCode::Zero_Data;
}

InfoCode BinaryPackage::pack( const std::string & filename, uint32_t headerVersion )
{
	if ( CURRENT_VERSION != headerVersion )
	{
		return InfoCode::Version_Unsupported;
	}

	if ( m_Resources.empty() )
	{
		return InfoCode::Zero_Data;
	}

	//configure header before packing
	Header header = configureHeader();
	std::string path = appendPackageExtention( filename );
	//pack header
	std::ofstream package( path.c_str(), std::ios::binary );
	package.write( ( char * )&header, sizeof( Header ) );

	//pack each resource in order of the BinaryResource struct
	for ( int i = 0; i < m_Resources.size(); ++i )
	{
		BinaryResource * resource = m_Resources[ i ];
		package.write( (char *)&resource->Type, sizeof( ResourceType ) );
		package.write( (char *)&resource->Handle, sizeof( uint32_t ) );
		package.write( (char *)&resource->Size, sizeof( uint32_t ) );
		package.write( (char *)resource->Buffer, resource->Size );
	}

	package.close();

	return InfoCode::Success;
}

InfoCode BinaryPackage::unpack( const std::string & package, const std::string & outDirectory )
{
	Header header = { 0 };
	std::ifstream input( package.c_str(), std::ios::binary );
	input.seekg( 0, std::ios::beg );
	input.read( ( char * )&header, sizeof( Header ) );

	if ( header.Version != CURRENT_VERSION )
	{
		input.close();
		return InfoCode::Version_Unsupported;
	}
	else
	{
		uint32_t count = header.NumberOfBlobs;
		for ( uint32_t i = 0; i < count; ++i )
		{
			//unpack to file (the same unpacking method, the only difference from reading into memory is stringoperations
			//and creating the file with the initial data)
			//for each blob in file we must unpack into a sepparate new file
			BinaryResource resource;
			//we will create each file named by its handle and extension by its type
			//according to chpkg format version 1, the 4 bytes after either a header or a blob of data will be the
			//type, and the following 4 bytes after will be the generated handle.
			input.read( ( char * )&resource.Type, sizeof( ResourceType ) );
			input.read( ( char * )&resource.Handle, sizeof( uint32_t ) );
			input.read( ( char * )&resource.Size, sizeof( uint32_t ) );


			resource.Buffer = new char[ resource.Size ];
			input.read( ( char * )resource.Buffer, resource.Size );
			

			//Now that we have the values we need to generate a filename, we perform necessary string operations.
			auto name = int_to_string( resource.Handle );
			
			int typeValue = static_cast< int >( resource.Type );
			auto extension = m_EnumMap[ typeValue ];

			std::ostringstream ss;
			if ( hasEnding( outDirectory, "/" ) )
			{
				ss << outDirectory << name << "." << extension;
			}
			else
			{
				ss << outDirectory << "/" << name << "." << extension;
			}
			
			std::ofstream out( ss.str(), std::ios::binary );
			//and now we write the data into the file

			out.write( resource.Buffer, resource.Size );
			out.close();
			delete[] resource.Buffer;
		}
	}
	input.close();
	
	return InfoCode::Success;
}

void BinaryPackage::clear()
{
	uint32_t count = m_Resources.size();
	if ( count < 1 )
		return;

	for ( uint32_t i = 0; i < count; ++i )
	{
		if ( m_Resources[ i ] )
		{
			delete m_Resources[ i ];
			m_Resources[ i ] = nullptr;
		}
	}

	m_Resources.clear();
	m_Resources.swap( m_Resources );
}
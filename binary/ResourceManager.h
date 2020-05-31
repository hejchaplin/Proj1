#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include "InfoCodes.h"

template<typename E>
constexpr auto toUnderlying( E e ) noexcept
{
	return static_cast< std::underlying_type_t<E> >( e );
}
#define CHSN_TYPE_MAX \
			static_cast< int >( ResourceType::ResourceType_MAX )

enum class ResourceType
{
	Texture = 0,
	Shader,
	Sound,
	Mesh,

	ResourceType_MAX
};

struct BinaryResource
{
	ResourceType	Type;
	uint32_t		Handle;
	uint32_t		Size;
	char *			Buffer;

	BinaryResource( ResourceType type, uint32_t handle, uint32_t size, char * buffer )
		: Type( type ), Handle(handle), Size( size ), Buffer( buffer )
	{
	}
	
	BinaryResource()
		: Type( ResourceType::Texture ), Handle( 0xffffffff ), 
		  Size( 0 ), Buffer( nullptr )
	{
	}
};

class IPacker
{
public:
	virtual ~IPacker() = default;
	virtual InfoCode submit( ResourceType type, const std::string & path ) = 0;
	virtual InfoCode remove( ResourceType type, uint32_t handle ) = 0;
	virtual InfoCode pack( const std::string & filename, uint32_t headerVersion ) = 0;
	virtual InfoCode unpack( const std::string & path, const std::string & outDirectory ) = 0;
};

class BinaryPackage : IPacker
{
public:
	BinaryPackage()
	{
		for ( int i = 0; i < CHSN_TYPE_MAX; ++i )
		{
			m_TypeCount[ i ] = 0;
		}

		m_EnumMap[ 0 ] = "texture";
		m_EnumMap[ 1 ] = "shader";
		m_EnumMap[ 2 ] = "sound";
		m_EnumMap[ 3 ] = "mesh";
	}

	~BinaryPackage()
	{
		clear();
	}

	InfoCode submit( ResourceType type, const std::string & path );
	InfoCode remove( ResourceType type, uint32_t handle );

	//Overrides current package data always, modify for upcoming need
	InfoCode pack( const std::string & filename, uint32_t headerVersion );
	InfoCode unpack( const std::string & path, const std::string & outDirectory );

	void clear();

	std::vector<BinaryResource *> getResources() const
	{
		return m_Resources;
	}

private:
	struct Header
	{
		uint32_t		Version;
		uint32_t		TotalSize;
		uint32_t		NumberOfBlobs;
	};

	std::vector<BinaryResource *>	m_Resources;
	uint32_t						m_TypeCount[ CHSN_TYPE_MAX ];
	std::string						m_EnumMap[ CHSN_TYPE_MAX ];
	const uint32_t					CURRENT_VERSION = 1;
	const std::string				EXTENSION = ".chpkg";

	std::string appendPackageExtention( const std::string & filename ) const;
	void incrementTypeCount( ResourceType type );
	Header configureHeader() const;
};
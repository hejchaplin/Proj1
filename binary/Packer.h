#pragma once
#include "ResourceManager.h"

class Packer
{
private:
	IPacker * legacy_class;
public:

	Packer() = default;
	Packer( IPacker * legacy )
		: legacy_class{ legacy }
	{
	}

	~Packer()
	{
		delete legacy_class;
	}

	InfoCode submit( ResourceType type, const std::string & path )
	{
		return legacy_class->submit( type, path );
	}
	InfoCode remove( ResourceType type, uint32_t handle )
	{
		return legacy_class->remove( type, handle );
	}
	InfoCode pack( const std::string & filename, uint32_t headerVersion )
	{
		return legacy_class->pack( filename, headerVersion );
	}
	InfoCode unpack( const std::string & path, const std::string & outDirectory )
	{
		return legacy_class->unpack( path, outDirectory );
	}
};
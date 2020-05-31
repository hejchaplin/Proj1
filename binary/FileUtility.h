#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>

static char * read( const std::string & path, uint32_t * size )
{
	std::ifstream file( path.c_str(), std::ios::binary );
	file.seekg( 0, std::ios::end );
	*size = file.tellg();
	char * buffer = new char[ *size ];
	file.seekg( 0, std::ios::beg );
	file.read( buffer, *size );
	file.close();
	return buffer;
}

static std::string int_to_string( int in )
{
	std::ostringstream ss;
	ss << in;
	return ss.str();
}

static bool exists( char * filePath )
{
	//This will get the file attributes bitlist of the file
	DWORD fileAtt = GetFileAttributesA( filePath );

	//If an error occurred it will equal to INVALID_FILE_ATTRIBUTES
	if ( fileAtt == INVALID_FILE_ATTRIBUTES )
		return false;

	//If the path referers to a directory it should also not exists.
	return ( ( fileAtt & FILE_ATTRIBUTE_DIRECTORY ) == 0 );
}
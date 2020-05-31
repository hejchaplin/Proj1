#pragma once

#define CH_RESOURCE_FAILED( x ) \
			( ( InfoCode )( x ) != InfoCode::Success )
#define CH_ERRORCODE(x) \
			static_cast< int >( (InfoCode)x )

enum class InfoCode
{
	Success = 0,
	Version_Unsupported,
	Zero_Data,
	Path_NoExist,
	Outside_Boundry,
};
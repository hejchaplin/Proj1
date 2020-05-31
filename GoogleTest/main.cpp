#include "pch.h"
#include "../binary/Packer.h"
#include "../binary/Array.h"
#include "../binary/String.h"
#include <gmock/gmock.h>

///////////////////////////////////////////////
//////////		MOCK SETUP
///////////////////////////////////////////////

struct IPackage
{
	struct Header
	{
		uint32_t version;
	};
	std::string valid_path = "valid";
	uint32_t valid_version = 1;
	uint32_t data_count = 0;

	virtual InfoCode submit( ResourceType type, const std::string & path )
	{
		if ( path == valid_path )
		{
			++data_count;
			return InfoCode::Success;
		}
		else
			return InfoCode::Path_NoExist;
		//noexist, success
	}

	virtual InfoCode remove( ResourceType type, uint32_t handle )
	{
		//zero, outside, success
		return InfoCode::Success;
	}

	virtual InfoCode pack( const std::string & filename, uint32_t headerVersion ) const
	{
		if ( valid_version != headerVersion )
		{
			return InfoCode::Version_Unsupported;
		}
		if ( data_count == 0 )
		{
			return InfoCode::Zero_Data;
		}
		return InfoCode::Success;
	}

	virtual InfoCode unpack( const std::string & path, const std::string & outDirectory ) const
	{
		return InfoCode::Success;
	}

	virtual std::string appendPackageExtention( const std::string & filename )
	{
		return filename + ".pkg";
	}

	virtual Header configureHeader()
	{
		Header h;
		h.version = 1;
		return h;
	}
};

struct MockPackage : public IPackage
{
	MOCK_METHOD( InfoCode, submit, ( ResourceType type,				const std::string & path ) );
	MOCK_METHOD( InfoCode, remove, ( ResourceType type,				uint32_t handle ) );
	MOCK_METHOD( InfoCode, pack,   ( const std::string & filename,	uint32_t headerVersion ) );
	MOCK_METHOD( InfoCode, unpack, ( const std::string & path,		const std::string & outDirectory ) );
};


///////////////////////////////////////////////
//////////		TESTS
///////////////////////////////////////////////

TEST( MockPackageTest, RemoveTestSUCCESS )
{
	//arange
	MockPackage mpkg;
	EXPECT_CALL( mpkg, remove( testing::_, testing::_ ) ).Times( 1 ).WillOnce( testing::Return( InfoCode::Success ) );
	
	//act
	InfoCode retValue = mpkg.remove( ResourceType::Mesh, 1 );

	//assert
	EXPECT_EQ( retValue, InfoCode::Success );
}

TEST( MockPackageTest, RemoveTestFAIL )
{
	//arange
	MockPackage mpkg;
	EXPECT_CALL( mpkg, remove( testing::_, testing::_ ) ).Times( 1 ).WillOnce( testing::Return( InfoCode::Zero_Data ) );

	//act
	InfoCode retValue = mpkg.remove( ResourceType::Mesh, 1 );

	//assert
	EXPECT_EQ( retValue, InfoCode::Zero_Data );
}

TEST( SuperMockTest, supertest )
{
	//arange
	MockPackage mpkg;
	EXPECT_CALL( mpkg, submit( testing::_, testing::_ ) ).Times( 1 ).WillOnce( testing::Return( InfoCode::Path_NoExist ) );

	//act
	InfoCode submitRetValue = mpkg.submit( ResourceType::Shader, "miss_spelled_filepath" );
	
	//assert
	EXPECT_EQ( submitRetValue, InfoCode::Path_NoExist );
}











struct LegacyClassMock : public IPacker
{
	MOCK_METHOD( InfoCode, submit, ( ResourceType type, const std::string & path ) );
	MOCK_METHOD( InfoCode, remove, ( ResourceType type, uint32_t handle ) );
	MOCK_METHOD( InfoCode, pack, ( const std::string & filename, uint32_t headerVersion ) );
	MOCK_METHOD( InfoCode, unpack, ( const std::string & path, const std::string & outDirectory ) );
};

TEST( MPackageTests, invoke_submit )
{
	auto * legacy_class = new LegacyClassMock();

	Packer p{ legacy_class };

	EXPECT_CALL( *legacy_class, submit );
	p.submit(ResourceType::Mesh, "test");

}

TEST( MPackageTests, invoke_remove )
{
	auto * legacy_class = new LegacyClassMock();

	Packer p{ legacy_class };

	EXPECT_CALL( *legacy_class, submit );
	p.submit( ResourceType::Mesh, "test" );

}

TEST( MPackageTests, invoke_pack )
{
	auto * legacy_class = new LegacyClassMock();

	Packer p{ legacy_class };

	EXPECT_CALL( *legacy_class, submit );
	p.submit( ResourceType::Mesh, "test" );

}

TEST( MPackageTests, invoke_unpack )
{
	auto * legacy_class = new LegacyClassMock();

	Packer p{ legacy_class };

	EXPECT_CALL( *legacy_class, submit );
	p.submit( ResourceType::Mesh, "test" );

}










///////////////////////////////////////////////
//////////		GMOCK SETUP
///////////////////////////////////////////////
#include <fstream>
int main( int argc, char ** argv )
{
	// The following line must be executed to initialize Google Mock
	// (and Google Test) before running the tests.
	::testing::InitGoogleMock( &argc, argv );
	return RUN_ALL_TESTS();
}
#include "pch.h"
#include "../binary/Packer.h"
#include "../binary/Array.h"
#include "../binary/String.h"
#include <gmock/gmock.h>

///////////////////////////////////////////////
//////////		MOCK SETUP
///////////////////////////////////////////////

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

	EXPECT_CALL( *legacy_class, remove );
	p.remove( ResourceType::Mesh, 2 );
}

TEST( MPackageTests, invoke_pack )
{
	auto * legacy_class = new LegacyClassMock();

	Packer p{ legacy_class };

	EXPECT_CALL( *legacy_class, pack );
	p.pack( "file", 1 );
}

TEST( MPackageTests, invoke_unpack )
{
	auto * legacy_class = new LegacyClassMock();

	Packer p{ legacy_class };

	EXPECT_CALL( *legacy_class, unpack );
	p.unpack( "file", "outdir" );
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
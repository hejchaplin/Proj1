#include "pch.h"
#include "CppUnitTest.h"
#include "../binary/ResourceManager.h"
#include "../binary/FileObserver.h"
//#include "../binary/Array.h"
//#include "../binary/String.h"

#include <fstream>
namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> 
			static std::wstring ToString( const InfoCode & errorCode )
			{
				return errorCode == InfoCode::Path_NoExist ? L"F" : L"R";
			};


			/*template<>
			static std::wstring ToString( const ResourceType & errorCode )
			{
				return errorCode == ResourceType::Shader ? L"F" : L"R";
			};*/

			/////////////////////////////////////////////////////////
			///////// UNORDERED ARRAY TESTS
			/////////////////////////////////////////////////////////

			TEST_CLASS(ArrayTests)
			{
			public:
		
				TEST_METHOD( testLength )
				{
					uint32_t expected = 6;

					CArray<int> f;
					for ( int i = 0; i < expected; ++i )
					{
						f.push( i );
					}
					Assert::AreEqual( expected, f.length() );
				}

				TEST_METHOD( testUnordered )
				{
					int32_t expected = 5;

					CArray<int> f0;
					for ( int i = 0; i < 6; ++i )
					{
						f0.push( i );
					}
					f0.remove( 0 );
					Assert::AreEqual( f0.getCopy( 0 ), expected );
				}

				TEST_METHOD( testException )
				{
					auto func = [&]
					{
						CArray<int> f0;
						f0.push( 5 );
						auto f = f0.getCopy( 1 );
					};
					Assert::ExpectException<std::out_of_range>( func );
				}
			};

			/////////////////////////////////////////////////////////
			///////// STRING TESTS
			/////////////////////////////////////////////////////////

			TEST_CLASS( StringTests )
			{
			public:

				TEST_METHOD( testAssignments )
				{
					const char expected[9] = "test";

					CString s0 = "test";
					CString s1 = s0;

					Assert::AreEqual( s1.raw(), expected );
				}

				TEST_METHOD( testAppend )
				{
					const char expected[ 9 ] = "testtest";

					CString s0 = "test";
					CString s1 = s0;
					CString s2 = s0 + s1;

					Assert::AreEqual( s2.raw(), expected );

					s1 += " med+=";
					Assert::AreEqual( s1.raw(), "test med+=" );
				}

				TEST_METHOD( testLength )
				{
					CString s0 = "test";
					uint32_t expected = 4;

					Assert::AreEqual( expected, s0.length() );
				}
			};
			
			/////////////////////////////////////////////////////////
			///////// PACKAGER TESTS
			/////////////////////////////////////////////////////////

			TEST_CLASS( PackageTests )
			{
			public:

				/////////////////////////////////////////////////////////
				///////// SUBMIT
				/////////////////////////////////////////////////////////

				TEST_METHOD( testSubmitSuccess )
				{
					BinaryPackage package;

					InfoCode info = package.submit( ResourceType::Shader, "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					InfoCode expected = InfoCode::Success;
					Assert::AreEqual( expected, info );
				}

				TEST_METHOD( testSubmitFailed )
				{
					BinaryPackage package;

					InfoCode info = package.submit( ResourceType::Shader, "C:/DoesntExist" );
					InfoCode expected = InfoCode::Path_NoExist;
					Assert::AreEqual( expected, info );
				}

				/////////////////////////////////////////////////////////
				///////// REMOVE
				/////////////////////////////////////////////////////////

				TEST_METHOD( testRemoveFailed )
				{
					BinaryPackage package;

					InfoCode info = package.remove( ResourceType::Shader, 0 );
					InfoCode expected = InfoCode::Zero_Data;
					Assert::AreEqual( expected, info );
				}

				TEST_METHOD( testRemoveSuccess )
				{
					BinaryPackage package;
					package.submit( ResourceType::Shader, "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					package.submit( ResourceType::Shader, "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					package.submit( ResourceType::Shader, "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					package.submit( ResourceType::Shader, "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					InfoCode info = package.remove( ResourceType::Shader, 3 );
					InfoCode expected = InfoCode::Success;
					Assert::AreEqual( expected, info );
				}

				/////////////////////////////////////////////////////////
				///////// PACK
				/////////////////////////////////////////////////////////

				TEST_METHOD( testPackSuccess )
				{
					BinaryPackage package;
					auto info = package.submit( ResourceType::Shader, "C:/Users/megadodd/source/repos/binary/binary/test/readme.txt" );
					info = package.submit( ResourceType::Texture, "C:/Users/megadodd/source/repos/binary/binary/test/Kappa.png" );
					if ( CH_RESOURCE_FAILED( info ) )
					{
						int errorCode = CH_ERRORCODE( info );
					}
					info = package.pack( "C:/Users/megadodd/source/repos/binary/Test_binary/test_packages/packedTest", 1 );
					Assert::AreEqual( InfoCode::Success, info );
				}

				TEST_METHOD( testPackFail_zero_data )
				{
					BinaryPackage package;
					auto info = package.pack( "C:/Users/megadodd/source/repos/binary/Test_binary/test_packages/packedTest", 1 );
					InfoCode expected = InfoCode::Zero_Data;
					Assert::AreEqual( expected, info );
				}

				TEST_METHOD( testPackFail_wrong_version )
				{
					BinaryPackage package;
					package.submit( ResourceType::Shader, "C:/Users/megadodd/source/repos/binary/binary/test/readme.txt" );
					auto info = package.pack( "C:/Users/megadodd/source/repos/binary/Test_binary/test_packages/packedTest", 2 );
					InfoCode expected = InfoCode::Version_Unsupported;
					Assert::AreEqual( expected, info );
				}

				/////////////////////////////////////////////////////////
				///////// UNPACK
				/////////////////////////////////////////////////////////

				TEST_METHOD( testUnpackSuccess )
				{
					BinaryPackage package;
					InfoCode actual = package.unpack( "C:/Users/megadodd/source/repos/binary/Test_binary/ver1.chpkg", "C:/Users/megadodd/source/repos/binary/Test_binary/test_packages/unpacked_test" );
					InfoCode expected = InfoCode::Success;
					Assert::AreEqual( expected, actual );
				}
				TEST_METHOD( testUnpackFail_wrong_version )
				{
					BinaryPackage package;
					InfoCode actual = package.unpack( "C:/Users/megadodd/source/repos/binary/Test_binary/test_packages/Version2.chpkg", "C:/Users/megadodd/source/repos/binary/Test_binary/test_packages/unpacked_test" );
					InfoCode expected = InfoCode::Version_Unsupported;
					Assert::AreEqual( expected, actual );
				}
			};

			/////////////////////////////////////////////////////////
			///////// OBSERVER TESTS
			/////////////////////////////////////////////////////////

			TEST_CLASS( ObserverTests )
			{
			public:
				TEST_METHOD( testNoticeFileChangeTick )
				{
					Observer obs( 10 );
					CArray<CString> files;
					files.push( "C:/Users/megadodd/source/GitHub/Project1/Assets/Text/readme.txt" );
					InfoCode submit_error = obs.submitFiles( files );

					for ( int i = 0; i < 25; ++i )
					{
						obs.startTickSpecified( []( FileStatus status, const CString & file )
							{
								if ( status == FileStatus::Modified )
								{
									CString expected = "C:/Users/megadodd/source/GitHub/Project1/Assets/Text/readme.txt";
									bool flag = false;
									if ( expected == file )
									{
										flag = true;
									}
									Assert::IsTrue( flag );
								}
							},
							0.33f );

						if ( i == 1 )
						{
							std::ofstream f( "C:/Users/megadodd/source/GitHub/Project1/Assets/Text/readme.txt" );
							f << "modify";
							f.close();
						}
					}
				}

				TEST_METHOD( testSubmitSuccess )
				{
					Observer observer( 10 );
					InfoCode expected = InfoCode::Success;

					CArray<CString> files;
					files.push( "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					files.push( "C:/Users/megadodd/source/repos/binary/binary/assets/1.texture" );
					InfoCode actual = observer.submitFiles( files );

					Assert::AreEqual( expected, actual );
				}

				TEST_METHOD( testSubmitFailed )
				{
					Observer observer( 10 );
					InfoCode expected = InfoCode::Path_NoExist;

					CArray<CString> files;
					files.push( "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					files.push( "C:/Doesn't_Exist" );
					InfoCode actual = observer.submitFiles( files );

					Assert::AreEqual( expected, actual );
				}

				TEST_METHOD( testTickFailed )
				{
					Observer observer( 10 );
					InfoCode expected = InfoCode::Zero_Data;

					InfoCode actual = observer.startTickSpecified(
						[]( FileStatus status, const CString & file )
						{
							Assert::IsTrue( false );
						},
						0.33f );

					Assert::AreEqual( expected, actual );
				}

				TEST_METHOD( testTickSuccess )
				{
					Observer observer( 10 );
					InfoCode expected = InfoCode::Success;
					CArray<CString> files;
					files.push( "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					observer.submitFiles( files );

					InfoCode actual = observer.startTickSpecified(
						[]( FileStatus status, const CString & file )
						{
							Assert::IsTrue( false );
						}, 
						0.33f );
					
					Assert::AreEqual( expected, actual );
				}

				TEST_METHOD( testThreadFailed )
				{
					Observer observer( 10 );
					InfoCode expected = InfoCode::Zero_Data;

					observer.setThreadObserverRunning( true );
					InfoCode actual = observer.startThreadSpecified( 
						[]( FileStatus status, const CString & file )
						{
							Assert::IsTrue( false );
						} );

					Assert::AreEqual( expected, actual );
				}

				TEST_METHOD( testThreadSuccess )
				{
					Observer observer( 10 );
					InfoCode expected = InfoCode::Success;
					CArray<CString> files;
					files.push( "C:/Users/megadodd/source/repos/binary/binary/assets/1.shader" );
					observer.submitFiles( files );

					observer.setThreadObserverRunning( false );
					InfoCode actual = observer.startThreadSpecified( 
						[]( FileStatus status, const CString & file )
						{
							Assert::IsTrue( false );
						} );
					
					Assert::AreEqual( expected, actual );
				}
			};
		}


	}
}
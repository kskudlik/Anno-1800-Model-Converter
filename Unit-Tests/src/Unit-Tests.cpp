#include "CppUnitTest.h"
#include "RDM-Converter.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
std::filesystem::path projectDir = __ProjectDir__;
// clang fucks up formatting big time for some reason...
// clang-format off
namespace ConverterTests
{
	template<typename InputIterator1, typename InputIterator2>
	bool
	range_equal(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2)
	{
		while(first1 != last1 && first2 != last2)
		{
			if(*first1 != *first2) return false;
			++first1;
			++first2;
		}
		return (first1 == last1) && (first2 == last2);
	}

	bool compare_files(const std::filesystem::path& filepath1, const std::filesystem::path& filepath2)
	{
		std::ifstream file1(filepath1);
		std::ifstream file2(filepath2);

		std::istreambuf_iterator<char> begin1(file1);
		std::istreambuf_iterator<char> begin2(file2);

		std::istreambuf_iterator<char> end;

		return range_equal(begin1, end, begin2, end);
	}
	
	bool testIfConvertedOBJIsEqualToExpectedOBJ(std::filesystem::path filename)
	{
		std::filesystem::path input = projectDir / "rdm-files\\" / filename;
		std::filesystem::path output = projectDir / "rdm-files\\output\\" / filename.replace_extension(".obj");
		std::filesystem::path expected = projectDir / "rdm-files\\expected\\" / filename.replace_extension(".obj");
		RDMFile testRDM(input); //TODO catch exceptions and log them
		testRDM.toOBJFile(output);
		
		return compare_files(output,expected);
	}

	TEST_CLASS(RDMTest)
	{
	public:

		TEST_METHOD_INITIALIZE(init)
		{
			std::filesystem::path outputDir = projectDir / "rdm-files\\output";
			std::filesystem::remove_all(outputDir);
		}
		TEST_METHOD_CLEANUP(clean)
		{
			std::filesystem::path outputDir = projectDir / "rdm-files\\output";
			std::filesystem::remove_all(outputDir);
		}
		TEST_METHOD(isTheCakeALie)
		{
			Assert::AreEqual(GLADOS,EVIL);
		}

		TEST_METHOD(P4h) // 8
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("3rd_party_02_ship_cutout_lod0.rdm"));
		}
		TEST_METHOD(P4h_T2h_C4c) // 16
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("cultural_props_1x1_03_decal_detail01_lod0.rdm"));
		}
		TEST_METHOD(P4h_N4b_T2h_I4b) // 20
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("pigeon_lod2.rdm"));
		}
		TEST_METHOD(P4h_N4b_G4b_B4b_T2h_Triangle_2) // 24 ("default")
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("preorder_statue_lod0.rdm"));
		}
		TEST_METHOD(P4h_N4b_G4b_B4b_T2h_Triangle_4) // 24
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("monument_01_04_var1_lod0.rdm"));
		}
		TEST_METHOD(P4h_N4b_G4b_B4b_T2h_I4b) // 28
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("agriculture_03_lod2.rdm"));
		}
		TEST_METHOD(P4h_N4b_G4b_B4b_T2h_C4b_C4b) // 32
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("agriculture01_field_var0_lod0.rdm"));
		}
		TEST_METHOD(P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b_Triangle_2) // 56
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("3rd_party_02.rdm"));
		}
		TEST_METHOD(P4h_N4b_G4b_B4b_T2h_I4b_I4b_I4b_I4b_W4b_W4b_W4b_W4b_Triangle_4) // 56
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("3rd_party_08.rdm"));
		}
		TEST_METHOD(P3f_N3b_37_T2f) // 60
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("3rd_party_08_building_01_cloth_lod0.rdm"));
		}
		TEST_METHOD(P3f_N3b_41_T2f) // 64
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("flag_pirate_03.rdm"));
		}
		TEST_METHOD(P3f_N3b_45_T2f) // 68
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("frigate_sail__bm_01.rdm"));
		}
		TEST_METHOD(P3f_N3b_49_T2f) // 72
		{
			Assert::IsTrue(testIfConvertedOBJIsEqualToExpectedOBJ("command_ship_sail_bk_01.rdm"));
		}



	};
}
// clang-format on

#include "Maverick/Global/Global.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/FileHandler/FileHandler.hpp"
#include <gtest/gtest.h>
#include <arpa/inet.h>

bool endian;

TEST(DataSerialization, DetermineEndianess){
    ASSERT_EQ(Engine::getEndian(), htons(1) == htons(1));
}

TEST(DataSerialization, ReadWriteVar){
    endian = 1;

    uint8_t var1 = 1;
    int8_t var2 = 2;
    uint16_t var3 = 3;
    int16_t var4 = 4;
    uint32_t var5 = 5;
    int32_t var6 = 6;
    uint64_t var7 = 7;
    int64_t var8 = 8;

    FILE* test_file = fopen("test_file", "wb");
    WriteVar(var1, test_file);
    WriteVar(var2, test_file);
    WriteVar(var3, test_file);
    WriteVar(var4, test_file);
    WriteVar(var5, test_file);
    WriteVar(var6, test_file);
    WriteVar(var7, test_file);
    WriteVar(var8, test_file);
    fclose(test_file);

    test_file = fopen("test_file", "rb");
    ReadVar(var1, test_file);
    ReadVar(var2, test_file);
    ReadVar(var3, test_file);
    ReadVar(var4, test_file);
    ReadVar(var5, test_file);
    ReadVar(var6, test_file);
    ReadVar(var7, test_file);
    ReadVar(var8, test_file);
    fclose(test_file);

    ASSERT_EQ(var1, 1);
    ASSERT_EQ(var2, 2);
    ASSERT_EQ(var3, 3);
    ASSERT_EQ(var4, 4);
    ASSERT_EQ(var5, 5);
    ASSERT_EQ(var6, 6);
    ASSERT_EQ(var7, 7);
    ASSERT_EQ(var8, 8);
}

TEST(StrUtility, StringDeepCopy){
    char* ref_str = "Here's a\n test string\t with some escape characters!";
    char* test_str = StrDeepCopy(ref_str);
    ASSERT_EQ(strcmp(ref_str, test_str), 0);
    free(test_str);
}

TEST(StrUtility, StrSetCase){
    char* comp_str = StrDeepCopy("ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+");
    ToLower(comp_str);
    ASSERT_EQ(strcmp(comp_str, "abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+"), 0);
    free(comp_str);

    comp_str = StrDeepCopy("abcdefghijklmnopqrstuvwxyz!@#$%^&*()_+");
    ToUpper(comp_str);
    ASSERT_EQ(strcmp(comp_str, "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+"), 0);
    free(comp_str);
}

TEST(StrUtility, ParseArgs){
    char* str = StrDeepCopy("Arg1 \"Arg 2\" Arg\\\\3 'Arg 4'");
    char** args = GetArgs(str, " ");
    ASSERT_NE(args, nullptr);

    ASSERT_NE(args[0], nullptr);
    ASSERT_EQ(strcmp(args[0], "Arg1"), 0);

    ASSERT_NE(args[1], nullptr);
    ASSERT_EQ(strcmp(args[1], "Arg 2"), 0);

    ASSERT_NE(args[2], nullptr);
    ASSERT_EQ(strcmp(args[2], "Arg\\3"), 0);

    ASSERT_NE(args[3], nullptr);
    ASSERT_EQ(strcmp(args[3], "Arg 4"), 0);

    ASSERT_EQ(args[4], nullptr);

    free(str);
}

int main(int argc, char** argv){
    Engine::getInstance();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include "gtest/gtest.h"
#include "templater.h"
#include <url_breaker.h>
#include <cppdb.h>
#include <time.h>

#include <common.h>
#include <cppdb/cppdb.h>
#include <tnt/httprequest.h>
#include <tnt/httpreply.h>
#include <tnt/tntnet.h>

#include <string>

TEST(StringTemplate, DISABLED_SetKeyword)
{
    STRING_TEMPLATE(SetStuff, "<!--#set some_var ABCD#-->ThisIs{some_var}");
    std::string sst = templater<SetStuff>().templatize().get();
    ASSERT_STREQ(sst.c_str(), "ThisIsABCD");
}

TEST(StringTemplate, IncludeRootData)
{

    STRING_TEMPLATE(RootRequiredIncluded, "Testing {str}");
    STRING_TEMPLATE(RootProviderIncTestStuff, "Testing <!--#include RootRequiredIncluded#-->");

    std::string tf = templater<RootProviderIncTestStuff>().templatize("str" <is> 123 ).get();

    ASSERT_STREQ(tf.c_str(), "Testing Testing 123");
}

TEST(StringTemplate, IfEq)
{
    STRING_TEMPLATE(IfElseStuff, "Testing<!--#eq {str},blabla#-->blaa<!--#endeq#-->123");
    template_par<std::string> a("str", "blabla");
    std::string tf = templater<IfElseStuff>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "Testingblaa123");
}

TEST(StringTemplate, DISABLED_Script)
{
    STRING_TEMPLATE(ScriptStuff, "Testing<!--#script#-->"
                                 "int i;"
                                 "for(i=0; i<10; i++) print(\"{str}\");"
                                 "<!--#endscript#-->123");
    template_par<std::string> a("str", "A");
    std::string tf = templater<ScriptStuff>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "TestingAAAAAAAAAA123");
}

TEST(StringTemplate, IfElse)
{
    STRING_TEMPLATE(IfElseStuff, "<!--#define blaa#-->Testing<!--#if blaa#-->blaa<!--#else#-->{str}<!--#endif blaa#-->123");
    template_par<std::string> a("str", "blabla");
    std::string tf = templater<IfElseStuff>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "Testingblaa123");
}

TEST(StringTemplate, Defines)
{
    STRING_TEMPLATE(DefineStuff, "<!--#define blaa#--><!--#if blaa#-->Testing {str}<!--#endif blaa#-->");
    template_par<std::string> a("str", "blabla");
    std::string tf = templater<DefineStuff>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "Testing blabla");
}

TEST(StringTemplate, NotSoDefines)
{
    STRING_TEMPLATE(NotSoDefines, "<!--#define not_blaa#--><!--#if blaa#-->Testing {str}<!--#endif blaa#-->");
    template_par<std::string> a("str", "blabla");
    std::string tf = templater<NotSoDefines>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "");
}

TEST(StringTemplate, ValueDefines)
{
    STRING_TEMPLATE(ValueDefines, "<!--#define blaa=bluu#-->Testing {blaa}{str}");
    template_par<std::string> a("str", "blabla");
    std::string tf = templater<ValueDefines>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "Testing bluublabla");
}

TEST(StringTemplate, IfTest)
{
    STRING_TEMPLATE(IfTestStuff, "Testing<!--#if str#-->:This is {str}<!--#endif str#-->");
    template_par<std::string> a("str", "blabla");
    std::string tf = templater<IfTestStuff>().templatize(a).get();
    ASSERT_STREQ(tf.c_str(), "Testing:This is blabla");

    template_par<std::string> b("str", "");
    std::string no_par = templater<IfTestStuff>().templatize(b).get();
    ASSERT_STREQ(no_par.c_str(), "Testing");
}

TEST(StringTemplate, SimpleString)
{
    STRING_TEMPLATE(TestStuff, "Testing {str}");

    template_par<std::string> a("str", "blabla");
    std::string tf = templater<TestStuff>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "Testing blabla");
}

TEST(StringTemplate, SimpleNumber)
{
    STRING_TEMPLATE(NumberTestStuff, "Testing {str}");

    auto x = "str" <is> 42;

    std::string tf = templater<NumberTestStuff>().templatize(x).get();

    ASSERT_STREQ(tf.c_str(), "Testing 42");
}

TEST(StringTemplate, SimpleInclusion)
{
    STRING_TEMPLATE(TestIncluded, "Testing {str}");
    STRING_TEMPLATE(IncTestStuff, "Testing <!--#include TestIncluded(str=\"blaa\")#-->");

    std::string tf = templater<IncTestStuff>().templatize().get();

    ASSERT_STREQ(tf.c_str(), "Testing Testing blaa");
}

TEST(StringTemplate, SimpleInclusionErroneous)
{
    STRING_TEMPLATE(TestIncluded1, "Testing {str}");
    STRING_TEMPLATE(IncTestStuff1, "Testing <!--#include TestIncluded(str=\"blaa)#-->");

    std::string tf = templater<IncTestStuff1>().templatize().get();

    ASSERT_STREQ(tf.c_str(), "Testing Testing blaa)#-->");
}

TEST(StringTemplate, SimpleInclusion2)
{
    STRING_TEMPLATE(TestIncluded2, "Testing");
    STRING_TEMPLATE(IncTestStuff2, "Testing <!--#include TestIncluded2#-->More");

    std::string tf = templater<IncTestStuff2>().templatize().get();

    ASSERT_STREQ(tf.c_str(), "Testing TestingMore");
}

TEST(StringTemplate, DoubleInclusion)
{
    STRING_TEMPLATE(DoubleTestIncluded, "Testing {str}");
    STRING_TEMPLATE(DoubleIncTestStuff, "Testing <!--#include TestIncluded(str=\"blaa\")#--><!--#include TestIncluded(str=\"blee\")#-->");

    std::string tf = templater<DoubleIncTestStuff>().templatize().get();

    ASSERT_STREQ(tf.c_str(), "Testing Testing blaaTesting blee");
}

TEST(StringTemplate, SimpleInclusionWithVariable)
{
    STRING_TEMPLATE(VarTestIncluded, "<!--#parameters st#-->Testing {str}");
    STRING_TEMPLATE(VarTestStuff, "Testing <!--#include TestIncluded(str:{more_str})#-->");

    template_par<std::string> a("more_str", "blabla");
    std::string tf = templater<VarTestStuff>().templatize(a).get();

    ASSERT_STREQ(tf.c_str(), "Testing Testing blabla");
}

TEST(StringTemplate, SimpleStruct)
{
    STRING_TEMPLATE(SimpleStructTemplate, "<!--#struct simple_pair(a,b)#-->"
                                          "<!--#parameters st:simple_pair#-->"
                                          "Testing {st.a}{st.b}");
    template_struct st("st", "simple_pair");
    st["a"] = "A";
    st["b"] = "B";
    std::string s = templater<SimpleStructTemplate>().templatize(st).get();

    ASSERT_STREQ(s.c_str(), "Testing AB");
}


TEST(StringTemplate, SimpleStructAndVar)
{
    STRING_TEMPLATE(VarSimpleStructTemplate, "<!--#struct simple_pair(a,b)#-->"
                                          "<!--#parameters st:simple_pair#-->"
                                          "Testing {st.a}{st.b}{x}");
    template_struct st("st", "simple_pair");
    st["a"] = "A";
    st["b"] = "B";
    std::string s = templater<VarSimpleStructTemplate>().templatize(
                st,
                "x" <is> 42
                ).get();

    ASSERT_STREQ(s.c_str(), "Testing AB42");
}


TEST(StringTemplate, VarAndSimpleStruct)
{
    STRING_TEMPLATE(VarAndSimpleStructTemplate, "<!--#struct simple_pair(a,b)#-->"
                                          "<!--#parameters st:simple_pair#-->"
                                          "Testing {x}{st.a}{st.b}");
    template_struct st("st", "simple_pair");
    st["a"] = "A";
    st["b"] = "B";
    std::string s = templater<VarAndSimpleStructTemplate>().templatize(
                "x" <is> 42,
                st
                ).get();

    ASSERT_STREQ(s.c_str(), "Testing 42AB");
}

TEST(StringTemplate, SimpleStruct2)
{
    STRING_TEMPLATE(SecondSimpleStructTemplate, "<!--#struct simple(c)#-->"
                                          "<!--#struct simple_pair(a,b)#-->"
                                          "<!--#parameters st:simple_pair, x:simple#-->"
                                          "Testing {x.c}{st.a}{st.b}");

    template_struct st("st", "simple_pair");
    st["a"] = "A";
    st["b"] = "B";

    template_struct x("x", "simple");
    x["c"] = "C";

    std::string s = templater<SecondSimpleStructTemplate>().templatize(
                x,
                st
                ).get();

    ASSERT_STREQ(s.c_str(), "Testing CAB");
}

TEST(StringTemplate, Iterator)
{
    STRING_TEMPLATE(ItSimpleStructTemplate, "<!--#struct simple(c)#-->"
                                          "<!--#parameters v:simple[]#-->"
                                          "Testing"
                                          "<!--#loop v#-->"
                                          "{v.c}"
                                          "<!--#endloop v#-->");

    std::vector<template_struct> structs;

    template_struct ts("unused", "simple");

    ts["c"] = "1";
    structs.push_back(ts);

    ts["c"] = "2";
    structs.push_back(ts);

    template_vector_par tvp("v", structs);

    std::string s = templater<ItSimpleStructTemplate>().templatize(
                tvp
                ).get();

    ASSERT_STREQ(s.c_str(), "Testing12");
}


TEST(StringTemplate, IteratorTwoStructsIfEq)
{
    STRING_TEMPLATE(IteratorTwoStructsIfEq,   "<!--#struct simple(c)#-->"
                                          "<!--#struct complex(d)#-->"
                                          "<!--#parameters v:simple[],w:complex[]#-->"
                                          "Testing"
                                          "<!--#loop v#-->"
                                          "{v.c}"
                                          "<!--#endloop v#-->"
                                          "<!--#loop w#-->"
                                          "<!--#eq {stg},{w.d}#-->"
                                          "{w.d}"
                                          "<!--#endeq#-->"
                                          "<!--#endloop w#-->"
                    );

    std::vector<template_struct> structs;
    std::vector<template_struct> structs2;

    template_struct ts("simples", "simple");
    template_struct tsw("doubles", "double");

    ts["c"] = "1"; structs.push_back(ts);
    ts["c"] = "2"; structs.push_back(ts);

    tsw["d"] = "3"; structs2.push_back(tsw);
    tsw["d"] = "4"; structs2.push_back(tsw);

    template_vector_par tvp("v", structs);
    template_vector_par tvp2("w", structs2);

    std::string s = templater<IteratorTwoStructsIfEq>().templatize("stg" <is> "3").
            templatize(tvp2).
            templatize(tvp).
            get();

    ASSERT_STREQ(s.c_str(), "Testing123");
}


TEST(StringTemplate, IteratorTwoStructs)
{
    STRING_TEMPLATE(IteratorTwoStructs,   "<!--#struct simple(c)#-->"
                                          "<!--#struct complex(d)#-->"
                                          "<!--#parameters v:simple[],w:complex[]#-->"
                                          "Testing"
                                          "<!--#loop v#-->"
                                          "{v.c}"
                                          "<!--#endloop v#-->"
                                          "<!--#loop w#-->"
                                          "{w.d}"
                                          "<!--#endloop w#-->"
                    );

    std::vector<template_struct> structs;
    std::vector<template_struct> structs2;

    template_struct ts("simples", "simple");
    template_struct tsw("doubles", "double");

    ts["c"] = "1"; structs.push_back(ts);
    ts["c"] = "2"; structs.push_back(ts);

    tsw["d"] = "3"; structs2.push_back(tsw);
    tsw["d"] = "4"; structs2.push_back(tsw);

    template_vector_par tvp("v", structs);
    template_vector_par tvp2("w", structs2);

    std::string s = templater<IteratorTwoStructs>().templatize(tvp2).templatize(tvp).get();

    ASSERT_STREQ(s.c_str(), "Testing1234");
}

TEST(StringTemplate, JsonSource)
{
    STRING_TEMPLATE(RequiredJsonTest, "Testing {str}");
    std::string s = templater<RequiredJsonTest>().templatize(nlohmann::json{
        {"str", "BLaaaaa"}
    }).get();
    std::cout << s << std::endl;
}

/********************************************************************************************************************/

TABLE(Address)
    COLUMN(Id, INTEGER);
    COLUMN(StreetName, VARCHAR(256));
    COLUMN(Code, INTEGER);
ENDTABLE(Address)

TABLE(Person)
    COLUMN(Id, INTEGER, PRIMARY_KEY);
    COLUMN(AddressId, INTEGER, NOT_NULL, DEFAULT(1));
    COLUMN(Name, VARCHAR(256));
    COLUMN(Age, INTEGER);
    COLUMN(Birthday, TIMESTAMP, DEFAULT(CURRENT_TIMESTAMP));
    FOREIGN_KEY(AddressId -> Address.Id);
ENDTABLE(Person)

TEST(CppDb, BasicOperations)
{

    std::string combined_select = SELECT(Person.Id, Person.Name, Person.Age, Address.StreetName) +
                    FROM ( Person, Address) +
                    WHERE( Person.Id != 23 AND Person.AddressId == Address.Id );
    std::string p_create = Person.create();
    std::string insert = Person( Person.Age, Person.Name ).insert( 14, std::string("John Doe") );
    std::string prep_insert = Person( Person.Age, Person.Name ).prepare_insert( );

    ASSERT_STREQ("SELECT Person.Id, Person.Name, Person.Age, Address.StreetName FROM Person, Address WHERE ((Person.Id<>23) AND (Person.AddressId=Address.Id))", combined_select.c_str() );
    ASSERT_STREQ("CREATE TABLE IF NOT EXISTS Person (Id INTEGER PRIMARY KEY, AddressId INTEGER NOT NULL DEFAULT 1, Name VARCHAR(256), Age INTEGER, Birthday TIMESTAMP DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY (AddressId) REFERENCES Address(Id))", p_create.c_str());
    ASSERT_STREQ("INSERT OR IGNORE INTO Person(Age, Name) VALUES (14, \"John Doe\")", insert.c_str() );
    ASSERT_STREQ("INSERT OR IGNORE INTO Person(Age, Name) VALUES (:v1, :v2)", prep_insert.c_str() );

    std::string ordered_select = SELECT(Person.Id, Person.Name, Person.Age, Address.StreetName) +
            FROM ( Person, Address) +
            WHERE( Person.Id != 23 AND Person.AddressId == Address.Id ) + ORDER_BY(Person.Name DESC, Person.Age ASC);

    std::cout << ordered_select << std::endl;

    std::string ordered_select2 = SELECT(Person.Id, Person.Name, Person.Age, Address.StreetName) +
            FROM ( Person, Address) +
            WHERE( Person.Id != 23 AND Person.AddressId == Address.Id ) + ORDER_BY(Person.Name DESC, Person.Age);

    std::cout << ordered_select2;
}

TEST(CppDb, Delete)
{
    std::string s = DELETE + FROM (Person) + WHERE (Person.Id == 23);
    std::cout << s;
}


TEST(XSSSanitizer, RemoveIp)
{
    std::string s = "THIS HAS AN IP192.168.1.1ABC";
    std::string s2 = "THIS HAS AN IP 192.168.1.1.ABC";
    std::string sanit = unafrog::utils::sanitize_user_input(s);
    std::string sanit2 = unafrog::utils::sanitize_user_input(s2);
    ASSERT_STREQ(sanit.c_str(), "THIS HAS AN IPABC");
    ASSERT_STREQ(sanit2.c_str(), "THIS HAS AN IP 192.168.1.1.ABC");
}

TEST(ComponentBreaker, cb1)
{
    url_breaker a("/A/B/C", "/alpha/beta/gamma");
    ASSERT_EQ( (a["A"] == "alpha"), true);
    ASSERT_EQ( (a["B"] == "beta"), true);
    ASSERT_EQ( (a["C"] == "gamma"), true);
}


TEST(B62, b62_enc)
{
    std::string s = "5zn2cg3h";

    uint64_t l = 19617184805931;
    std::string g = unafrog::utils::b62::base62_encode(l);

    ASSERT_EQ(s, g);
}

TEST(Convert, HexStringToNr)
{
    ASSERT_EQ(9, unafrog::utils::hex_string_to_nr<int>("09"));
    ASSERT_EQ(0, unafrog::utils::hex_string_to_nr<int>("00"));
    ASSERT_EQ(10, unafrog::utils::hex_string_to_nr<int>("0A"));
    ASSERT_EQ(1987, unafrog::utils::hex_string_to_nr<int>("07c3"));
}


TEST(UrlMaker, args)
{
    std::string s = unafrog::utils::make_url("a", "b", "c");
    std::string s1 = unafrog::utils::make_url("a");
    std::string s2 = unafrog::utils::make_url("a", "b");

    ASSERT_EQ(s, "a/b/c");
    ASSERT_EQ(s1, "a");
    ASSERT_EQ(s2, "a/b");
}

TEST(Conversion, IntToHex)
{
    ASSERT_EQ("0f", unafrog::utils::int_to_hex<char>(15));
}


TEST(Common, DuplicateRemover)
{
    ASSERT_EQ("ABC/DEF", remove_duplicates("ABC//DEF", '/'));
    ASSERT_EQ("ABCC/DEF", remove_duplicates("ABCC/DEF", '/'));
    ASSERT_EQ("/", remove_duplicates("//", '/'));
}


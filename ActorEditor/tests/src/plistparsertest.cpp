#include "include/plistparsertest.h"
#include "include/fileutils.h"
#include "include/plistparser.h"

PlistParserTest::PlistParserTest()
{
}

void PlistParserTest::init()
{
    m_plistPath = FileUtils::getAbsoluteFilePath("data/ailisi_2.plist");
}

void PlistParserTest::cleanup()
{
}

void PlistParserTest::testLoadReturnBoolean()
{
    PlistParser parser;
    QCOMPARE(parser.load(m_plistPath), true);
}

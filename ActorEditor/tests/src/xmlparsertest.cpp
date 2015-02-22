#include "include/xmlparsertest.h"
#include "include/fileutils.h"
#include "include/xmlparser.h"

XMLParserTest::XMLParserTest()
{
}

void XMLParserTest::init()
{
    m_xmlFilePath = FileUtils::getAbsoluteFilePath("data/ailun_2.xml");
}

void XMLParserTest::cleanup()
{
}

void XMLParserTest::testParserReturnBoolean()
{
    XMLParser parser;
    QCOMPARE(parser.parser(m_xmlFilePath+"xadfas22ss2_saf.xml", "ailun_2", 0.5), false);
    QVERIFY(parser.getXmlString().length() == 0);

    QCOMPARE(parser.parser(m_xmlFilePath, "ailun_2", 0.5), true);
    QVERIFY(parser.getXmlString().length() > 0);
}

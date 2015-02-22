#include <QTest>
#include "include/fileutilstest.h"
#include "include/publishertest.h"
#include "include/imagetest.h"
#include "include/plistparsertest.h"
#include "include/xmlparsertest.h"

int main(int argc, char *argv[])
{
    FileUtilsTest fileUtils;
    PublisherTest Publisher;
    ImageTest image;
    PlistParserTest plistParser;
    XMLParserTest xmlParser;

    QTest::qExec(&fileUtils, argc, argv);
    QTest::qExec(&Publisher, argc, argv);
    QTest::qExec(&image, argc, argv);
    QTest::qExec(&plistParser, argc, argv);
    QTest::qExec(&xmlParser, argc, argv);

    return 0;
}

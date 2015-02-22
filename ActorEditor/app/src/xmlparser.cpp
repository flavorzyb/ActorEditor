#include <QtCore>
#include <QFile>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>

#include "include/xmlparser.h"

XMLParser::XMLParser():m_xmlString("")
{
}

XMLParser::~XMLParser()
{
}

static bool loadToDocument(const QString &plistPath, QDomDocument * doc)
{
    QFile file(plistPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    if (!doc->setContent(&file))
    {
        file.close();
        return false;
    }

    file.close();

    return true;
}

bool XMLParser::parser(const QString &xmlFilePath, const QString &prefixPath, double scaled)
{
    m_xmlString.clear();
    QDomDocument doc;
    if (!loadToDocument(xmlFilePath, &doc))
    {
        qDebug()<<"error: load xml("<< xmlFilePath <<")fail";
        return false;
    }

    QDomElement element = doc.documentElement();

    if (!element.hasChildNodes())
    {
        qDebug()<<"error: error xml content (" << xmlFilePath <<") file.";
        return false;
    }

    if (!parseElement(&element, prefixPath, scaled))
    {
        qDebug()<<"error: parse Element (" << element.tagName() <<")";
        return false;
    }

    m_xmlString = doc.toString();
    return true;
}

QString XMLParser::getXmlString() const
{
    return m_xmlString;
}

bool XMLParser::parseElement(QDomElement *element, const QString & prefixPath, double scaled)
{
    if (NULL == element)
    {
        qDebug()<<"error: element is null";
        return false;
    }

    if (element->hasAttribute("name"))
    {
        if ((element->tagName().toLower() == "skeleton")
                || (element->tagName().toLower() == "armature")
                || (element->tagName().toLower() == "textureatlas"))
        {
            element->setAttribute("name", prefixPath);
        }
        else if ((element->tagName().toLower() == "d")
                 || (element->tagName().toLower() == "subtexture"))
        {
            element->setAttribute("name", prefixPath + "/" + element->attribute("name"));
        }
    }
    //处理缩放
    if ((element->tagName().toLower() == "f")
            && (element->parentNode().toElement().tagName().toLower() == "b"))
    {
        scaledFrame(element, "x", scaled);
        scaledFrame(element, "y", scaled);
        scaledFrame(element, "cocos2d_x", scaled);
        scaledFrame(element, "cocos2d_y", scaled);
    }

    QDomNodeList nodeList = element->childNodes();
    for (int i = 0; i < nodeList.length(); i++)
    {
        QDomNode node = nodeList.at(i);
        QDomElement el = node.toElement();
        parseElement(&el, prefixPath, scaled);
    }

    return true;
}

void XMLParser::scaledFrame(QDomElement *element, const QString &name, double scaled)
{
    if (element->hasAttribute(name))
    {
        float value =  element->attribute(name).toFloat();
        char strValue[256] = {0};
        sprintf(strValue, "%.3f", value * scaled);
        element->setAttribute(name, strValue);
    }
}

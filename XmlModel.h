#pragma once

#include "XmlReader.h"

#include <memory>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

struct XmlAttribute
{
    std::string name;
    std::string value;
};

struct XmlNode
{
    std::string name;
    std::string text;
    std::vector<XmlAttribute> attributes;
    std::vector<std::unique_ptr<XmlNode>> children;

    const XmlAttribute* FindAttribute(const std::string& attributeName) const;
    const XmlNode* FindChild(const std::string& childName) const;
};

class XmlModel : public XmlReader
{
public:
    bool Load(const std::filesystem::path& path) { m_root.reset(); m_stack.clear(); return Read(path); }
    const XmlNode* Root() const { return m_root.get(); }

protected:
    bool OnDocumentBegin() override;
    bool OnElementBegin(const std::string& name) override;
    bool OnElementEnd(const std::string& name) override;
    bool OnAttribute(const std::string& name, const std::string& value) override;
    bool OnCharacters(const std::string& value) override;
    bool OnCData(const std::string& value) override;

    std::unique_ptr<XmlNode> m_root;


private:

    std::vector<XmlNode*> m_stack;
};

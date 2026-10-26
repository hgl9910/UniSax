#include "XmlModel.h"

const XmlAttribute* XmlNode::FindAttribute(const std::string& attributeName) const
{
    for (const auto& attribute : attributes)
        if (attribute.name == attributeName) return &attribute;
    return nullptr;
}

const XmlNode* XmlNode::FindChild(const std::string& childName) const
{
    for (const auto& child : children)
        if (child->name == childName) return child.get();
    return nullptr;
}

bool XmlModel::OnDocumentBegin()
{
    m_root.reset();
    m_stack.clear();
    return true;
}

bool XmlModel::OnElementBegin(const std::string& name)
{
    auto node = std::make_unique<XmlNode>();
    node->name = name;
    XmlNode* raw = node.get();

    if (m_stack.empty())
    {
        if (m_root) return false; // XML document must have one document element.
        m_root = std::move(node);
    }
    else
    {
        m_stack.back()->children.push_back(std::move(node));
    }
    m_stack.push_back(raw);
    return true;
}

bool XmlModel::OnElementEnd(const std::string& name)
{
    if (m_stack.empty() || m_stack.back()->name != name) return false;
    m_stack.pop_back();
    return true;
}

bool XmlModel::OnAttribute(const std::string& name, const std::string& value)
{
    if (m_stack.empty()) return false;
    m_stack.back()->attributes.push_back({ name, value });
    return true;
}

bool XmlModel::OnCharacters(const std::string& value)
{
    if (m_stack.empty())
    {
        for (char c : value)
            if (c != ' ' && c != '\t' && c != '\r' && c != '\n') return false;
        return true;
    }
    m_stack.back()->text += value;
    return true;
}

bool XmlModel::OnCData(const std::string& value)
{
    if (m_stack.empty()) return false;
    m_stack.back()->text += value;
    return true;
}

#pragma once

#include "JsonReader.h"
#include "GuiAccess.h"
#include "IGuiAccess.h"

#include <string>
#include <vector>
#include <filesystem>

class JsonModel : public JsonReader
{
public:
    JsonModel(
        IGuiAccess& gui,
        const std::vector<GuiAccess>& items);

    bool Save(std::filesystem::path path);
    bool Load(std::filesystem::path path);

protected:
    bool OnArrayBegin(const std::string& name) override;
    bool OnArrayEnd() override;

    bool OnString(
        const std::string& name,
        const std::string& value) override;

    bool OnNumber(
        const std::string& name,
        const std::string& value) override;

    bool OnBoolean(
        const std::string& name,
        bool value) override;

private:
    bool IsGuiControl(
        int dialogID,
        int controlID) const;

private:
    IGuiAccess& m_gui;
    const std::vector<GuiAccess>& m_items;

    int m_currentDialogID = -1;

    bool m_inDialogs = false;
    bool m_inControls = false;
};


#include "JsonFormatter.h"
#include "JsonWriter.h"
#include "JsonModel.h"
#include <filesystem>

JsonModel::JsonModel(
    IGuiAccess& gui,
    const std::vector<GuiAccess>& items)
    : m_gui(gui),
      m_items(items)
{
}


// -----------------------------------------------------------------------------
// Save
// -----------------------------------------------------------------------------

bool JsonModel::Save(std::filesystem::path path)
{
    JsonWriter writer;

    writer.Write( path);

    writer.BeginDocument();
    writer.BeginObject();
    writer.BeginArray("dialogs");

    int currentDialogID = -1;

    for (const auto& item : m_items)
    {
        // Neuer Dialog
        if (item.DialogID != currentDialogID)
        {
            // alten Dialog schließen
            if (currentDialogID != -1)
            {
                writer.EndArray();   // controls
                writer.EndObject();  // dialog
            }

            currentDialogID = item.DialogID;

            writer.BeginObject();

            writer.WriteField(
                "id",
                std::to_string(currentDialogID));

            writer.BeginArray("controls");
        }

        // Wert aus der GUI holen
        const std::string value =
            m_gui.GetValue(
                item.DialogID,
                item.ControlID);

        // Control schreiben
        writer.BeginObject();

        writer.WriteField(
            std::to_string(item.ControlID),
            value);

        writer.EndObject();
    }

    // letzten Dialog schließen
    if (currentDialogID != -1)
    {
        writer.EndArray();   // controls
        writer.EndObject();  // dialog
    }

    writer.EndArray();       // dialogs
    writer.EndObject();
    writer.EndDocument();

    return true;
}


// -----------------------------------------------------------------------------
// Load
// -----------------------------------------------------------------------------

bool JsonModel::Load(const std::filesystem::path path)
{
    m_currentDialogID = -1;
    m_inDialogs = false;
    m_inControls = false;

    return Read(path);
}


// -----------------------------------------------------------------------------
// JSON callbacks
// -----------------------------------------------------------------------------

bool JsonModel::OnArrayBegin(const std::string& name)
{
    if (name == "dialogs")
    {
        m_inDialogs = true;
        return true;
    }

    if (name == "controls")
    {
        m_inControls = true;
        return true;
    }

    return true;
}


bool JsonModel::OnArrayEnd()
{
    if (m_inControls)
    {
        m_inControls = false;
        return true;
    }

    if (m_inDialogs)
    {
        m_inDialogs = false;
        return true;
    }

    return true;
}


bool JsonModel::OnString(
    const std::string& name,
    const std::string& value)
{
    // Dialog-ID
    if (m_inDialogs &&
        !m_inControls &&
        name == "id")
    {
        m_currentDialogID = std::stoi(value);
        return true;
    }

    // Control-ID + Wert
    if (m_inControls)
    {
        const int controlID = std::stoi(name);

        if (!IsGuiControl(
                m_currentDialogID,
                controlID))
        {
            return true;
        }

        return m_gui.SetValue(
            m_currentDialogID,
            controlID,
            value);
    }

    return true;
}


bool JsonModel::OnNumber(
    const std::string& name,
    const std::string& value)
{
    // Momentan werden die Controls als String gespeichert.
    // Falls später numerische JSON-Werte unterstützt werden,
    // kann hier dieselbe Zuordnung erfolgen.

    if (!m_inControls)
        return true;

    const int controlID = std::stoi(name);

    if (!IsGuiControl(
            m_currentDialogID,
            controlID))
    {
        return true;
    }

    return m_gui.SetValue(
        m_currentDialogID,
        controlID,
        value);
}


bool JsonModel::OnBoolean(
    const std::string& name,
    bool value)
{
    if (!m_inControls)
        return true;

    const int controlID = std::stoi(name);

    if (!IsGuiControl(
            m_currentDialogID,
            controlID))
    {
        return true;
    }

    return m_gui.SetValue(
        m_currentDialogID,
        controlID,
        value ? "true" : "false");
}


// -----------------------------------------------------------------------------
// Mapping
// -----------------------------------------------------------------------------

bool JsonModel::IsGuiControl(
    int dialogID,
    int controlID) const
{
    for (const auto& item : m_items)
    {
        if (item.DialogID == dialogID &&
            item.ControlID == controlID)
        {
            return true;
        }
    }

    return false;
}
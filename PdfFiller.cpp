#include "stdafx.h"
#include "PdfFiller.h"

#include <atltrace.h>
#include <podofo/podofo.h>

using namespace PoDoFo;

bool PdfFiller::Fill(
    const std::filesystem::path& inputPath,
    const std::filesystem::path& outputPath,
    const PdfFillList& fills)
{
    PdfMemDocument document;

    try
    {
        document.Load(inputPath.string());
    }
    catch (const PdfError& e)
    {
        ATLTRACE("PDF konnte nicht geladen werden: %s\n",
            inputPath.string().c_str());
        ATLTRACE("PdfError name: %.*s\n",
            static_cast<int>(e.GetName().size()),
            e.GetName().data());
        ATLTRACE("PdfError what: %s\n", e.what());
        return false;
    }

    PdfAcroForm* acroForm = document.GetAcroForm();
    if (acroForm == nullptr)
    {
        ATLTRACE("PDF besitzt kein AcroForm.\n");
        return false;
    }

    for (const auto& fill : fills)
    {
        const auto& access = fill.pdf;

        if (access.AcroFormIndex >= acroForm->GetFieldCount())
        {
            ATLTRACE("Ungueltiger AcroFormIndex: %u\n",
                access.AcroFormIndex);
            return false;
        }

        PdfField& field = acroForm->GetFieldAt(access.AcroFormIndex);
        const auto fieldName = field.GetFullName();

        if (!access.FieldName.empty() && fieldName != access.FieldName)
        {
            ATLTRACE("Feldname stimmt nicht: Index=%u, erwartet '%s'\n",
                access.AcroFormIndex,
                access.FieldName.c_str());
            return false;
        }

        switch (field.GetType())
        {
        case PdfFieldType::TextBox:
        {
            PdfTextBox& textBox = static_cast<PdfTextBox&>(field);
            textBox.SetText(PdfString(fill.value));
            break;
        }
        default:
            ATLTRACE("Nicht unterstuetzter Feldtyp: Index=%u\n",
                access.AcroFormIndex);
            return false;
        }
    }

    // Erforderlich, damit PDF-Viewer die Feldwerte sichtbar darstellen.
    acroForm->SetNeedAppearances(true);

    try
    {
        document.Save(outputPath.string());
    }
    catch (const PdfError& e)
    {
        ATLTRACE("PDF konnte nicht gespeichert werden: %s\n",
            outputPath.string().c_str());
        ATLTRACE("PdfError name: %.*s\n",
            static_cast<int>(e.GetName().size()),
            e.GetName().data());
        ATLTRACE("PdfError what: %s\n", e.what());
        return false;
    }

    return true;
}

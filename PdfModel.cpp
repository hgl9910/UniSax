#include "PdfModel.h"


PdfModel::PdfModel(IGuiAccess& gui) : m_gui(gui){};


bool PdfModel::Fill(const std::vector<PdfMap>& mapper, std::filesystem::path inputPath, std::filesystem::path outputPath)
{
        PdfFillList fills;

        for (const auto& map : mapper)
        {
            fills.push_back(
                {
                    map.pdf,
                    m_gui.GetValue(
                        map.gui.DialogID,
                        map.gui.ControlID)
                });
        }

        PdfFiller filler;

        return filler.Fill(inputPath, outputPath, fills);

        return 0;

}

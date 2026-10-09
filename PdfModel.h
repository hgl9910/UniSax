#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "IGuiAccess.h"
#include "PdfMapper.h"
#include "PdfFiller.h"
#include <filesystem>


class PdfModel
{
public:
    PdfModel(IGuiAccess& gui);
        
    bool Fill(const std::vector<PdfMap>& mapper, std::filesystem::path inputPath, std::filesystem::path outputPath);

private:
    IGuiAccess& m_gui;
 
};

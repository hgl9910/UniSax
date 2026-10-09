#pragma once

#include <filesystem>

#include "PdfFill.h"

class PdfFiller
{
public:
    bool Fill(
        const std::filesystem::path& inputPath,
        const std::filesystem::path& outputPath,
        const PdfFillList& fills);
};

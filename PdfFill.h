#pragma once


#include "PdfAccess.h"
#include <string>
#include <vector>

struct PdfFill
{
    PdfAccess pdf;
    std::string value;
};

using PdfFillList = std::vector<PdfFill>;
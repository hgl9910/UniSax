
#include "PdfMapper.h"
#include "resource.h"

const std::vector<PdfMap> MapDocument1 =
{
	// Dialog 1
	{
		{ IDD_DIALOG1, IDED_D1FA_NAME },
		{ 80, "Textfeld 12"   }
	},
	{
		{ IDD_DIALOG1, IDED_D1FA_WORT },
		{ 81, "Textfeld 12_2" },
	},
	{
		{ IDD_DIALOG1,IDED_D1FA_STR },
		{ 82, "Textfeld 12_3" },
	},

	// Dialog 2
	{
		{ IDD_DIALOG2,IDED_D2VS_NAME},
		{  0, "Textfeld 1"   },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_WORT},
		{ 24, "Textfeld 1_4" },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_STR},
		{ 23, "Textfeld 1_3" },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_GNAME},
		{ 22, "Textfeld 1_2" },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_GORT},
		{ 29, "Textfeld 1_6" },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_SORT},
		{ 25, "Textfeld 1_5" },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_GBDAT},
		{  1, "Textfeld 2"   },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_STDAT},
		{ 26, "Textfeld 2_2" },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_ALT  },
		{  2, "Textfeld 3"   },
	},
	{
		{ IDD_DIALOG2,IDED_D2VS_KONF},
		{ 21, "Textfeld 4"   },
	},

	// Dialog 3

	{
		{ IDD_DIALOG3,IDED_D3AG_NAME },
		{ 73, "Textfeld 1_7" },
	},
	{
		{ IDD_DIALOG3,IDED_D3AG_WORT },
		{ 77, "Textfeld 1_11"},
	},
	{
		{ IDD_DIALOG3,IDED_D3AG_STR  },
		{ 76, "Textfeld 1_10"},
	},
	{
		{ IDD_DIALOG3,IDED_D3AG_GNAME},
		{ 74, "Textfeld 1_8" },
	},
	{
		{ IDD_DIALOG3,IDED_D3AG_TEL  },
		{ 70, "Textfeld 9"   },
	},
	{
		{ IDD_DIALOG3,IDED_D3AG_MAIL },
		{ 71, "Textfeld 10"  }

	}
};

// text Filler Document 2


const std::vector<PdfMap> MapDocument2 =
{

	// Dialog 4
	{
		{ IDD_DIALOG4,IDED_D4AB_NAME },
		{  4, "Textfeld 1"   },
	},
	{
		{ IDD_DIALOG4,IDED_D4AB_WORT },
		{ 11, "Textfeld 1_3" },
	},
	{
		{ IDD_DIALOG4,IDED_D4AB_STR  },
		{ 10, "Textfeld 1_2" },
	},
	{
		{ IDD_DIALOG4,IDED_D4AB_GNAME},
		{  5, "Textfeld 2"   },
	},
	{
		{ IDD_DIALOG4,IDED_D4AB_TEL  },
		{  7, "Textfeld 6"   },
	},
	{
		{ IDD_DIALOG4,IDED_D4AB_MAIL },
		{ 13, "Textfeld 2_2" },
	},
	{
		{ IDD_DIALOG4,IDED_D4NB_NAME },
		{ 12, "Textfeld 1_4" },
	},
	{
		{ IDD_DIALOG4,IDED_D4NB_WORT },
		{ 17, "Textfeld 1_6" },
	},
	{
		{ IDD_DIALOG4,IDED_D4NB_STR  },
		{ 16, "Textfeld 1_5" },
	},
	{
		{ IDD_DIALOG4,IDED_D4NB_GNAME},
		{ 14, "Textfeld 2_3" },
	},
	{
		{ IDD_DIALOG4,IDED_D4NB_TEL  },
			{ 19, "Textfeld 6_2" },
	},
	{
			{ IDD_DIALOG4,IDED_D4NB_MAIL },
			{ 18, "Textfeld 2_4" },
	}
};

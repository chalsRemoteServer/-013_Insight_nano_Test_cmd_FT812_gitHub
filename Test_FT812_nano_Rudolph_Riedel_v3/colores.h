#ifndef COLORES_H
#define COLORES_H


/*
// === COLORES BÁSICOS (BGR) ===
#define BLACK           0x000000
#define WHITE           0xFFFFFF
#define RED             0xFF0000    // RGB 0x0000FF → BGR 0xFF0000
#define GREEN           0x00FF00
#define BLUE            0x0000FF    // RGB 0xFF0000 → BGR 0x0000FF
#define CYAN            0xFFFF00
#define MAGENTA         0xFF00FF
#define YELLOW          0x00FFFF
#define GRAY            0x808080
#define LIGHT_GRAY      0xD3D3D3
#define DARK_GRAY       0x404040

// === GRISES ===
#define WHITE_GRAY      0xF8F8FF
#define SILVER_GRAY     0xC0C0C0
#define MEDIUM_GRAY     0xA9A9A9
#define DARK_GRAY       0x696969
#define CHARCOAL_GRAY   0x333333
#define BLACK_GRAY      0x1A1A1A

// === ROJOS ===
#define LIGHT_RED       0x6666FF    // RGB 0xFF6666 → BGR 0x6666FF
#define DARK_RED        0x00008B    // RGB 0x8B0000 → BGR 0x00008B
#define CRIMSON         0x3C14DC    // RGB 0xDC143C → BGR 0x3C14DC
#define SALMON          0x7280FA    // RGB 0xFA8072 → BGR 0x7280FA
#define INDIAN_RED      0x5C5CCD    // RGB 0xCD5C5C → BGR 0x5C5CCD
#define FIREBRICK       0x2222B2    // RGB 0xB22222 → BGR 0x2222B2

// === VERDES ===
#define LIGHT_GREEN     0x90EE90
#define DARK_GREEN      0x006400
#define FOREST_GREEN    0x228B22
#define LIME_GREEN      0x32CD32
#define OLIVE           0x008080
#define MINT_GREEN      0x98FF98

// === AZULES ===
#define LIGHT_BLUE      0xE6D8AD    // RGB 0xADD8E6 → BGR 0xE6D8AD
#define DARK_BLUE       0x8B0000    // RGB 0x00008B → BGR 0x8B0000
#define ROYAL_BLUE      0xE16941    // RGB 0x4169E1 → BGR 0xE16941
#define SKY_BLUE        0xEBCE87    // RGB 0x87CEEB → BGR 0xEBCE87
#define NAVY_BLUE       0x800000    // RGB 0x000080 → BGR 0x800000
#define STEEL_BLUE      0xB48246    // RGB 0x4682B4 → BGR 0xB48246

// === AMARILLOS ===
#define LIGHT_YELLOW    0xE0FFFF    // RGB 0xFFFFE0 → BGR 0xE0FFFF
#define DARK_YELLOW     0x0C879B    // RGB 0x9B870C → BGR 0x0C879B
#define GOLD            0x00D7FF    // RGB 0xFFD700 → BGR 0x00D7FF
#define KHAKI           0x8CE6F0    // RGB 0xF0E68C → BGR 0x8CE6F0
#define MUSTARD         0x58DBFF    // RGB 0xFFDB58 → BGR 0x58DBFF

// === NARANJAS ===
#define LIGHT_ORANGE    0xB9DAFF    // RGB 0xFFDAB9 → BGR 0xB9DAFF
#define DARK_ORANGE     0x008CFF    // RGB 0xFF8C00 → BGR 0x008CFF
#define ORANGE          0x00A5FF    // RGB 0xFFA500 → BGR 0x00A5FF
#define CORAL           0x507FFF    // RGB 0xFF7F50 → BGR 0x507FFF
#define TOMATO          0x4763FF    // RGB 0xFF6347 → BGR 0x4763FF

// === ROSADOS ===
#define LIGHT_PINK      0xC1B6FF    // RGB 0xFFB6C1 → BGR 0xC1B6FF
#define DARK_PINK       0x8515C7    // RGB 0xC71585 → BGR 0x8515C7
#define PINK            0xCBC0FF    // RGB 0xFFC0CB → BGR 0xCBC0FF
#define HOT_PINK        0xB469FF    // RGB 0xFF69B4 → BGR 0xB469FF
#define DEEP_PINK       0x9314FF    // RGB 0xFF1493 → BGR 0x9314FF

// === PÚRPURAS ===
#define LIGHT_PURPLE    0xFAE6E6    // RGB 0xE6E6FA → BGR 0xFAE6E6
#define DARK_PURPLE     0x82004B    // RGB 0x4B0082 → BGR 0x82004B
#define PURPLE          0x800080
#define LAVENDER        0xDC7EB5    // RGB 0xB57EDC → BGR 0xDC7EB5
#define VIOLET          0xE22B8A    // RGB 0x8A2BE2 → BGR 0xE22B8A

// === MARRONES / TIERRA ===
#define BROWN           0x2A2AA5    // RGB 0xA52A2A → BGR 0x2A2AA5
#define LIGHT_BROWN     0x3F85CD    // RGB 0xCD853F → BGR 0x3F85CD
#define DARK_BROWN      0x214365    // RGB 0x654321 → BGR 0x214365
#define SADDLE_BROWN    0x13458B    // RGB 0x8B4513 → BGR 0x13458B
#define SIENNA          0x2D52A0    // RGB 0xA0522D → BGR 0x2D52A0
#define TAN             0x8CB4D2    // RGB 0xD2B48C → BGR 0x8CB4D2
#define SAND            0xB2B2C2    // RGB 0xC2B2B2 → BGR 0xB2B2C2

// === TONOS DE PIEL ===
#define SKIN_LIGHT      0xBDE0FF    // RGB 0xFFE0BD → BGR 0xBDE0FF
#define SKIN_MEDIUM     0x94CDFF    // RGB 0xFFCD94 → BGR 0x94CDFF
#define SKIN_TAN        0x86C0EA    // RGB 0xEAC086 → BGR 0x86C0EA
#define SKIN_BROWN      0x24558D    // RGB 0x8D5524 → BGR 0x24558D
#define SKIN_DARK       0x33405C    // RGB 0x5C4033 → BGR 0x33405C

// === TONOS PASTEL ===
#define PASTEL_RED      0x6169FF    // RGB 0xFF6961 → BGR 0x6169FF
#define PASTEL_GREEN    0x77DD77
#define PASTEL_BLUE     0xCFC6AE    // RGB 0xAEC6CF → BGR 0xCFC6AE
#define PASTEL_YELLOW   0xB3FFFF    // RGB 0xFFFFB3 → BGR 0xB3FFFF
#define PASTEL_PURPLE   0xCBACCB    // RGB 0xCBACCB → BGR 0xCBACCB (no cambia)
#define PASTEL_ORANGE   0x47B3FF    // RGB 0xFFB347 → BGR 0x47B3FF
#define PASTEL_PINK     0xDCD1FF    // RGB 0xFFD1DC → BGR 0xDCD1FF
#define PASTEL_TEAL     0xE6E699    // RGB 0x99E6E6 → BGR 0xE6E699

// === ESPECIALES ===
#define TRANSPARENT     0x000000
#define SILVER          0xC0C0C0
#define GOLDENROD       0x2050DA    // RGB 0xDAA520 → BGR 0x2050DA
#define TEAL            0x808000    // RGB 0x008080 → BGR 0x808000
#define INDIGO          0x82004B    // RGB 0x4B0082 → BGR 0x82004B
#define MAROON          0x000080    // RGB 0x800000 → BGR 0x000080
#define BEIGE           0xDCF5F5    // RGB 0xF5F5DC → BGR 0xDCF5F5
#define IVORY           0xF0FFFF    // RGB 0xFFFFF0 → BGR 0xF0FFFF


*/

// === COLORES BÁSICOS ===
#define BLACK           0x000000
#define WHITE           0xFFFFFF
#define RED             0x0000FF
#define GREEN           0x00FF00
#define BLUE            0xFF0000
#define CYAN            0xFFFF00
#define MAGENTA         0xFF00FF
#define YELLOW          0x00FFFF
#define GRAY            0x808080
#define LIGHT_GRAY      0xD3D3D3
#define DARK_GRAY       0x404040

// === GRISES ===
#define WHITE_GRAY      0xF8F8FF
#define SILVER_GRAY     0xC0C0C0
#define MEDIUM_GRAY     0xA9A9A9
#define DARK_GRAY       0x696969
#define CHARCOAL_GRAY   0x333333
#define BLACK_GRAY      0x1A1A1A

// === ROJOS ===
#define LIGHT_RED       0x6666FF
#define DARK_RED        0x00008B
#define CRIMSON         0x3C14DC
#define SALMON          0x7280FA
#define INDIAN_RED      0x5C5CCD
#define FIREBRICK       0x2222B2

// === VERDES ===
#define LIGHT_GREEN     0x90EE90
#define DARK_GREEN      0x006400
#define FOREST_GREEN    0x228B22
#define LIME_GREEN      0x32CD32
#define OLIVE           0x008080
#define MINT_GREEN      0x98FF98

// === AZULES ===
#define LIGHT_BLUE      0xE6D8AD
#define DARK_BLUE       0x8B0000
#define ROYAL_BLUE      0xE16941
#define SKY_BLUE        0xEBCE87
#define NAVY_BLUE       0x800000
#define STEEL_BLUE      0xB48246

// === AMARILLOS ===
#define LIGHT_YELLOW    0xE0FFFF
#define DARK_YELLOW     0x0C879B
#define GOLD            0x00D7FF
#define KHAKI           0x8CE6F0
#define MUSTARD         0x58DBFF

// === NARANJAS ===
#define LIGHT_ORANGE    0xB9DAFF
#define DARK_ORANGE     0x008CFF
#define ORANGE          0x00A5FF
#define CORAL           0x507FFF
#define TOMATO          0x4763FF

// === ROSADOS ===
#define LIGHT_PINK      0xC1B6FF
#define DARK_PINK       0x8515C7
#define PINK            0xCBC0FF
#define HOT_PINK        0xB469FF
#define DEEP_PINK       0x9314FF

// === PÚRPURAS ===
#define LIGHT_PURPLE    0xFAE6E6
#define DARK_PURPLE     0x82004B
#define PURPLE          0x800080
#define LAVENDER        0xDC7EB5
#define VIOLET          0xE22B8A

// === MARRONES / TIERRA ===
#define BROWN           0x2A2AA5
#define LIGHT_BROWN     0x3F85CD
#define DARK_BROWN      0x214365
#define SADDLE_BROWN    0x13458B
#define SIENNA          0x2D52A0
#define TAN             0x8CB4D2
#define SAND            0xB2B2C2

// === TONOS DE PIEL ===
#define SKIN_LIGHT      0xBDE0FF
#define SKIN_MEDIUM     0x94CDFF
#define SKIN_TAN        0x86C0EA
#define SKIN_BROWN      0x24558D
#define SKIN_DARK       0x33405C

// === TONOS PASTEL ===
#define PASTEL_RED      0x6169FF
#define PASTEL_GREEN    0x77DD77
#define PASTEL_BLUE     0xCFC6AE
#define PASTEL_YELLOW   0xB3FFFF
#define PASTEL_PURPLE   0xCBACCB
#define PASTEL_ORANGE   0x47B3FF
#define PASTEL_PINK     0xDCD1FF
#define PASTEL_TEAL     0xE6E699

// === ESPECIALES ===
#define TRANSPARENT     0x000000
#define SILVER          0xC0C0C0
#define GOLDENROD       0x2050DA
#define TEAL            0x808000
#define INDIGO          0x82004B
#define MAROON          0x000080
#define BEIGE           0xDCF5F5
#define IVORY           0xF0FFFF

#endif  // COLORES_H

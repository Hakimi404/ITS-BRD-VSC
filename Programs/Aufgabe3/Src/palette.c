#include "palette.h"
#include "input.h"
#include "errorhandler.h"

int load_palette(RGBQUAD *pal, int count)
{
    for(int i = 0; i < count; i++)
    {
        if (1 != COMread((char*)&pal[i], sizeof(RGBQUAD), 1)) //Wenn COMread < 1 zurückgibt → Datenstrom abgebrochen → Fehler
            return NOK;
    }
    return EOK;
}


// BMP-Palette besteht aus 4 Bytes:
//             Byte 0 = Blue
//             Byte 1 = Green
//             Byte 2 = Red
//             Byte 3 = Reserved
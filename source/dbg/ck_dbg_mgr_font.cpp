/*
    Copyright (c) 2007-2010 Takashi Kitao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#include <stdarg.h>

#include "ck_dbg_all.h"

#include "ck_low_level_api.h"
#include "ck_private_macro.h"


static const u8 s_debug_font[96 * 8] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, // SP !
    0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x7c, 0x28, 0x28, 0x28, 0x7c, 0x28, 0x00, // " #
    0x38, 0x54, 0x50, 0x38, 0x14, 0x54, 0x38, 0x00, 0x40, 0xa4, 0x48, 0x10, 0x24, 0x4a, 0x04, 0x00, // $ %
    0x30, 0x48, 0x48, 0x30, 0x4a, 0x44, 0x3a, 0x00, 0x30, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, // & '
    0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x00, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x00, // ( )
    0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10, 0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00, 0x00, // * +
    0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, // , -
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, // . /
    0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38, 0x00, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, // 0 1
    0x38, 0x44, 0x04, 0x18, 0x20, 0x40, 0x7c, 0x00, 0x38, 0x44, 0x04, 0x38, 0x04, 0x44, 0x38, 0x00, // 2 3
    0x18, 0x28, 0x28, 0x48, 0x7c, 0x08, 0x08, 0x00, 0x7c, 0x40, 0x40, 0x78, 0x04, 0x44, 0x38, 0x00, // 4 5
    0x38, 0x44, 0x40, 0x78, 0x44, 0x44, 0x38, 0x00, 0x7c, 0x44, 0x44, 0x08, 0x08, 0x10, 0x10, 0x00, // 6 7
    0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, 0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x44, 0x38, 0x00, // 8 9
    0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x10, 0x20, 0x00, // : ;
    0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x00, 0x00, // < =
    0x40, 0x20, 0x10, 0x08, 0x10, 0x20, 0x40, 0x00, 0x38, 0x44, 0x44, 0x18, 0x10, 0x00, 0x10, 0x00, // > ?
    0x38, 0x44, 0x5c, 0x54, 0x5c, 0x40, 0x3c, 0x00, 0x38, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44, 0x00, // @ A
    0x78, 0x44, 0x44, 0x78, 0x44, 0x44, 0x78, 0x00, 0x38, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00, // B C
    0x70, 0x48, 0x44, 0x44, 0x44, 0x48, 0x70, 0x00, 0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7c, 0x00, // D E
    0x7c, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x00, 0x38, 0x44, 0x40, 0x5c, 0x44, 0x44, 0x38, 0x00, // F G
    0x44, 0x44, 0x44, 0x7c, 0x44, 0x44, 0x44, 0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, // H I
    0x1c, 0x08, 0x08, 0x08, 0x48, 0x48, 0x30, 0x00, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x00, // J K
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7c, 0x00, 0x44, 0x6c, 0x54, 0x44, 0x44, 0x44, 0x44, 0x00, // L M
    0x44, 0x44, 0x64, 0x54, 0x4c, 0x44, 0x44, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, // N O
    0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, 0x00, 0x38, 0x44, 0x44, 0x44, 0x54, 0x4c, 0x38, 0x00, // P Q
    0x78, 0x44, 0x44, 0x78, 0x50, 0x48, 0x44, 0x00, 0x38, 0x44, 0x40, 0x38, 0x04, 0x44, 0x38, 0x00, // R S
    0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, // T U
    0x44, 0x44, 0x44, 0x28, 0x28, 0x10, 0x10, 0x00, 0x54, 0x54, 0x54, 0x54, 0x54, 0x7c, 0x28, 0x00, // V W
    0x44, 0x44, 0x28, 0x10, 0x28, 0x44, 0x44, 0x00, 0x44, 0x44, 0x44, 0x28, 0x10, 0x10, 0x10, 0x00, // X Y
    0x7c, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7c, 0x00, 0x1c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1c, 0x00, // Z [
    0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70, 0x00, // \ ]
    0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, // ^ _
    0x00, 0x66, 0x7e, 0xbb, 0xef, 0xd7, 0x7e, 0x00, 0x00, 0x00, 0x38, 0x04, 0x3c, 0x44, 0x3c, 0x00, // ` a
    0x00, 0x40, 0x40, 0x78, 0x44, 0x44, 0x78, 0x00, 0x00, 0x00, 0x38, 0x44, 0x40, 0x44, 0x38, 0x00, // b c
    0x00, 0x04, 0x04, 0x3c, 0x44, 0x44, 0x3c, 0x00, 0x00, 0x00, 0x38, 0x44, 0x7c, 0x40, 0x38, 0x00, // d e
    0x00, 0x0c, 0x10, 0x3c, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x44, 0x3c, 0x04, 0x38, // f g
    0x00, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x00, 0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, // h i
    0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x30, 0x00, 0x20, 0x24, 0x28, 0x30, 0x28, 0x24, 0x00, // j k
    0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x78, 0x54, 0x54, 0x54, 0x54, 0x00, // l m
    0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, // n o
    0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x00, 0x00, 0x3c, 0x44, 0x44, 0x3c, 0x04, 0x04, // p q
    0x00, 0x00, 0x78, 0x44, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x3c, 0x40, 0x38, 0x04, 0x78, 0x00, // r s
    0x00, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x0c, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x3c, 0x00, // t u
    0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 0x00, 0x00, 0x44, 0x54, 0x54, 0x54, 0x28, 0x00, // v w
    0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x44, 0x44, 0x28, 0x10, 0x60, 0x00, // x y
    0x00, 0x00, 0x7c, 0x08, 0x10, 0x20, 0x7c, 0x00, 0x0c, 0x10, 0x10, 0x20, 0x10, 0x10, 0x0c, 0x00, // z {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x60, 0x10, 0x10, 0x08, 0x10, 0x10, 0x60, 0x00, // | }
    0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x7c, 0x7c, 0x7c, 0x7c, 0x7c, 0x7c, 0x00 // ~ BL
};


void ckDbgMgr::drawString(r32 left, r32 top, ckCol col, u8 scale, const char* str, ...)
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    if (scale == 0 || !str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    CK_VSPRINTF(buf, 256, str);

    ckSprt* font_sprt = &ins->m_font_sprt;

    r32 x = left + static_cast<r32>(DEBUG_FONT_WIDTH) * scale / 2.0f;
    r32 y = top - static_cast<r32>(DEBUG_FONT_HEIGHT) * scale / 2.0f;

    r32 sprt_size = 8.0f * scale;
    r32 font_width = static_cast<r32>(DEBUG_FONT_WIDTH) * scale;

    for (char* c = buf; *c != '\0'; c++)
    {
        if (font_sprt->getCurDataNum() == font_sprt->getMaxDataNum())
        {
            font_sprt->reallocData(font_sprt->getMaxDataNum() * 2);
        }

        u16 index = font_sprt->getCurDataNum();

        char ch = (*c >= 32 && *c <= 126) ? *c : 127;

        if (ch != 32)
        {
            r32 u1 = static_cast<r32>((ch - 32) % 16) / 16.0f;
            r32 v1 = static_cast<r32>((ch - 32) / 16) / 8.0f;
            r32 u2 = u1 + 1.0f / 16.0f;
            r32 v2 = v1 + 1.0f / 8.0f;

            font_sprt->dataPos(index).set(x, y, 0.0f);
            font_sprt->dataCol(index) = (ch == '`') ? ckCol(255, 128, 64, 255) : col;
            font_sprt->setDataSize(index, sprt_size, sprt_size);
            font_sprt->setDataUV(index, u1, v1, u2, v2);
            font_sprt->setCurDataNum(index + 1);
        }

        x += font_width;
    }
}


void ckDbgMgr::newDebugFontTexture()
{
    ckTex* debug_font_tex = ckDrawMgr::newTexture(DEBUG_FONT_TEXTURE_ID, 128, 64, ckTex::FORMAT_ALPHA);

    debug_font_tex->clearImage(ckCol::ZERO);

    u8* debug_font_image = static_cast<u8*>(debug_font_tex->beginEditImage());

    for (u32 i = 0; i < 96; i++)
    {
        u32 x = i % 16;
        u32 y = i / 16;

        for (u32 j = 0; j < 8; j++)
        {
            u8 font = s_debug_font[i * 8 + j];
            u8* dest = debug_font_image + x * 8 + y * (8 * 16 * 8) + j * (8 * 16);

            for (u32 k = 0; k < 8; k++)
            {
                *(dest + k) = (font & 0x80) ? 0xff : 0x00;
                font <<= 1;
            }
        }
    }

    debug_font_tex->endEditImage();
}

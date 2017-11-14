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


#include "catcake_main.h"


void newCatcakeLogo(const char** cmd, u8 cmd_num);
void newGolem();
void newJeep();


static const char* s_cmd[] =
{
    "L-BUTTON DRAG: ROTATE CAMERA", //
    "T: TOGGLE TEXTURE", //
    "L: TOGGLE LIGHT", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


ckMain()
{
    ckCreateCatcake("Sample11 - Model", 854, 480, 60);

    ckResMgr::loadResource("/sdcard/catcake/data/catcake_logo_71x14.png", true);
    ckResMgr::loadResource("/sdcard/catcake/data/copyright_304x34.png", true);
    ckResMgr::loadResource("/sdcard/catcake/data/golem.png", true);
    ckResMgr::loadResource("/sdcard/catcake/data/jeep.png", true);
    ckUtil::import3DSModel("/sdcard/catcake/data/golem.3ds", ckID_("golem.png"), true, true, 1.5f);
    ckUtil::import3DSModel("/sdcard/catcake/data/jeep.3ds", ckID_("jeep.png"), true, true, 0.5f);

    // newCatcakeLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newGolem();
    newJeep();

    ckStartCatcake();
    // ckDestroyCatcake();
}

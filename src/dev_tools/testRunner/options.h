#pragma once
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

#define API (1<<0)
#define CMD (1<<1)
#define BOTH (API | CMD)

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
public:
    int which = CMD;
    string_q speed_filter = "";
    bool quit_on_error = true;
    bool ignoreOff = false;
    bool cleanTests = false;
    CStringArray tests;

    COptions(void);
    ~COptions(void);

    bool parseArguments(string_q& command) override;
    void Init(void) override;

    bool doTest(const string_q& path, const string_q& testName, bool cmdTests);
    bool cleanTest(const string_q& path, const string_q& testName);
};

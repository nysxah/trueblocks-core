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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("which", "w", "enum[cmd*|api|both]", OPT_FLAG, "run command line, api, or both test cases (default 'cmd')."),
    COption("filter", "f", "enum[fast*|medi|slow]", OPT_FLAG, "filter the tests to run only the fast, medium speed, or slow tests (default 'fast')."),
    COption("no_quit", "n", "", OPT_FLAG, "Do not quit on error (default is to quit)."),
    COption("clean", "c", "", OPT_FLAG, "Clean working test folder before running tests."),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve a transaction's logs from the local cache or a running node."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (startsWith(arg, "-f:") || startsWith(arg, "--filter:")) {
            arg = substitute(substitute(arg, "-f:", ""), "--filter:", "");
            if (arg != "fast" && arg != "medi" && arg != "slow" && arg != "all")
                return usage("Specify only fast, medi, or slow for --filter option. Quitting...");
            speed_filter = arg;

        } else if (startsWith(arg, "-w:") || startsWith(arg, "--which:")) {
            arg = substitute(substitute(arg, "-w:", ""), "--which:", "");
                 if (arg == "cmd")  which = CMD;
            else if (arg == "api")  which = API;
            else if (arg == "both") which = BOTH;
            else return usage("Specify only cmd, api, or both for --which option. Quitting...");

        } else if (arg == "-n" || arg == "--no_quit") {
            quit_on_error = false;

        } else if (arg == "-c" || arg == "--clean") {
            cleanTests = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            arg = trim(arg, '/');
            tests.push_back(arg);
        }
    }

    if (speed_filter.empty())
        speed_filter = "fast";
    else if (speed_filter == "all")
        speed_filter = "";

    if (tests.empty()) {
        tests.push_back("libs/utillib");
        tests.push_back("libs/etherlib");
        tests.push_back("libs/acctlib");
        tests.push_back("dev_tools/makeClass");
        tests.push_back("tools/ethQuote");
        tests.push_back("tools/ethslurp");
        tests.push_back("tools/getAccounts");
        tests.push_back("tools/getBlock");
        tests.push_back("tools/getBloom");
        tests.push_back("tools/getLogs");
        tests.push_back("tools/getReceipt");
        tests.push_back("tools/getState");
        tests.push_back("tools/getTokenInfo");
        tests.push_back("tools/getTrace");
        tests.push_back("tools/getTrans");
        tests.push_back("tools/grabABI");
        tests.push_back("tools/whenBlock");
        tests.push_back("tools/whereBlock");
        tests.push_back("apps/acctExport");
        tests.push_back("apps/acctScrape");
        tests.push_back("apps/blockScrape");
        tests.push_back("apps/cacheMan");
        tests.push_back("apps/cacheStatus");
        tests.push_back("apps/chifra");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOn(0);
    minArgs = 0;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

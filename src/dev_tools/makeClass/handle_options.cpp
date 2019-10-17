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
#include "optiondef.h"

extern const char* STR_OPTION_STR;
//---------------------------------------------------------------------------------------------------
bool COptions::handle_options(void) {
    COptionDef::registerClass();

    string_q contents = asciiFileToString("../src/other/build_assets/option-master-list.csv");

    CStringArray lines;
    explode(lines, contents, '\n');

    map<string, bool> tools;
    COptionDefArray optionArray;
    for (auto line : lines) {
        COptionDef optDef(line);
        if (!optDef.tool.empty() && optDef.tool != "all" && optDef.tool != "tool" && optDef.tool != "templates") {
            optionArray.push_back(optDef);
            tools[optDef.group + "/" + optDef.tool] = true;
        }
    }

    for (auto tool : tools) {
        warnings.clear();
        warnings.str("");

        map<string, string> shortCmds;
        string_q fn = "../src/" + tool.first + "/options.cpp";

        ostringstream os;
        for (auto option : optionArray) {
            if ((option.group + "/" + option.tool) == tool.first) {
                check_option(option);
                os << option.Format(STR_OPTION_STR) << endl;
                if (!option.command_short.empty() && !contains(option.option_kind, "positional") && !contains(option.option_kind, "description")) {
                    if (!shortCmds[option.command_short].empty())
                        warnings << "Short command '" << cRed << option.command << "-" << option.command_short << cOff << "' conflicts with existing '" << cRed << shortCmds[option.command_short] << cOff << "'|";
                    shortCmds[option.command_short] = option.command + "-" + option.command_short;
                }
            }
        }

        string_q orig = asciiFileToString(fn);
        string_q converted = orig;
        converted = substitute(converted, "// BEG_CODE_OPTIONS", "// BEG_CODE_OPTIONS\n[{NEW_CODE}]\n<remove>");
        converted = substitute(converted, "// END_CODE_OPTIONS", "</remove>\n// END_CODE_OPTIONS");
        snagFieldClear(converted, "remove");
        replace(converted, "[{NEW_CODE}]\n\n", os.str());
        cerr << bBlue << "Processing: " << cOff << fn << " ";
        if (converted != orig) {
            cerr << cGreen << "wrote " << converted.size() << " bytes...";
            stringToAsciiFile(fn, converted);
        } else {
            cerr << cTeal << "no changes...";
        }
        cerr << cOff << endl;
        if (!warnings.str().empty()) {
            CStringArray w;
            explode(w, warnings.str(), '|');
            for (auto warning : w)
                LOG_WARN(warning);
        }
        break;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
COptionDef::COptionDef(const string_q& line) {
    CStringArray parts;
    explode(parts, line, ',');
    if (parts.size() > 0)  num = parts[0];
    if (parts.size() > 1)  group = parts[1];
    if (parts.size() > 2)  api_route = parts[2];
    if (parts.size() > 3)  tool = parts[3];
    if (parts.size() > 4)  order = parts[4];
    if (parts.size() > 5)  command = parts[5];
    if (parts.size() > 6)  command_short = parts[6];
    if (parts.size() > 7)  data_type = parts[7];
    if (parts.size() > 8)  option_kind = parts[8];
    if (parts.size() > 9)  default_value = parts[9];
    if (parts.size() > 10) description_core = substitute(parts[10], "&#44;", ",");
    if (parts.size() > 11) description_api = substitute(parts[11], "&#44;", ",");
    if (parts.size() > 12) is_required = parts[12];
    if (parts.size() > 13) core_visible = parts[13];
    if (parts.size() > 14) docs_visible = parts[14];
}

//---------------------------------------------------------------------------------------------------
bool COptions::check_option(const COptionDef& option) {

    // Check valid data types
    CStringArray validTypes = {
        "<addr>", "<blknum>", "<pair>", "<path>", "<range>", "<string>", "<uint>",
        "boolean", "string",
    };

    bool valid_type = false;
    for (auto type : validTypes) {
        if (type == option.data_type) {
            valid_type = true;
        }
    }
    if (!valid_type) {
        if (startsWith(option.data_type, "enum")) valid_type = true;
        if (startsWith(option.data_type, "list")) valid_type = true;
    }
    if (!valid_type && option.option_kind == "description" && option.data_type.empty()) valid_type = true;

    if (!valid_type)
        warnings << "Unknown type '" << cRed << option.data_type << cOff << "' for option '" << cRed << option.command << cOff << "'|";
    if (option.option_kind == "description" && !endsWith(option.description_core, "."))
        warnings << "Description '" << cRed << option.description_core << cOff << "' should end with a period.|";
    if (option.option_kind != "description" && endsWith(option.description_core, "."))
        warnings << "Option '" << cRed << option.description_core << cOff << "' should not end with a period.|";

    return true;
}

//---------------------------------------------------------------------------------------------------
const char* STR_OPTION_STR =
"    COption(\"[{COMMAND}]\", \"[{COMMAND_SHORT}]\", \"[{DATATYPE}]\", [{OPTS}], \"[{DESCRIPTION_CORE}]\"),";
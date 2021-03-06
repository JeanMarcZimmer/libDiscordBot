/*
 * MIT License
 *
 * Copyright (c) 2020 Christian Tost
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "JSONCmdsConfig.hpp"
#include <fstream>

namespace DiscordBot
{
    CJSONCmdsConfig::CJSONCmdsConfig() 
    {
        std::ifstream in("databs.json", std::ios::in);
        if(in.is_open())
        {
            std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            in.close();

            CJSON json;
            m_CmdDatabase = json.Deserialize<CmdDatabase>(str); 
        }

        in.open("databs_prefixes.json", std::ios::in);
        if(in.is_open())
        {
            std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            in.close();

            CJSON json;
            m_PrefixDatabase = json.Deserialize<PrefixDatabase>(str); 
        }
    }

    void CJSONCmdsConfig::AddRoles(const std::string &Guild, const std::string &Command, const std::vector<std::string> &Roles)
    {
        // m_Database[Guild][Command].insert(m_Database[Guild][Command].end(), Roles.begin(), Roles.end());

        std::vector<std::string> &DBRoles = m_CmdDatabase[Guild][Command];
        for (auto &&e : Roles)
        {
            auto IT = std::find(DBRoles.begin(), DBRoles.end(), e);
            if(IT == DBRoles.end())
                DBRoles.push_back(e);
        }

        SaveCmdDB();
    }

    std::vector<std::string> CJSONCmdsConfig::GetRoles(const std::string &Guild, const std::string &Command)
    {
        auto GIT = m_CmdDatabase.find(Guild);
        if (GIT != m_CmdDatabase.end())
        {
            auto CIT = GIT->second.find(Command);
            if (CIT != GIT->second.end())
                return CIT->second;
        }
        
        return std::vector<std::string>();
    }

    void CJSONCmdsConfig::DeleteCommand(const std::string &Guild, const std::string &Command)
    {
        auto GIT = m_CmdDatabase.find(Guild);
        if (GIT != m_CmdDatabase.end())
        {
            if(GIT->second.find(Command) != GIT->second.end())
            {
                GIT->second.erase(Command);
                SaveCmdDB();
            }
        }
    }

    void CJSONCmdsConfig::RemoveRoles(const std::string &Guild, const std::string &Command, const std::vector<std::string> &Roles)
    {
        auto GIT = m_CmdDatabase.find(Guild);
        if (GIT != m_CmdDatabase.end())
        {
            auto CIT = GIT->second.find(Command);
            if (CIT != GIT->second.end())
            {
                std::vector<std::string> &DBRoles = CIT->second;
                auto IT = std::remove_if(DBRoles.begin(), DBRoles.end(), [Roles](std::string Val) 
                {
                    auto IT = std::find(Roles.begin(), Roles.end(), Val);
                    return IT != Roles.end();
                });

                DBRoles.erase(IT, DBRoles.end());

                if(DBRoles.empty())
                    DeleteCommand(Guild, Command);
                else
                    SaveCmdDB();
            }
        }
    }

    void CJSONCmdsConfig::ChangePrefix(const std::string &Guild, const std::string &Prefix)
    {
        m_PrefixDatabase[Guild] = Prefix;
        SavePrefixDB();
    }

    void CJSONCmdsConfig::RemovePrefix(const std::string &Guild)
    {
        m_PrefixDatabase.erase(Guild);
        SavePrefixDB();
    }

    std::string CJSONCmdsConfig::GetPrefix(const std::string &Guild, const std::string &Default)
    {
        std::string Ret = Default;

        auto IT = m_PrefixDatabase.find(Guild);
        if(IT != m_PrefixDatabase.end())
            Ret = IT->second;

        return Ret;
    }

    void CJSONCmdsConfig::SaveCmdDB()
    {
        CJSON json;
        std::string str = json.Serialize(m_CmdDatabase);

        std::ofstream out("databs.json", std::ios::out);
        if (out.is_open())
        {
            out << str;
            out.close();
        }
    }

    void CJSONCmdsConfig::SavePrefixDB()
    {
        CJSON json;
        std::string str = json.Serialize(m_PrefixDatabase);

        std::ofstream out("databs_prefixes.json", std::ios::out);
        if (out.is_open())
        {
            out << str;
            out.close();
        }
    }
} // namespace DiscordBot
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

#ifndef RIGHTSCOMMAND_HPP
#define RIGHTSCOMMAND_HPP

#include <controller/ICommand.hpp>
#include <controller/IController.hpp>

namespace DiscordBot
{
    class CRightsCommand : public ICommand
    {
        public:
            CRightsCommand(IController *controller, IDiscordClient *client);
            ~CRightsCommand() {}

        private:
            IController *m_Controller;
            IDiscordClient *m_Client;

            void SetRoles(CommandContext ctx);
            void RemoveRoles(CommandContext ctx);
            void RemoveAllRoles(CommandContext ctx);
            void GetRoles(CommandContext ctx);

            std::string GetRoleID(Guild guild, const std::string &RoleName);
            bool SplitParams(CommandContext ctx, std::string &Cmd, std::vector<std::string> &RoleIDs);
    };
} // namespace DiscordBot


#endif //RIGHTSCOMMAND_HPP
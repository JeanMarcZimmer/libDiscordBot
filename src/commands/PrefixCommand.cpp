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

#include "PrefixCommand.hpp"
#include <IDiscordClient.hpp>
#include <algorithm>

namespace DiscordBot
{
    CPrefixCommand::CPrefixCommand(IController *controller, IDiscordClient *client) : m_Controller(controller), m_Client(client)
    {
        RegisterCommandHandler("setp", std::bind(&CPrefixCommand::SetPrefix, this, std::placeholders::_1));
        RegisterCommandHandler("removep", std::bind(&CPrefixCommand::RemovePrefix, this, std::placeholders::_1));
        RegisterCommandHandler("showp", std::bind(&CPrefixCommand::ShowPrefix, this, std::placeholders::_1));
    }

    void CPrefixCommand::SetPrefix(CommandContext ctx)
    {
        if(!ctx->Msg->GuildRef)
            return;

        CommandsConfig cfg = m_Controller->GetCmdConfig();
        if(cfg)
        {
            std::string Prefix = ctx->Params.front();
            Prefix.erase(std::remove(Prefix.begin(), Prefix.end(), ' '), Prefix.end());

            cfg->ChangePrefix(ctx->Msg->GuildRef->ID, Prefix);
            m_Client->SendMessage(ctx->Msg->ChannelRef, "New prefix: " + Prefix);
        }
    }

    void CPrefixCommand::RemovePrefix(CommandContext ctx)
    {
        if(!ctx->Msg->GuildRef)
            return;

        CommandsConfig cfg = m_Controller->GetCmdConfig();
        if(cfg)
        {
            cfg->RemovePrefix(ctx->Msg->GuildRef->ID);
            m_Client->SendMessage(ctx->Msg->ChannelRef, "New prefix: " + m_Controller->GetPrefix());
        } 
    }

    void CPrefixCommand::ShowPrefix(CommandContext ctx)
    {
        if(!ctx->Msg->GuildRef)
            return;

        m_Client->SendMessage(ctx->Msg->ChannelRef, "Prefix: " + m_Controller->GetPrefix(ctx->Msg->GuildRef));     
    }
} // namespace DiscordBot

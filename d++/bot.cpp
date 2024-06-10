	#include <dpp/dpp.h>
	 
#include "../debug.h"

#include "../utf8.h"

#include "../types.h"
#include "../machine.h"

	using namespace std::literals;

	const std::string BOT_TOKEN = "xyz";
	 
	int main() {

		machine_c machine;

		uint64_t intents = dpp::i_default_intents | dpp::i_message_content;
	    dpp::cluster bot(BOT_TOKEN, intents);
	 
	    bot.on_log(dpp::utility::cout_logger());

		bot.on_message_create([&](const dpp::message_create_t& event) mutable {
			
	        /* See if the message contains the phrase we want to check for.
	         * If there's at least a single match, we reply and say it's not allowed.
	         */
	        if (event.msg.content.starts_with("cjk-->"sv)) {

	        	std::string example_string = event.msg.content.substr(5);
	        	machine.load(example_string); 
	        	machine.lex();

	        	int runtime = machine.run(1<<16);

	        	if(machine.response.empty())
	        	{
	        		event.reply("Execution finished after "s
	        			.append(std::to_string(runtime)
	        			.append(" steps, with no response."))
	        		);
	        	}
	            else
	            {
	            	event.reply(
	            		"```\n"s.append(machine.response)
	            			    .append("```\nExecution finished after ")
	            			    .append(std::to_string(runtime))
	            			    .append(" steps"), 
	            		true
	            	);
	        	}
	        }
	    });
	 
	    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
	        if (event.command.get_command_name() == "ping") {
	            event.reply("Pong!");
	        }
	    });
	 
	    bot.on_ready([&bot](const dpp::ready_t& event) {
	        if (dpp::run_once<struct register_bot_commands>()) {
	            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
	        }
	    });
	 
	    bot.start(dpp::st_wait);
	}

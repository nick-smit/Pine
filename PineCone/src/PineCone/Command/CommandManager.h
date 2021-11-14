#pragma once

#include "Command.h"

#include <stack>
#include <memory>

namespace Pine {

	typedef std::stack<std::shared_ptr<Command>> CommandStack;

	class CommandManager {
	public:
		CommandManager();
		~CommandManager() = default;

		void ExecuteCommand(std::shared_ptr<Command> command);
		void UndoCommand();
		void RedoCommand();

		bool CanUndo() const;
		bool CanRedo() const;
	private:
		CommandStack m_UndoStack = {};
		CommandStack m_RedoStack = {};

	public:
		static CommandManager& GetInstance() { return *s_CommandManager; }
	private:
		static CommandManager* s_CommandManager;
	};

}

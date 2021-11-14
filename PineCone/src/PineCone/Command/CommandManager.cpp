#include "pcpch.h"
#include "CommandManager.h"


namespace Pine {

	CommandManager* CommandManager::s_CommandManager = nullptr;

	CommandManager::CommandManager()
	{
		s_CommandManager = this;
	}

	void CommandManager::ExecuteCommand(std::shared_ptr<Command> command)
	{
		// Clear redo stack
		m_RedoStack = CommandStack();

		command->Execute();
		m_UndoStack.push(command);
	}

	void CommandManager::UndoCommand()
	{
		if (CanUndo()) {
			m_UndoStack.top()->Undo();
			m_RedoStack.push(m_UndoStack.top());
			m_UndoStack.pop();
		}
	}

	void CommandManager::RedoCommand()
	{
		if (CanRedo()) {
			m_RedoStack.top()->Redo();
			m_UndoStack.push(m_RedoStack.top());
			m_RedoStack.pop();
		}
	}

	bool CommandManager::CanUndo() const
	{
		return m_UndoStack.size() > 0;
	}

	bool CommandManager::CanRedo() const
	{
		return m_RedoStack.size() > 0;
	}

}

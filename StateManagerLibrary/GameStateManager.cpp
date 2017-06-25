#include <GameStateManager.h>
#include <assert.h>

GameStateManager::GameStateManager()
{
}

GameStateManager::~GameStateManager()
{
	// Delete states that have been assigned dynamically allocated memory (ONLY DELETE MEMORY AT POINTERS FROM ONE CONTAINER)
	for (auto state : m_states) {
		delete state.m_val;
	}
}

void GameStateManager::Update(float deltaTime)
{
	// Process commands on the queue
	ProcessCommands();

	// Update active states on the stack
	Stack<IGameState*> tmp = m_activeStates;	// Avoid needing to shift memory by rebuilding the stack

	while (!tmp.IsEmpty()) {
		// Only update states where updating is active
		if (tmp.Top()->IsUpdateActive()) {
			tmp.Top()->Update(deltaTime);
		}
		// Remove state to access next in stack
		tmp.PopBack();
	}
}

void GameStateManager::Draw()
{
	// Draw active states on the stack
	Stack<IGameState*> tmp = m_activeStates;	// Avoid needing to shift memory by rebuilding stack

	while (!tmp.IsEmpty()) {
		// Only draw states where drawing is active
		if (tmp.Top()->IsDrawActive()) {
			tmp.Top()->Draw();
		}
		// Remove state to access next in stack
		tmp.PopBack();
	}
}

void GameStateManager::PushState(const char * name, IGameState * state)
{
	Command cmd;
	cmd.action = eCommands::PUSH;
	cmd.name = name;
	cmd.state = state;

	m_commands.PushBack(cmd);
}

void GameStateManager::SetState(const char * name)
{
	Command cmd;
	cmd.action = eCommands::SET;
	cmd.name = name;

	m_commands.PushBack(cmd);
}

void GameStateManager::PopState()
{
	Command cmd;
	cmd.action = eCommands::POP;

	m_commands.PushBack(cmd);
}

void GameStateManager::PauseStates()
{
	Command cmd;
	cmd.action = eCommands::PAUSE;

	m_commands.PushBack(cmd);
}


void GameStateManager::ResumeStates()
{
	Command cmd;
	cmd.action = eCommands::RESUME;

	m_commands.PushBack(cmd);
}

void GameStateManager::DoPauseStates()
{
	// 'Freeze' all active states by stopping updating capabilities
	Stack<IGameState*> tmp = m_activeStates;	// Avoid needing to shift memory by rebuilding stack

	while (!tmp.IsEmpty()) {
		tmp.Top()->SetUpdateActive(false);		// Will apply to m_activeStates because of pointers
		tmp.PopBack();
	}
}

void GameStateManager::DoResumeStates()
{
	// Un-freeze all active states by restoring updating capabilities
	Stack<IGameState*> tmp = m_activeStates;	// Avoid needing to shift memory by rebuilding stack

	while (!tmp.IsEmpty()) {
		tmp.Top()->SetUpdateActive(true);		// Will apply to m_activeStates because of pointers
		tmp.PopBack();
	}
}


void GameStateManager::DoPushState(const char * name, IGameState * state)
{
	Map<const char*, IGameState*>::PairNode* foundNode = m_states.findNode(name);

	// State value already exists
	if (foundNode) {
		delete foundNode->m_val;
	}

	// Push paired value to the map OR replace existing
	m_states[name] = state;
}

void GameStateManager::DoSetState(const char * name)
{
	auto foundNode = m_states.findNode(name);

	if (foundNode) {
		// Check if state is already in active states
		assert(!m_activeStates.Contains(foundNode->m_val) && "State already in active states");
		m_activeStates.PushBack(foundNode->m_val);

		// Run start up function on state
		m_activeStates.Top()->Startup();

		// Reset update and draw activeness
		m_activeStates.Top()->SetDrawActive(true);
		m_activeStates.Top()->SetUpdateActive(true);
	}
	else {
		assert(false && "404 State not found.");
	}
}

void GameStateManager::DoPopState()
{
	if (m_activeStates.IsEmpty()) {
		assert(false && "Tried to remove a state from an empty stack.");
		return;
	}

	// Call shut-down function before removing
	m_activeStates.Top()->Shutdown();
	m_activeStates.PopBack();
}

void GameStateManager::ProcessCommands()
{
	// Execute commands and clear them so that queue is empty by the end
	while (!m_commands.IsEmpty()) {
		GameStateManager::Command cmd = m_commands.Top();

		switch (cmd.action) {
		case eCommands::SET:	DoSetState(cmd.name);				break;
		case eCommands::PUSH:	DoPushState(cmd.name, cmd.state);	break;
		case eCommands::POP:	DoPopState();						break;
		case eCommands::PAUSE:	DoPauseStates();					break;
		case eCommands::RESUME:	DoResumeStates();					break;

		default:
			assert(false && "Invalid command on queue");
		}
		// Move to next command in queue
		m_commands.PopBack();
	}
}

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

void GameStateManager::Draw(aie::Renderer2D* a_r2d)
{
	// Draw active states on the stack
	Stack<IGameState*> tmp = m_activeStates;	// Avoid needing to shift memory by rebuilding stack

	while (!tmp.IsEmpty()) {
		// Only draw states where drawing is active
		if (tmp.Top()->IsDrawActive()) {
			tmp.Top()->Draw(a_r2d);
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
	cmd.action = eCommands::SET_BYNAME;
	cmd.name = name;

	m_commands.PushBack(cmd);
}

void GameStateManager::SetState(IGameState * state)
{
	Command cmd;
	cmd.action = eCommands::SET_BYSTATE;
	cmd.state = state;

	m_commands.PushBack(cmd);
}

void GameStateManager::PopState()
{
	Command cmd;
	cmd.action = eCommands::POP;

	m_commands.PushBack(cmd);
}

IGameState* GameStateManager::GetTopState() {
	// Don't access active states if stack isn't initialized
	if (!m_activeStates.IsEmpty()) {
		return m_activeStates.Top();
	}

	// No active states, return nullptr
	return nullptr;
}

IGameState * GameStateManager::GetState(const char * a_name)
{
	assert(m_states.findNode(a_name) != nullptr && "Tried to get state that doesn't exist in available states.");

	return m_states[a_name];
}

std::vector<IGameState*> GameStateManager::GetActiveStates()
{
	std::vector<IGameState*> result;

	// Copy active state pointers to a vector via temporary copy
	Stack<IGameState*> tmp = m_activeStates;	

	while (!tmp.IsEmpty()) {
		result.push_back(tmp.Top());
		tmp.PopBack();					// Does NOT delete memory, only removes it from temporary stack
	}

	return result;
}

std::vector<IGameState*> GameStateManager::GetStates()
{
	std::vector<IGameState*> result;

	// Copy pointers from available states into vector
	for (auto state : m_states) {
		result.push_back(state.m_val);
	}

	return result;
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
		IGameState* setState = foundNode->m_val;

		// Run start up function on state
		setState->Startup();

		// Only add state to active states if it's not there already
		if (!m_activeStates.Contains(setState)) {
			m_activeStates.PushBack(setState);
		}
	}
	else {
		assert(false && "Tried to set state that wasn't in available states.");
	}
}

void GameStateManager::DoSetState(IGameState * a_state)
{
	IGameState* foundState = nullptr;

	// Check that state is in available states
	for (auto state : m_states) {
		if (state.m_val == a_state) {
			foundState = state.m_val;
		}
	}

	// State is available
	if (foundState) {
		// Run start up function on state
		foundState->Startup();

		// Only add state to active states if it's not there already
		if (!m_activeStates.Contains(foundState)) {
			m_activeStates.PushBack(foundState);
		}
	}
	
	else {
		assert(false && "Tried to set state that wasn't in available states.");
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
		case eCommands::SET_BYNAME:	DoSetState(cmd.name);				break;
		case eCommands::SET_BYSTATE:DoSetState(cmd.state);				break;
		case eCommands::PUSH:		DoPushState(cmd.name, cmd.state);	break;
		case eCommands::POP:		DoPopState();						break;
		case eCommands::PAUSE:		DoPauseStates();					break;
		case eCommands::RESUME:		DoResumeStates();					break;

		default:
			assert(false && "Invalid command on queue");
		}
		// Move to next command in queue
		m_commands.PopBack();
	}
}

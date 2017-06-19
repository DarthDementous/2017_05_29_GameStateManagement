#include "GameStateManager.h"
#include <assert.h>

GameStateManager::GameStateManager()
{
}

GameStateManager::~GameStateManager()
{
	// Delete states that have been assigned dynamically allocated memory (ONLY DELETE MEMORY AT POINTERS FROM ONE CONTAINER)
	for (auto state : m_states) {
		delete state.second;
	}
}

void GameStateManager::Update(float deltaTime)
{
	// Process commands on the queue
	ProcessCommands();
	
	// Update active states
	for (auto state : m_activeStates) {
		// Only update states where updating is active
		if (state->IsUpdateActive()) {
			state->Update(deltaTime);
		}
	}
}

void GameStateManager::Draw()
{
	// Draw active states
	for (auto state : m_activeStates) {
		// Only draw states where drawing is active
		if (state->IsDrawActive()) {
			state->Draw();
		}
	}
}

void GameStateManager::PushState(const char * name, IGameState * state)
{
	Command cmd;
	cmd.action	= eCommands::PUSH;
	cmd.name	= name;
	cmd.state	= state;

	m_commands.push_back(cmd);
}

void GameStateManager::SetState(const char * name)
{
	Command cmd;
	cmd.action	= eCommands::SET;
	cmd.name	= name;

	m_commands.push_back(cmd);
}

void GameStateManager::PopState()
{
	Command cmd;
	cmd.action	= eCommands::POP;

	m_commands.push_back(cmd);
}

void GameStateManager::PauseStates()
{
	Command cmd;
	cmd.action = eCommands::PAUSE;

	m_commands.push_back(cmd);
}


void GameStateManager::ResumeStates()
{
	Command cmd;
	cmd.action = eCommands::RESUME;

	m_commands.push_back(cmd);
}

void GameStateManager::DoPauseStates()
{
	// 'Freeze' all active states by stopping updating capabilities
	for (auto state : m_activeStates) {
		state->SetUpdateActive(false);
	}
}

void GameStateManager::DoResumeStates()
{
	// Un-freeze all active states by restoring updating capabilities
	for (auto state : m_activeStates) {
		state->SetUpdateActive(true);
	}
}


void GameStateManager::DoPushState(const char * name, IGameState * state)
{
	auto iter = m_states.find(name);
	
	// State value already exists
	if (iter != m_states.end()) {
		delete iter->second;
	}

	// Push paired value to the map OR replace existing
	m_states[name] = state;
}

void GameStateManager::DoSetState(const char * name)
{
	auto map_iter = m_states.find(name);

	if (map_iter != m_states.end()) {
#ifdef _DEBUG
		// Check if state is already in active states
		auto vec_iter = std::find(m_activeStates.begin(), m_activeStates.end(), map_iter->second);
		assert(vec_iter == m_activeStates.end() && "State already in active states");
#endif
		m_activeStates.push_back(map_iter->second);
	}
	else {
		assert(false && "404 State not found.");
	}
}

void GameStateManager::DoPopState()
{
	if (m_activeStates.empty()) {
		assert(false && "Tried to remove a state from an empty stack.");
		return;
	}

	m_activeStates.pop_back();
}

void GameStateManager::ProcessCommands()
{
	for (auto cmd : m_commands) {
		switch (cmd.action) {
		case eCommands::SET:	DoSetState(cmd.name);				break;
		case eCommands::PUSH:	DoPushState(cmd.name, cmd.state);	break;
		case eCommands::POP:	DoPopState();						break;
		case eCommands::PAUSE:	DoPauseStates();					break;
		case eCommands::RESUME:	DoResumeStates();					break;

		default:
			assert(false && "Invalid command on queue");
		}
	}

	// Clear command queue
	m_commands.clear();
}

#pragma once

#include "IGameState.h"
#include <map>
#include <vector>

class GameStateManager {
public:
	/*
	*	@brief	Class that handles which states are active.
	*/
	GameStateManager();
	~GameStateManager();

	void Update(float deltaTime);
	void Draw();

	void PushState(const char* name, IGameState *state);
	void SetState(const char* name);
	void PopState();

private:
	// Commands to be queued and executed
	enum class eCommands {
		SET,
		PUSH,
		POP
	};

	// Node that holds all information necessary to executing a command
	struct Command {
		eCommands		action;
		const char*		name;
		IGameState*		state;
	};

	/*
	*	@brief	Push a state onto the stack with an assigned value
	*	@param	name is the key constant char array of the state.
	*	@param	state is the state object serving as the value of the key.
	*/
	void DoPushState(const char* name, IGameState *state);

	/*
	*	@brief	Add the active state by key to the active state vector
	*	@param	name is the key that identifies the desired state.
	*/
	void DoSetState(const char* name);
	
	/*
	*	@brief	Removes a state from the end of the active state container.
	*/
	void DoPopState();

	/*
	*	@brief	Interpret commands that have been added to the queue
	*/
	void ProcessCommands();
	
	std::map<const char*, IGameState*>		m_states;			/*Map container of currently held states*/
	std::vector<IGameState*>				m_activeStates;		/*Vector container of active states*/
	std::vector<Command>					m_commands;			/*Vector container of commands to be executed*/
};
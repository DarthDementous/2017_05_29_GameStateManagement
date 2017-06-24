#pragma once

#include "IGameState.h"
#include <Stack.hpp>
#include <Queue.hpp>
#include <Map.hpp>

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

	/**
	*	@brief Get top state off the stack.
	*	@return State at the end of the container.
	*/
	IGameState* GetTopState() { return m_activeStates.Top(); }

	/**
	*	@brief Send command to pause current states
	*/
	void PauseStates();

	/**
	*	@brief Send command to resume current states
	*/
	void ResumeStates();

private:
	// Commands to be queued and executed
	enum class eCommands {
		SET,
		PUSH,
		POP,
		PAUSE,
		RESUME
	};

	// Node that holds all information necessary to executing a command
	struct Command {
		eCommands		action;
		const char*		name;
		IGameState*		state;
	};

	/**
	*	@brief De-activate update capabilities of all active states
	*/
	void DoPauseStates();

	/*
	*	@brief Re-active update capabilities of all active states
	*/
	void DoResumeStates();

	/**
	*	@brief	Push a state onto the stack with an assigned value
	*	@param	name is the key constant char array of the state.
	*	@param	state is the state object serving as the value of the key.
	*/
	void DoPushState(const char* name, IGameState *state);

	/**
	*	@brief	Add the active state by key to the active state vector
	*	@param	name is the key that identifies the desired state.
	*/
	void DoSetState(const char* name);

	/**
	*	@brief	Removes a state from the end of the active state container.
	*/
	void DoPopState();

	/**
	*	@brief	Interpret commands that have been added to the queue
	*/
	void ProcessCommands();

	Map<const char*, IGameState*>	m_states;			/*Map container of currently held states*/
	Stack<IGameState*>				m_activeStates;		/*Stack of active states*/
	Queue<Command>					m_commands;			/*Queue of commands to be executed*/
};
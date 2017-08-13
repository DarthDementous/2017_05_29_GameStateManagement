#pragma once

#include "IGameState.hpp"
#include <Stack.hpp>
#include <Queue.hpp>
#include <Map.hpp>
#include <vector>

namespace aie {
	class Renderer2D;
}

class GameStateManager {
public:
	/*
	*	@brief	Class that handles which states are active.
	*	@param	a_r2d is the pointer to renderer2D used to render states.
	*/
	GameStateManager();
	~GameStateManager();

	void Update(float deltaTime);
	void Draw(aie::Renderer2D* a_r2d);

	void PushState(const char* name, IGameState *state);
	void SetState(const char* name);
	void SetState(IGameState* state);
	void PopState();

	/**
	*	@brief Get specified state from available states by key.
	*	@param a_name is the name of the state.
	*	@return desired state.
	*/
	IGameState* GetState(const char* a_name);

	/**
	*	@brief Return all current active states.
	*	@return Vector of active states (empty if no states active)
	*/
	std::vector<IGameState*> GetActiveStates();

	/**
	*	@brief Return all available states.
	*	@return Vector of possible states (empty if no possible states)
	*/
	std::vector<IGameState*> GetStates();

	/**
	*	@brief Get top state off the stack.
	*	@return State at the end of the container, if no active states then returns nullptr.
	*/
	IGameState* GetTopState();

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
		SET_BYNAME,
		SET_BYSTATE,
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
	*	@brief	Add the active state by key to the active state stack.
	*	@param	name is the key that identifies the desired state.
	*/
	void DoSetState(const char* name);

	/**
	*	@brief Add the active state by value to the active state stack.
	*	@param state is the pointer to the state to go onto the active stack (must already be in available states)
	*/
	void DoSetState(IGameState* state);

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
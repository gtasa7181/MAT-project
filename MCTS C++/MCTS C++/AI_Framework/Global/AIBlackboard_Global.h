

#ifndef AI_BLACKBOARD_GLOBAL_H
#define AI_BLACKBOARD_GLOBAL_H


#include "../../MCTS/GameState.h"
#include <map>
#include <vector>
#include <string>
#include "AIMath_Global.h"
/*
CMP304/MAT501 AI Framework (2025)
*/


#pragma once
class AIBlackboard_Global
{

public:

	AIBlackboard_Global(const AIBlackboard_Global& obj) = delete;

	static AIBlackboard_Global* getInstance();

	void setValue(int val) { value_ = val; }
	int getValue() {
		return value_;
	}

	void AddNewValueInt(std::string name, int new_value);
	int GetValueInt(std::string name);
	int GetAndDeleteValueInt(std::string name);
	void EditValueInt(std::string name, int new_value);
	void DeleteValueInt(std::string name);

	void AddNewValueGameState(std::string name, GameState new_value);
	GameState GetValueGameState(std::string name);
	GameState GetAndDeleteValueGameState(std::string name);
	void EditValueGameState(std::string name, GameState new_value);
	void DeleteValueGameState(std::string name);

	void AddNewValueVec2f(std::string name, AIMath_Global::Vector2f data);
	AIMath_Global::Vector2f GetValueVec2f(std::string name);
	AIMath_Global::Vector2f GetAndDeleteValueVec2f(std::string name);
	void EditValueVec2f(std::string name, AIMath_Global::Vector2f new_value);
	void DeleteValueVec2f(std::string name);

	void AddNewValueIntVec(std::string name, std::vector<int> new_value);
	std::vector<int> GetValueIntVec(std::string name);
	std::vector<int> GetAndDeleteValueIntVec(std::string name);
	void EditValueIntVec(std::string name, std::vector<int>  new_value);
	void DeleteValueIntVec(std::string name);

protected:
	int value_;

	std::map<std::string, GameState> datamapGameState;
	std::map<std::string, int> datamapInt;
	std::map<std::string, AIMath_Global::Vector2f> datamapVec2f;
	std::map<std::string, std::vector<int> > datamapIntVec;


private:

	static AIBlackboard_Global* instancePointer;
	AIBlackboard_Global() :value_(0) {}



};

#endif //!AI_BLACKBOARD_GLOBAL_H


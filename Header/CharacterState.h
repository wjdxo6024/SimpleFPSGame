#pragma once

enum State {
	Nothing = 0,
	Wating,
	Moving,
	Running,
	Guard,
	Skill
};

class CharacterState
{
public:
	void SetState();
	void GetState();
	void Update();
	CharacterState();
	~CharacterState();

private:
	State CharaState;
};


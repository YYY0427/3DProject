#pragma once
class Map
{
public:
	Map();
	virtual ~Map();

	void Init();
	void Update();
	void Draw();

private:
	int texture_;
};
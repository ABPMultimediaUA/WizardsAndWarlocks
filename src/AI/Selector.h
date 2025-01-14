#ifndef SELECTOR_H
#define SELECTOR_H

#include <stdlib.h>
#include <vector>
#include "Task.h"

class Selector : public Task{
public:
	virtual ~Selector();
	Selector();
	void addChild(Task* task);
	void addChildren(std::vector<Task*> tasks);
	virtual bool run(Blackboard* bb);
	bool terminate();
protected:
	std::vector<Task*> children;
};

class RandomSelector: public Selector{
public:
	~RandomSelector();
	RandomSelector();
	bool run(Blackboard* bb);
	void shuffleVector();
private:
};

#endif

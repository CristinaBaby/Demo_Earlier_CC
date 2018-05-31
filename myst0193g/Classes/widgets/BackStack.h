/**
*	BackStack
*
*	@Author:				liujian
*	@Version				1.0
*	@Data					2013-9-2
*
*	@Description: assist class used to record layers which can back, decide the order of backing
**/
#ifndef _BACKSTACK_H_
#define _BACKSTACK_H_
#include "cocos2d.h"
#include <vector>

class BackStack
{
public:
	virtual ~BackStack();

	static BackStack* sharedBackStack();
	static void purge();

	void pushBackLayer(cocos2d::CCNode* node);
	cocos2d::CCNode* popBackLayer();
	cocos2d::CCNode* getTopBackLayer();

private:
	BackStack();

	std::vector<cocos2d::CCNode*>					m_NodeStack;
};

#endif

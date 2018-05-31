#include "BackStack.h"

USING_NS_CC;
using namespace std;

BackStack::~BackStack()
{
	m_NodeStack.clear();
}

static BackStack* s_BackStack = NULL;
BackStack* BackStack::sharedBackStack()
{
	if(!s_BackStack)
	{
		s_BackStack = new BackStack;
	}
	return s_BackStack;
}

void BackStack::purge()
{
	if(s_BackStack)
	{
		delete s_BackStack;
		s_BackStack = NULL;
	}
}

void BackStack::pushBackLayer(CCNode* node)
{
	if(m_NodeStack.size() < 1 || m_NodeStack[m_NodeStack.size() - 1] != node)
	{
		m_NodeStack.push_back(node);
	}
}

CCNode* BackStack::popBackLayer()
{
	if(m_NodeStack.size() < 1) return NULL;

	CCNode* top = m_NodeStack[m_NodeStack.size() - 1];
	m_NodeStack.erase(--m_NodeStack.end());
	return top;
}
	
CCNode* BackStack::getTopBackLayer()
{
	if(m_NodeStack.size() < 1) return NULL;

	return m_NodeStack[m_NodeStack.size() - 1];
}

BackStack::BackStack()
{

}
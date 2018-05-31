#include "CoverLayer.h"
#include "BackStack.h"

USING_NS_CC;

#define DEFAULT_BACKGROUND_COLOR        ccc4(0, 0, 0, 80)

CoverLayer::CoverLayer() : m_pfnSelector(NULL), m_pListener(NULL)
{
    
}

CoverLayer::~CoverLayer()
{
    
}

bool CoverLayer::init()
{
    bool bRet = initWithColor(DEFAULT_BACKGROUND_COLOR);
    
    // Smooth fade-in to dim with semi-transparency.
    this->runAction(CCFadeTo::create(kCoverLayerAnimationTime, 180));
    
	return bRet;
}

bool CoverLayer::initWithColor(const ccColor4B& color)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(color));
        
        // enable back key
        this->setKeypadEnabled(true);
        BackStack::sharedBackStack()->pushBackLayer(this);
        
        // Tag for later validation.
        this->setTag(kCoverLayerTag);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

CoverLayer* CoverLayer::createWithColor(ccColor4B color)
{
	CoverLayer* layer = new CoverLayer();
	if(layer && layer->initWithColor(color))
	{
		return layer;
	}
    
	CC_SAFE_DELETE(layer);
	return NULL;
}

void CoverLayer::onEnter()
{
	CCLayerColor::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true); // swallows touches.
}

void CoverLayer::onExit()
{
	CCLayerColor::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void CoverLayer::keyBackClicked()
{
	if(this == BackStack::sharedBackStack()->getTopBackLayer())
	{
        // remove from back stack
		BackStack::sharedBackStack()->popBackLayer();
        
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        
		this->removeFromParentAndCleanup(true);
	}
}

void CoverLayer::setCloseCallback(cocos2d::CCObject *target, cocos2d::SEL_CallFuncN selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
}

bool CoverLayer::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	CC_ASSERT(this->getChildren());
    //	CCPoint touchLocation = this->convertTouchToNodeSpace(touch);
    
	// just have only one child
	/*CCNode *dialogBox = this->getChildByTag(kCoverLayerContentTag);
     CC_ASSERT(dialogBox);
     CCRect const bbox = dialogBox->boundingBox();
     
     // eat all touches outside of dialog box.
     return !bbox.containsPoint(touchLocation);*/
    
	return true;
}
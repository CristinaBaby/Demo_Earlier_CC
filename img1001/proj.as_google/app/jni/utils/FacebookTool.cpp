#include "../../Classes/modules/FacebookTool.h"
#include "../../Classes/modules/STFacebook.h"
void FacebookTool::postToFacebook(const char* msg, const char* imagePath, const char* url)
{
	//STFacebook::getInstance()->NativePostStatusUpdate(1,"987654321");
	STFacebook::getInstance()->NativePostRichContentFile(1,msg,url,imagePath);
}



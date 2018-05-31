#include "../../Classes/modules/STFacebook.h"
#include "../../Classes/modules/STSystemFunction.h"
static STFacebook* sp_Facebook = NULL;
STFacebook* STFacebook::getInstance() {
	if(NULL == sp_Facebook) sp_Facebook = new STFacebook();
	return sp_Facebook;
}

void STFacebook::purgeInstance() {
	if(NULL != sp_Facebook)
	{
		delete sp_Facebook;
		sp_Facebook = NULL;
	}
}

STFacebook::STFacebook()
{

}

STFacebook::~STFacebook() {

}


void STFacebook::onPostStatus(int tag, bool bSuccess)
{
	// TODO
}

void STFacebook::onPostPhoto(int tag,bool bSuccess)
{
	// TODO
	if(bSuccess)
	{
		STSystemFunction sf;
		sf.popAlertDialog("Post Successful!");
	}
	else
	{
		STSystemFunction sf;
		sf.popAlertDialog("Post Failed!");
	}

}

void STFacebook::onPostRichContent(int tag, bool bSuccess)
{
	//STAndroidUtility::getInstance()->log("C++:STFacebook","+++STFacebook::onPostRichContent()+++");
	if (bSuccess)
	{
//		QZDialog* dialog = QZDialog::create(450,210);
//		dialog->setContent(FACEBOOK_SUCCEED_MESSAGE,kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
//		dialog->addOneButton(STRING_OK,NULL,NULL);
//		dialog->show(true);



	}else
	{
//		QZDialog* dialog = QZDialog::create(450,210);
//		dialog->setContent(FACEBOOK_FAILED_MESSAGE,kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
//		dialog->addOneButton(STRING_OK,NULL,NULL);
//		dialog->show(true);
	}




	if(bSuccess)
	{
		STSystemFunction sf;
		sf.popAlertDialog("Post Successful!");
	}
	else
	{
		STSystemFunction sf;
		sf.popAlertDialog("Post Failed!");
	}


}

void STFacebook::onPostTimeOut(int tag)
{
//	QZDialog* dialog = QZDialog::create(450,210);
//	dialog->setContent(FACEBOOK_TIME_OUT_MESSAGE,kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop);
//	dialog->addOneButton(STRING_OK,NULL,NULL);
//	dialog->show(true);

	STSystemFunction sf;
	sf.popAlertDialog("Request time out!");


}

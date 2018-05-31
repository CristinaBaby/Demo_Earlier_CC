//
//  STFacebook.mm
//  SweetDiscovery
//
//  Created by zhangguangzong1 on 9/3/13.
//
//

#include "STFacebook.h"
#include "FaceBookManager.h"
STFacebook* STFacebook::getInstance()
{

}
void STFacebook::purgeInstance()
{

}

STFacebook::STFacebook()
{
}

STFacebook::~STFacebook()
{
}
void STFacebook::onPostStatus(int tag, bool bSuccess)
{

}
void STFacebook::onPostPhoto(int tag, bool bSuccess)
{

}
void STFacebook::onPostRichContent(int tag, bool bSuccess)
{

}
void STFacebook::onPostTimeOut(int tag)
{

}


void STFacebook::NativePostStatusUpdate(int tag, std::string text)
{

}
void STFacebook::NativePostPhotoFromAsset(int tag, std::string filename)
{

}
// FIXME: maybe this method will not be called(do not know drawable id)
void STFacebook::NativePostPhotoFromDrawable(int tag, int drawableId)
{

}
void STFacebook::NativePostPhotoFromFile(int tag, std::string filename)
{

}

void STFacebook::NativePostRichContentAsset(int tag, std::string message, std::string link, std::string assetFileName)
{

}
// FIXME: maybe this method will not be called(do not know drawable id)
void STFacebook::NativePostRichContentDrawable(int tag, std::string message, std::string link, int imgId)
{

}
void STFacebook::NativePostRichContentFile(int tag, std::string message, std::string link, std::string fileName)
{

}
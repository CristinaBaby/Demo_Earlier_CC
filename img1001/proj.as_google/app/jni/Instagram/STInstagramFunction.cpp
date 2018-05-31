/*
 * STInstagramFunction.cpp
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#include "../../../Classes/modules/STInstagramFunction.h"
#include "ST_InstagramFunction_Android.h"

STInstagramFunction::STInstagramFunction()
{
}
STInstagramFunction::~STInstagramFunction()
{
}


void STInstagramFunction::sendImageToInstagram(const char* imagePath)
{
    InstagramFunctionManager::getInstance()->sendImageToInstagram(imagePath);

}


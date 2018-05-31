/*
 * ST_SystemFunction_Android.h
 *
 *  Created on: 2013-9-2
 *      Author: Steven.Xc.Tian
 */
#ifndef ST_INSTAGRAMFUNCTION_ANDROID_H_
#define ST_INSTAGRAMFUNCTION_ANDROID_H_

#include "../utils/ST_JNI_Helper.h"
#include <string>
using namespace std;
class InstagramFunctionManager
{
public:
	InstagramFunctionManager() :
			ClassSF(0),
			stSFJava(0),
			MethodSendImageToInstagram(0)

	{
	}
	~InstagramFunctionManager();

	static InstagramFunctionManager* getInstance();

	static void destroy();

	bool init(JNIEnv * pEnv, jobject pJava);

public:

	void sendImageToInstagram(const char* path);


private:
	/*>>>>>>>> below declaration is used by jni <<<<<<<<*/
	// Cached Classes.
	jclass ClassSF;
	// Cached java object
	jobject stSFJava;
	// Cached active Methods.
	
	jmethodID MethodSendImageToInstagram;
};

#endif /* ST_SYSTEMFUNCTION_ANDROID_H_ */

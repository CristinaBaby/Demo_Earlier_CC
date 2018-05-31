#ifndef _STFACEBOOK_H_
#define _STFACEBOOK_H_
#include <string>

class STFacebook {
public:
	static STFacebook* getInstance();
	static void purgeInstance();

	virtual ~STFacebook();

public:
	// the below four methods used to inform game change status
	// and implements other game logic, and the source file
	// is placed in "Classes" directory
	void onPostStatus(int tag, bool bSuccess);
	void onPostPhoto(int tag, bool bSuccess);
	void onPostRichContent(int tag, bool bSuccess);
	void onPostTimeOut(int tag);

public:
	// in Android OS, the below methods need communicate with jni,
	// so the relative source file is placed in jni directory under
	// the proj.androd
	void NativePostStatusUpdate(int tag, std::string text);
	void NativePostPhotoFromAsset(int tag, std::string filename);
	// FIXME: maybe this method will not be called(do not know drawable id)
	void NativePostPhotoFromDrawable(int tag, int drawableId);
	void NativePostPhotoFromFile(int tag, std::string filename);

	void NativePostRichContentAsset(int tag, std::string message, std::string link, std::string assetFileName);
	// FIXME: maybe this method will not be called(do not know drawable id)
	void NativePostRichContentDrawable(int tag, std::string message, std::string link, int imgId);
	void NativePostRichContentFile(int tag, std::string message, std::string link, std::string fileName);

private:
	STFacebook();
};

#endif
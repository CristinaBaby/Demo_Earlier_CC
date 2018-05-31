#pragma once

class FacebookTool
{
public:
	static void postToFacebook(const char* msg, const char* imagePath=0, const char* url=0);
};
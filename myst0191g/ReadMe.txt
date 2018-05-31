此版本使用 cocosdx 版本为 2.1.5

将此文件夹check out至 COCOS2DX_ROOT/ 下。在命令行下，进入 COCOS2DX_ROOT 目录下，然后运行命令，比如：
"./timeout.py -project TIME011 -package net.timeoutapps.android_hog_dragon"

注意：应该使用 Android 包名来新建工程，在工程建好之后修改 iOS 版本的Bundle ID，因为Java并不认可连字符 "-"，所以使用iOS包名新建工程会导致Android工程出错。


大部分的配置信息都在 Class/AppConfig.h 文件中，要进行修改。


----------------------------------------------------------------------------------------------

在 proj.amazon 和 proj.google 两个工程中，请修改：
1. 公共库使用 cocos2dx-2.1.5 下面的 V1.3-2.1.5-MultiTouch
2. build_native.sh 文件，NDK_ROOT替换为自己的NDK根目录
3. AndroidManifest.xml文件，其中将中文 “用自己的包名替换” 替换为 自己的 package name, 和最上面那个 package 一致即可，可以全局查找中文然后替换全部。替换广告ID。
4. 修改 res/values/strings.xml 中 app_name
5. 替换 res/drawable-xxx 中的 icon.png 文件，仅仅只需要替换icon
6. 修改 src 文件夹下的 java 文件，google版本需要填 public key，其他只需要更改 getDebugMode() 中的返回值, 还有 getPlatformCode() 中的平台代码。



----------------------------------------------------------------------------------------------

在 proj.ios 工程中:
1. 在 info.plist 文件里修改 Bundle display name 为相应的Display Name 
2. 在 info.plist 文件里修改 URL Types，在time-后面添加bundle id 最后一部分。比如： time-[xxx]，xxx代表 bundle id 最后一个点后面的部分
3. 关于depends文件夹，在SVN中关联文件夹下的 depends-externals.txt 中的公共库，当然这个仅供参考，如有最新的版本，自己更换为所要求版本。在Cornerstone中，选中所选文件夹，在右侧窗口中的Properties标签中点击 Open Externals按键，在弹出的窗口中选择Property Text标签，把 depends-externals.txt 文件中的内容复制到里面，保存，更新即可。
4. 大部分的配置信息都在 AppConfig.h 文件中
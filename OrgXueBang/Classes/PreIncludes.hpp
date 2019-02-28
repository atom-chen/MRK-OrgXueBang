//
//  PreIncludes.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/7.
//

#ifndef PreIncludes_hpp
#define PreIncludes_hpp

#include <stdio.h>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "editor-support/cocostudio/CocoStudio.h"
using namespace cocostudio;

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
using namespace cocos2d::network;

#include <spine/spine-cocos2dx.h>
using namespace spine;

#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;


//book下载结果
#define MSG_BOOK_DOWN_RESULT                "msg_book_down_result"
#define MSG_BOOK_DOWN_PROGRESS              "msg_book_down_progress"


#endif /* PreIncludes_hpp */

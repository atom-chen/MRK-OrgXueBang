#!/usr/bin/env python
# _*_ coding:utf-8 _*_

import os,sys
dir_path = sys.path[0] + '/OCFiles/'

if not os.path.exists(dir_path):
    os.makedirs(dir_path)


#第一步:首先生成一个500位的数组 驼峰类型的元素 用作文件名 eg:AsdfdfGsd
import random
import os,sys
import string

random.seed()
first = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
second = "abcdefghijklmnopqrstuvwxyz"
number = "345"
index = 0
array = []
for i in range(500):
    final=(random.choice(first))
    index = random.randint(3, 5)
    for i in range(index):
        final+=(random.choice(second))
    final += (random.choice(first))
    for i in range(index):
        final+=(random.choice(second))
    array.append(final)


print (array)

#第二步:
#用上边生成的数组来创建对应的.h和.m文件
# -*- coding: utf-8 -*-
import random
import os
import string
#创建.h文件
def text_createH(fileNmae,msg,msg1,propertyNumber,methodArray,msg3):
    full_path = sys.path[0] + '/OCFiles/' + fileNmae + '.h'
    file = open(full_path, 'w')
    file.write('//\n//  '+fileNmae+'.h\n//  SajiaoShopping\n\n//  Created by 某 波 bo on 15/10/23.\n//  Copyright ©  2017年 阿里郎. All rights reserved.\n//\n\n')
    file.write(msg)
    file.write(msg1)
    propryNameArray = []
    for index in range(1,propertyNumber):
        propryNameArray.append(random.choice(array))
    propryNameArray = list(set(propryNameArray))
    for propertyName in propryNameArray:
        file.write('@property(nonatomic,strong)'+random.choice(classArray)+' * '+propertyName+';\n')
    file.write('\n\n')
    for methodName in methodArray:
        file.write('- (void)pushTo'+methodName+'VC:(NSDictionary *)info;\n')
    file.write(msg3)
    file.close()
    print('Done')
#创建.m文件
def text_createM(fileNmae,msg,msg1,methodArray,msg3):
    full_path = sys.path[0] + '/OCFiles/' + fileNmae + '.m'
    file = open(full_path, 'w')
    file.write('//\n//  '+fileNmae+'.m\n//  SajiaoShopping\n\n//  Created by 某 波 bo on 15/10/23.\n//  Copyright ©  2017年 阿里郎. All rights reserved.\n//\n\n')
    file.write(msg)
    file.write(msg1)
    for methodName in methodArray:
        file.write('- (void)pushTo'+methodName+'VC:(NSDictionary *)info\n{\n\n  NSMutableArray *array = [NSMutableArray array];\n')
        number = random.randint(3, 10)
        for i in range(1,number):
            file.write('  [array addObject:@"'+random.choice(array)+'"];\n')
        file.write('\n}\n\n')
    file.write(msg3)
    file.close()
    print('Done')

classArray = ['NSString','UILabel','NSDictionary','NSData','UIScrollView','UIView']
# array = ['HwxrFvrj', 'QnzduQbtdd', 'PvcrwLtqhf', 'UvdhDbjn', 'SuntmyTxvyzg', 'CvlxwBipbp', 'GzrdyzIbimvz', 'CqsjqMmgsp', 'OxaaeuWjhasc', 'NjiardRvwgbi', 'NcculmLtpljq', 'ApoqQrll', 'GkgokDyvjb', 'EblldkVouplj', 'KfdrFvnw', 'SfhyhObftc', 'SmruByoc', 'YzcccvXmpmit', 'OmqvaHpxat', 'XzytsUyvyd', 'MjforNnnyi', 'ZvjhuIdogs', 'BzfrxzSeahxc', 'PycycwFjtpny', 'XvngtoSedljr', 'DktiaCbucd', 'AqbplNuodc', 'MzkvgZuala', 'KdwzIoej', 'AaynatUpqcfd', 'IyvwhZvtjc', 'UmijGmsy', 'AoayndXxghym']
array = list(set(array))

for name in array:
    number = random.randint(3, 10)
    methodArray = []
    for i in range(1,5):
        methodArray.append(random.choice(array))
    methodArray = list(set(methodArray))#数组去重
    text_createH(name+'ViewController', '#import <UIKit/UIKit.h>\n','@interface '+name+ 'ViewController:'+ 'UIViewController\n\n',number,methodArray,'\n\n@end')
    text_createM(name+'ViewController', '#import "'+name+'ViewController.h"\n\n' '@interface '+ name+'ViewController()\n\n @end\n\n','@implementation '+name+'ViewController\n\n- (void)viewDidLoad { \n\n [super viewDidLoad];\n\n}\n\n',methodArray,'\n\n@end')


# 生产后直接把文件夹拖到xcode工程目录中





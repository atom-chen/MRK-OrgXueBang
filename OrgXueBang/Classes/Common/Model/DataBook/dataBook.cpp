//
//  dataBook.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/5.
//
//

#include "dataBook.h"

//std::string stuBookLevel::level2Cohort(){
//    std::string cohort("");
//    if (name.length() == 1) {
//        char c = name.at(0);
//        if (c >= 'A' && c <= 'Z') {
//            cohort= "RM" + name + "-001";
//        }
//    }
//    
//    return cohort;
//}

stuBookLevel::~bookLevel(){
    books.clear();
}

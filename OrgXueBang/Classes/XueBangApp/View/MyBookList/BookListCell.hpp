//
//  BookListCell.hpp
//  ReadingMate-mobile
//
//  Created by liuwei on 2018/4/9.
//

#ifndef BookListCell_hpp
#define BookListCell_hpp

class BOOK_INFO;
class BookListCell : public cocos2d::extension::TableViewCell
{
public:
    BookListCell();
    ~BookListCell();
    CREATE_FUNC(BookListCell);
    virtual bool init() override;
public:
    void updateCell(const BOOK_INFO& info);
    void rollTextNode(Node* cliperNode, Text* text);
private:
    cocos2d::Size m_winSize;
    cocos2d::Vec2 m_posBookTouch;
};

#endif /* BookListCell_hpp */

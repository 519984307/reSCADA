#ifndef LIST_OF_TAGS_H
#define LIST_OF_TAGS_H

#include <QObject>
#include <QList>
#include "tag.h"

class ListOfTags : public QList<Tag*>
{
public:
  void append( Tag* const tag){QList::append(tag);sort(*this);};
  void append( const QList<Tag*> tags){QList::append(tags);sort(*this); }
  void prepend( Tag* const tag){QList::prepend(tag);sort(*this); }
  void insert( int i, Tag* const tag){QList::insert(i,tag);sort(*this); }
  void replace( int i, Tag* const tag){QList::replace(i,tag);sort(*this); }
  int removeAll( Tag* const tag){int i; i = QList::removeAll(tag);sort(*this);return i; }
  //bool removeOne( Tag* const tag){bool i; i = QList::removeOne(tag);sort(*this);return i; }
  ListOfTags &operator+=(const ListOfTags &LofTag);
  inline ListOfTags operator+(const ListOfTags &LofTag) const { ListOfTags n = *this; n += LofTag; return n; }
  inline ListOfTags &operator +=( Tag* const tag) { append(tag); return *this; }
  inline ListOfTags &operator<< ( Tag* const tag){ append(tag); return *this; }
  inline ListOfTags &operator<<(const ListOfTags &LofTag){ *this += LofTag; return *this; }
  //std::function
  void (*sort)(QList<Tag*> &){ };
private:
  void move(int from, int to){ QList::move(from, to); sort(*this); }
  void swapItemsAt(int i, int j){ QList::swapItemsAt(i, j); sort(*this); }
};
#endif

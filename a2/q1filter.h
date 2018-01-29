#ifndef __Q1FILTER_H__
#define __Q1FILTER_H__ 1

_Coroutine Filter {
  protected:
    _Event Eof {};                    // no more characters
    Filter * next;                    // next filter in chain
    unsigned char ch;                 // communication variable
  public:
    void put( unsigned char c ) {
      ch = c;
      resume();
    }
};

#endif

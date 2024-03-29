//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2014 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: Stack.hxx 2838 2014-01-17 23:34:03Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef STACK_HXX
#define STACK_HXX

/**
 * Simple fixed size stack class.
 */
namespace Common {

template <class T, int MAX_SIZE = 10>
class FixedStack
{
  public:
    FixedStack<T, MAX_SIZE>() : _size(0) {}
	
    bool empty() const { return _size <= 0; }
    bool full() const {return _size >= MAX_SIZE; }
    void clear() { _size = 0; }
    void push(const T& x)
    {
      _stack[_size++] = x;
    }
    T top() const
    {
      if(_size > 0)
        return _stack[_size - 1];
      else
        return 0;
    }
    T pop()
    {
      T tmp;
      tmp = _stack[--_size];
      return tmp;
    }
    int size() const { return _size; }
    T operator [](int i) const
    {
      return _stack[i];
    }

  protected:
    T   _stack[MAX_SIZE];
    int _size;
};

}  // Namespace Common

#endif

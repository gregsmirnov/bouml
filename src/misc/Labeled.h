// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#ifndef LABELED_H
#define LABELED_H

#include <qintdict.h> 

// id 1 user 1
#define PROJECT_ID (1*128 + 1)

// id 2 user 0
#define FIRST_BASE_ID (2*128)

// id 2 user 1
#define FIRST_USER_ID (2*128 + 1)

// first 1000 items reserved for builtins
#define FIRST_ID 128000

template <class X> class Labeled;
template <class X> class IdIterator;
template <class X> class IdDict;

extern void set_in_import(bool y, bool as_lib = FALSE);
extern bool in_import();
extern bool in_lib_import();

extern int place(IdDict<void> & d, int id, void *);
extern int new_place(IdDict<void> & d, int user_id, void *);
extern void will_change_id(IdDict<void> & d, int &, void *);
extern void do_change_shared_ids();

// to add a margin on idmax after project loading
// to limit the possibility for a deleted element still
// present in a diagram known through its id to exist
// for an other element created after
extern void memo_idmax_loc(int & idmaxref, const char * who);
extern void idmax_add_margin();

template <class X> class IdDict {
#if 0
  // not managed by some C++ compiler
  template <class> friend class Labeled;
  template <class> friend class IdIterator;
  friend int place(IdDict<void> & d, int id, void *);
  friend int new_place(IdDict<void> & d, int user_id, void *);
  
  private:
#else
  public:
#endif
    QIntDict<X> dict[2];
    QIntDict<char> dictlib;
    int idmax;
    bool old_diagram;
    
  public:
    IdDict(const char * who) { idmax = 0; memo_idmax_loc(idmax, who); }
    IdDict(int sz, const char * who) { idmax = 0; dict[0].resize(sz); memo_idmax_loc(idmax, who); }
  
    X * operator[](int k) {
      return dict[(old_diagram || in_import()) ? 1 : 0][k];
    }
  
    void remove(int id) {
      dict[0].remove(id);
    }
    
    void clear(bool olds) {
      if (olds) {
	dict[1].clear();
	dictlib.clear();
      }
      else {
	idmax = FIRST_ID;
	dict[0].clear();
      }
    }
    
    void update_idmax_for_root() {
      QIntDictIterator<X> it(dict[0]); 
      
      while (it.current()) {
	int id = it.currentKey();
	
	if ((((unsigned) (id & ~127)) > ((unsigned) idmax)) &&
	    ((id & 127) == 0))
	  idmax = id & ~127;
	
	++it;
      }
    }
    
    void read_old_diagram(bool y) { old_diagram = y; }
    
    void memo_id_oid(int id, int oid) { 
      dict[0].remove(id);
      dictlib.replace(oid, (char *) id);
      
      if ((dictlib.count() / 2) >= dictlib.size())
	dictlib.resize(dictlib.size() * 2 - 1);
    }
};

template <class X> class IdIterator : public QIntDictIterator<X> {
  public:
    IdIterator(IdDict<X> & ids) : QIntDictIterator<X>(ids.dict[0]) {}
};

template <class X> class Labeled {
  private:
    int ident;
      
  protected:
    Labeled(IdDict<X> & d, int id) {
      ident = place((IdDict<void> &) d, id, (X *) this);
    }
    
  public:
    int get_ident() const { return ident; }
    bool is_api_base() { return ((ident & 127) == 0); }
    
    void new_ident(int user_id, IdDict<X> & d) {
      ident = new_place((IdDict<void> &) d, user_id, (X *) this);
    }
    void must_change_id(IdDict<X> & d) {
      will_change_id((IdDict<void> &) d, ident, (X *) this);
    }
};

//

extern void check_ids_cleared();

#endif

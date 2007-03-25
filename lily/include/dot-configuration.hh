/*
  dot-configuration.hh -- declare Dot_configuration

  Source file of the GNU LilyPond music typesetter.  Distributed under
  terms of the GNU General Public License.  LilyPond comes with NO
  WARRANTY.

  (c) 2007 Han-Wen Nienhuys <hanwen@xs4all.nl>
*/

#ifndef DOT_CONFIGURATION_HH
#define DOT_CONFIGURATION_HH

#include "lily-proto.hh"
#include "direction.hh"

#include <map>

struct Dot_position
{
  int pos_;
  Direction dir_;
  Grob *dot_;
  bool extremal_head_;

  Dot_position ()
  {
    dot_ = 0;
    pos_ = 0;
    dir_ = CENTER;
    extremal_head_ = false;
  }
};

struct Dot_configuration : public map<int, Dot_position>
{
  int badness () const;
  void print () const;
  Dot_configuration shifted (int k, Direction d) const;

    void remove_collision (int p);
};

#endif

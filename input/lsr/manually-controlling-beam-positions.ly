%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.35"

\header {
  lsrtags = "rhythms, tweaks-and-overrides"
 texidoc = "
Beam positions may be controlled manually, by overriding the
@code{positions} setting of the @code{Beam} grob.



" }
% begin verbatim
\score { 
    \context Voice \relative c {
	%% from upper staffline (position 4) to centre (position 0)
	\override Beam  #'positions = #'(2 . 0)
	 c'8[ c] 
	
	%% from center to one above centre (position 2)
	\override Beam  #'positions = #'(0 . 1)
	 c[ c]
  }

}



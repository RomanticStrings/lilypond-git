%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.35"

\header {
  lsrtags = "rhythms"
 texidoc = "
Lilypond can subdivide beams automatically. Set the property
subdivideBeams, and beams are subdivided at beat positions (as
specified in beat length)
" }
% begin verbatim
\relative{ b'32^"default"[ a g f c' b a g f e d' c b a g f ]
	       \set subdivideBeams = ##t
	       b32^"subdivision enabled"[ a g f c' b a g f e d' c b a g f ] 
	       \set Score.beatLength = #(ly:make-moment 1 8)
	       b32^"beatLength 1 8"[ a g f c' b a g f e d' c b a g f ]
	       \set Score.beatLength = #(ly:make-moment 1 16)
	       b32^"beatLength 1 16"[ a g f c' b a g f e d' c b a g f ]
    }


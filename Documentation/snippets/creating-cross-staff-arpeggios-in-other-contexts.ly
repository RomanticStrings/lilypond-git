%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "expressive-marks"

%% Translation of GIT committish: dff50e8e1d3134657a6b6203b9c93826dc4cef65
  texidoces = "
Se pueden crear arpegios que se cruzan entre pentagramas dentro de
contextos distintos a @code{PianoStaff} si se incluye el grabador
@code{Span_arpeggio_engraver} en el contexto de @code{Score}.

"
  doctitlees = "Creación de arpegios que se cruzan entre pentagramas dentro de otros contextos"
  
%% Translation of GIT committish: 6ce7f350682dfa99af97929be1dec6b9f1cbc01a
 texidocde = "
In einem Klaviersystem (@code{PianoStaff}) ist es möglich, ein Arpeggio
zwischen beiden Systemen zu verbinden, indem die
@code{PianoStaff.connectArpeggios}-Eigenschaft gesetzt wird.
 

"
  doctitlede = "Arpeggio zwischen Systemen in einem Klaviersystem erstellen"

  texidoc = "
Cross-staff arpeggios can be created in contexts other than
@code{PianoStaff} if the @code{Span_arpeggio_engraver} is included in
the @code{Score} context. 

"
  doctitle = "Creating cross-staff arpeggios in other contexts"
} % begin verbatim

\score {
  \new StaffGroup {
    \set Score.connectArpeggios = ##t
    <<
      \new Voice \relative c' {
        <c e>2\arpeggio
        <d f>2\arpeggio
        <c e>1\arpeggio
      }
      \new Voice  \relative c {
        \clef bass
         <c g'>2\arpeggio
         <b g'>2\arpeggio
         <c g'>1\arpeggio
      }
    >>
  }
  \layout {
    \context {
      \Score
      \consists "Span_arpeggio_engraver"
    }
  }
}

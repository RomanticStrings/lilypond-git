\version "2.19.14"

\header {
  lsrtags = "simultaneous-notes, tweaks-and-overrides"

  texidoc = "
If notes from two voices with stems in the same direction are placed at
the same position, but the voices have no voice-specific shifts
specified, the error message @samp{warning: ignoring too many clashing
note columns} will appear when compiling the LilyPond file.  This
message can be suppressed by setting the @code{'ignore-collision}
property of the @code{NoteColumn} object to @code{#t}. Please note that
this does not just suppress warnings  but stops LilyPond trying to
resolve collisions at all and so may have  unintended results unless
used with care.
"
  doctitle = "Suppressing warnings for clashing note columns"
}

ignore = \override NoteColumn.ignore-collision = ##t

\relative c' {
  \new Staff <<
    \new Voice{ \ignore \stemDown f2 g }
    \new Voice{ c2 \stemDown c, }
  >>
}

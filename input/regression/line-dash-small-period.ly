\header {

  texidoc = "Generate valid postscript even if dash-period is small
  compared to line thickness."

}

\version "2.11.29"
\relative {
  \override Staff.OttavaBracket #'dash-period = #0.1
  \override Score.OttavaBracket #'dash-fraction = #1

  #(set-octavation 1)
  c4 c
}


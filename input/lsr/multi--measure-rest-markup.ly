%% Do not edit this file; it is auto-generated from input/new
%% This file is in the public domain.
\version "2.13.1"

\header {
texidoces = "
Los elementos de marcado aplicados a un silencio multicompás se
centran encima o debajo de éste.  Los elementos de marcado extensos
que se adjuntan a silencios multicompás no producen la expansión del
compás.  Para expandir un silencio multicompás de forma que quepa todo
el marcado, utilice un silencio de separación con un marcado aplicado
antes del silencio multicompás.

Observe que el silencio separador produce la inserción de un compás.
El texto aplicado a un siencio sparador de esta forma se alinea por la
izquierda a la posición en que la nota estaría situada dentro del
compás, pero si la longitud del compás está determinada por la
longitud del texto, éste aparecerá centrado.

"

doctitlees = "Marcado de silencios multicompás"

%% Translation of GIT committish :<0364058d18eb91836302a567c18289209d6e9706>
  texidocde = "Textbeschriftungen, die an Mehrtaktpausen gehängt wird,
wird über oder unter der Pause zentriert.  Lange Beschriftungen
lassen den Takt nicht breiter werden.  Um eine Mehrtaktpause einer
Beschriftung anzupassen, muss eine unsichtbare Pause mit der
Beschriftung direkt vor der Mehrtkatpause eingesetzt werden.

Man sollte beachten, dass unsichtbare Pausen automatische Taktstriche
nach sich ziehen.  Text, der an eine unsichtbare Pause gehänt wird,
ist links ausgerichtet an der Position, wo die Pause erscheinen
würde.  Wenn aber die Länge des Taktes durch die Länge des Textes
bestimmt wird, sieht es so aus, als ob der Text zentriert gesetzt
ist."

  doctitlede = "Textbeschriftung und Mehrtaktpausen"


%% Translation of GIT committish :<e71f19ad847d3e94ac89750f34de8b6bb28611df>
  texidocfr = "
Lorsque du texte est attaché à un silence multi-mesures, il sera centré
dans la mesure, au-dessus ou en dessous de la portée.  Afin d'étirer la
mesure dans le cas ou ce texte est relativement long, il suffit
d'insérer un silence invisible auquel on attache le texte en question,
avant le silence multi-mesures.

Rappelez-vous qu'un silence invisible génère une barre de mesure.  Le
texte attaché à ce silence invisible sera alors aligné sur la gauche de
là où serait positionnée la note.  Cependant, si la taille de la mesure
est déterminée par la longueur du texte, il apparaîtra comme centré.

"
  doctitlefr = "Ajout de texte à un silence multi-mesures"

  lsrtags = "rhythms, text"
  texidoc = "Markups attached to a multi-measure rest will be
centered above or below it.  Long markups attached to multi-measure
rests do not cause the measure to expand.  To expand a multi-measure
rest to fit the markup, use a spacer rest with an attached markup
before the multi-measure rest.

Note that the spacer rest causes a bar line to be inserted.  Text attached
to a spacer rest in this way is left-aligned to the position where
the note would be placed in the measure, but if the measure length is
determined by the length of the text, the text will appear to be
centered."
  doctitle = "Multi-measure rest markup"
} % begin verbatim


\relative c' {
  \compressFullBarRests
  \textLengthOn
  s1*0^\markup { [MAJOR GENERAL] }
  R1*19
  s1*0_\markup { \italic { Cue: ... it is yours } }
  s1*0^\markup { A }
  R1*30^\markup { [MABEL] }
  \textLengthOff
  c4^\markup { CHORUS } d f c
}

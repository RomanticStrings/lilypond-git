\header{
filename =	 "sarabande-viola.ly";
title =	 "Solo Cello Suites";
subtitle = "Suite II";
% piece = "Sarabande";
% opus =	 "BWV 1008";
opus =	 "";
composer =	 "Johann Sebastian Bach (1685-1750)";
enteredby =	 "JCN";
copyright =	 "public domain";
}

%{
 Tested Features:breaking algorithm, chords, multivoice, accents, 
 dotted slurs
%}

\version "1.0.2";

\include "sarabande-urtext.ly";

sarabande_viola_global = \melodic{
	\time 3/4;
	\key f;
	\clef alto;
	\skip 2.*12;
	\bar ":|:";
	\skip 2.*16;
	\bar ":|";
}

sarabande_viola_scripts = \melodic{
}

sarabande_viola_staff = \type Staff <
	\melodic \transpose c' \$sarabande
	\$sarabande_viola_global
	\$sarabande_viola_scripts
>

\score{
	\$sarabande_viola_staff
	\include "scs-paper.ly";
	\midi{ \tempo 4 = 40; }
	\header{ piece = "Sarabande"; }
}



\version "1.3.110";

%
% Test new font selection and scm text markup
%

\score{
	\notes\relative c''{
		\stemUp
		a-"text"
		b-\textscript #"texta"
		c-\textscript #'(bold "textb")

		d-\textscript #'(lines "one" "two" "three")
		e-\textscript #'(lines (bold "one") 
		  (rows "and" "there" "is" ((font-family . number) "2"))
		  (italic "three"))
		f-\textscript #'(finger "3")
		g-\textscript #'(music (named "noteheads-2" "flags-u3"))
		
		b-#'(rows "a" ((kern . 3) ((raise . 2) "b")) "c")
		c-#'(rows "1" ((kern . -3) ((raise . -2) "2")) "3")
		
		}
	\paper{
		linewidth = -1.\mm;
		\translator{
			\ScoreContext
			TextScript \override #'font-family = #'roman
			TextScript \override #'font-shape = #'upright
			TextScript \revert #'no-spacing-rods
			TextScript \override #'direction = #1
		}
	}
}

%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.64"

\header {
  lsrtags = "rhythms, text"

  texidoc = "
This example shows how to add a markup command to get a three sided box
around some text (or other markup).

"
  doctitle = "Three-sided box"
} % begin verbatim

% New command to add a three sided box, with sides north, west and south
% Based on the box-stencil command defined in scm/stencil.scm
% Note that you use ";" to comment a line in Scheme
#(define-public (NWS-box-stencil stencil thickness padding)
  "Add a box around STENCIL, producing a new stencil."
  (let* ((x-ext (interval-widen (ly:stencil-extent stencil 0) padding))
	 (y-ext (interval-widen (ly:stencil-extent stencil 1) padding))
	 (y-rule (make-filled-box-stencil (cons 0 thickness) y-ext))
	 (x-rule (make-filled-box-stencil
		  (interval-widen x-ext thickness) (cons 0 thickness))))
;    (set! stencil (ly:stencil-combine-at-edge stencil X 1 y-rule padding))
    (set! stencil (ly:stencil-combine-at-edge stencil X -1 y-rule padding))
    (set! stencil (ly:stencil-combine-at-edge stencil Y 1 x-rule 0.0))  
    (set! stencil (ly:stencil-combine-at-edge stencil Y -1 x-rule 0.0))
    stencil))

% The corresponding markup command, based on the \box command defined 
% in scm/define-markup-commands.scm
#(define-markup-command (NWS-box layout props arg) (markup?)
  "Draw a box round @var{arg}.  Looks at @code{thickness},
@code{box-padding} and @code{font-size} properties to determine line
thickness and padding around the markup."
  
  (let* ((th (chain-assoc-get 'thickness props  0.1))
	 (size (chain-assoc-get 'font-size props 0))
	 (pad (* (magstep size)
		 (chain-assoc-get 'box-padding props 0.2)))
	 (m (interpret-markup layout props arg)))
    (NWS-box-stencil m th pad)))


% Test it:

\layout { ragged-right = ##f }

\relative c' { 
  c2^\markup { \NWS-box ABCD }
  c^\markup { \NWS-box \note #"4" #1.0 } 
}

/*
  This file is part of LilyPond, the GNU music typesetter.

  Copyright (C) 2005--2015 Han-Wen Nienhuys <hanwen@xs4all.nl>

  LilyPond is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LilyPond is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <unistd.h>

#include "lily-parser.hh"

#include "file-name-map.hh"
#include "file-name.hh"
#include "file-path.hh"
#include "international.hh"
#include "lily-lexer.hh"
#include "ly-module.hh"
#include "main.hh"
#include "program-option.hh"
#include "sources.hh"
#include "warn.hh"

LY_DEFINE (ly_parse_file, "ly:parse-file",
           1, 0, 0, (SCM name),
           "Parse a single @code{.ly} file."
           "  Upon failure, throw @code{ly-file-failed} key.")
{
  LY_ASSERT_TYPE (scm_is_string, name, 1);
  string file = ly_scm2string (name);
  char const *extensions[] = {"ly", "", 0};

  string file_name = global_path.find (file, extensions);

  /* By default, use base name of input file for output file name,
     write output to cwd; do not use root and directory parts of input
     file name.  */
  File_name out_file_name (file_name);

  out_file_name.ext_ = "";
  out_file_name.root_ = "";
  if (ly_get_option (ly_symbol2scm ("gui")) != SCM_BOOL_T
      && ly_get_option (ly_symbol2scm ("strip-output-dir")) == SCM_BOOL_T)
    {
      out_file_name.dir_ = "";
    }

  /* When running from gui, generate output in .ly source directory.  */
  string output_name = output_name_global;
  if (!output_name.empty ())
    {
      /* Interpret --output=DIR to mean --output=DIR/BASE.  */
      string dir;
      if (is_dir (output_name))
        {
          dir = output_name;
          output_name = "";
        }
      else
        {
          File_name out (output_name);
          dir = out.dir_part ();
          out_file_name = out.file_part ();
        }

      if (dir != "" && dir != "." && dir != get_working_directory ())
        {
          global_path.prepend (get_working_directory ());
          message (_f ("Changing working directory to: `%s'", dir));
          // If we can't change to the output dir (not existing, wrong
          // permissions), exit lilypond
          if (chdir (dir.c_str ()) != 0)
            error (_f ("unable to change directory to: `%s'", dir));
        }
      else
        out_file_name = File_name (output_name);
    }

  string init;
  if (!init_name_global.empty ())
    init = init_name_global;
  else
    init = "init.ly";

  string out_file = out_file_name.to_string ();
  if (init.length () && global_path.find (init).empty ())
    {
      warning (_f ("cannot find init file: `%s'", init));
      warning (_f ("(search path: `%s')",
                   global_path.to_string ().c_str ()));
      exit (2);
    }

  bool error = false;
  if ((file_name != "-") && file_name.empty ())
    {
      warning (_f ("cannot find file: `%s'", file));
      error = true;
    }
  else
    {
      Sources sources;
      sources.set_path (&global_path);

      string mapped_fn = map_file_name (file_name);
      basic_progress (_f ("Processing `%s'", mapped_fn.c_str ()));

      Lily_parser *parser = new Lily_parser (&sources);

      parser->parse_file (init, file_name, out_file);

      error = parser->error_level_;

      parser->clear ();
      parser->unprotect ();
    }

  /*
    outside the if-else to ensure cleanup fo Sources object,
  */
  if (error)
    /* TODO: pass renamed input file too.  */
    scm_throw (ly_symbol2scm ("ly-file-failed"),
               scm_list_1 (ly_string2scm (file_name)));

  return SCM_UNSPECIFIED;
}

LY_DEFINE (ly_parser_lexer, "ly:parser-lexer",
           1, 0, 0, (SCM parser_smob),
           "Return the lexer for @var{parser-smob}.")
{
  Lily_parser *parser = Lily_parser::unsmob (parser_smob);
  return parser->lexer_->self_scm ();
}

LY_DEFINE (ly_parser_clone, "ly:parser-clone",
           1, 2, 0, (SCM parser_smob, SCM closures, SCM location),
           "Return a clone of @var{parser-smob}.  An association list"
           " of port positions to closures can be specified in @var{closures}"
           " in order to have @code{$} and @code{#} interpreted in their original"
           " lexical environment.  If @var{location} is a valid location,"
           " it becomes the source of all music expressions inside.")
{
  LY_ASSERT_SMOB (Lily_parser, parser_smob, 1);
  Lily_parser *parser = Lily_parser::unsmob (parser_smob);
  if (SCM_UNBNDP (closures))
    closures = SCM_EOL;
  else
    LY_ASSERT_TYPE (ly_is_list, closures, 2);
  Lily_parser *clone = new Lily_parser (*parser, closures, location);

  return clone->unprotect ();
}

LY_DEFINE (ly_parser_define_x, "ly:parser-define!",
           3, 0, 0, (SCM parser_smob, SCM symbol, SCM val),
           "Bind @var{symbol} to @var{val} in @var{parser-smob}'s module.")
{

  LY_ASSERT_SMOB (Lily_parser, parser_smob, 1);
  Lily_parser *parser = Lily_parser::unsmob (parser_smob);

  LY_ASSERT_TYPE (ly_is_symbol, symbol, 2);

  parser->lexer_->set_identifier (scm_symbol_to_string (symbol), val);
  return SCM_UNSPECIFIED;
}

LY_DEFINE (ly_parser_lookup, "ly:parser-lookup",
           2, 0, 0, (SCM parser_smob, SCM symbol),
           "Look up @var{symbol} in @var{parser-smob}'s module."
           "  Return @code{'()} if not defined.")
{
  LY_ASSERT_SMOB (Lily_parser, parser_smob, 1);

  Lily_parser *parser = Lily_parser::unsmob (parser_smob);

  LY_ASSERT_TYPE (ly_is_symbol, symbol, 2);

  SCM val = parser->lexer_->lookup_identifier (ly_symbol2string (symbol));
  if (val != SCM_UNDEFINED)
    return val;
  else
    return SCM_EOL;
}

LY_DEFINE (ly_parser_parse_string, "ly:parser-parse-string",
           2, 0, 0, (SCM parser_smob, SCM ly_code),
           "Parse the string @var{ly-code} with @var{parser-smob}."
           "  Upon failure, throw @code{ly-file-failed} key.")
{
  LY_ASSERT_SMOB (Lily_parser, parser_smob, 1);
  Lily_parser *parser = Lily_parser::unsmob (parser_smob);
  LY_ASSERT_TYPE (scm_is_string, ly_code, 2);

  if (!parser->lexer_->is_clean ())
    parser->parser_error (_ ("ly:parser-parse-string is only valid with a new parser."
                             "  Use ly:parser-include-string instead."));
  else
    parser->parse_string (ly_scm2string (ly_code));

  return SCM_UNSPECIFIED;
}

LY_DEFINE (ly_parse_string_expression, "ly:parse-string-expression",
           2, 2, 0, (SCM parser_smob, SCM ly_code, SCM filename,
                     SCM line),
           "Parse the string @var{ly-code} with @var{parser-smob}."
           " Return the contained music expression."
           " @var{filename} and @var{line} are optional source indicators.")
{
  LY_ASSERT_SMOB (Lily_parser, parser_smob, 1);
  Lily_parser *parser = Lily_parser::unsmob (parser_smob);
  LY_ASSERT_TYPE (scm_is_string, ly_code, 2);
  string fn;
  if (SCM_UNBNDP (filename) || !scm_is_string (filename))
    fn = "<string>";
  else
    fn = ly_scm2string (filename);
  int ln;
  if (SCM_UNBNDP (line) || !scm_is_integer (line))
    ln = 0;
  else
    ln = scm_to_int (line);

  if (!parser->lexer_->is_clean ())
    {
      parser->parser_error (_ ("ly:parse-string-expression is only valid with a new parser."
                               "  Use ly:parser-include-string instead."));
      return SCM_UNSPECIFIED;
    }

  return parser->parse_string_expression (ly_scm2string (ly_code),
                                          fn, ln);
}

LY_DEFINE (ly_parser_include_string, "ly:parser-include-string",
           2, 0, 0, (SCM parser_smob, SCM ly_code),
           "Include the string @var{ly-code} into the input stream"
           " for @var{parser-smob}.  Can only be used in immediate"
           " Scheme expressions (@code{$} instead of @code{#}).")
{
  LY_ASSERT_SMOB (Lily_parser, parser_smob, 1);
  Lily_parser *parser = Lily_parser::unsmob (parser_smob);
  LY_ASSERT_TYPE (scm_is_string, ly_code, 2);

  parser->include_string (ly_scm2string (ly_code));

  return SCM_UNSPECIFIED;
}

LY_DEFINE (ly_parser_set_note_names, "ly:parser-set-note-names",
           2, 0, 0, (SCM parser, SCM names),
           "Replace current note names in @var{parser}."
           "  @var{names} is an alist of symbols.  This only has effect"
           " if the current mode is notes.")
{
  LY_ASSERT_SMOB (Lily_parser, parser, 1);
  Lily_parser *p = Lily_parser::unsmob (parser);

  if (p->lexer_->is_note_state ())
    {
      p->lexer_->pop_state ();
      p->lexer_->push_note_state (names);
    }

  return SCM_UNSPECIFIED;
}

LY_DEFINE (ly_parser_output_name, "ly:parser-output-name",
           1, 0, 0, (SCM parser),
           "Return the base name of the output file.")
{
  LY_ASSERT_SMOB (Lily_parser, parser, 1);
  Lily_parser *p = Lily_parser::unsmob (parser);

  return ly_string2scm (p->output_basename_);
}

LY_DEFINE (ly_parser_error, "ly:parser-error",
           2, 1, 0, (SCM parser, SCM msg, SCM input),
           "Display an error message and make the parser fail.")
{
  LY_ASSERT_SMOB (Lily_parser, parser, 1);
  Lily_parser *p = Lily_parser::unsmob (parser);

  LY_ASSERT_TYPE (scm_is_string, msg, 2);
  string s = ly_scm2string (msg);

  Input *i = Input::unsmob (input);
  if (i)
    p->parser_error (*i, s);
  else
    p->parser_error (s);

  return parser;
}

LY_DEFINE (ly_parser_clear_error, "ly:parser-clear-error",
           1, 0, 0, (SCM parser),
           "Clear the error flag for the parser.")
{
  LY_ASSERT_SMOB (Lily_parser, parser, 1);
  Lily_parser *p = Lily_parser::unsmob (parser);

  p->error_level_ = 0;
  p->lexer_->error_level_ = 0;

  return SCM_UNSPECIFIED;
}

LY_DEFINE (ly_parser_has_error_p, "ly:parser-has-error?",
           1, 0, 0, (SCM parser),
           "Does @var{parser} have an error flag?")
{
  LY_ASSERT_SMOB (Lily_parser, parser, 1);
  Lily_parser *p = Lily_parser::unsmob (parser);

  return scm_from_bool (p->error_level_ || p->lexer_->error_level_);
}

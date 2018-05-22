/*************************************************************************
 *
 * Copyright (C) 2014-2018 Barbara Geller & Ansel Sermersheim
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by DoxyPress are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#include "mainwindow.h"

#include <QRegularExpression>

void MainWindow::convertDoxy(QByteArray data)
{
   QString tempStr;
   QString tempText;
   bool tempBool;
   int tempInt;

   // tab 1
   tempStr = convert_Str(data,"PROJECT_NAME");
   m_ui->project_name->setText(tempStr);

   tempStr = convert_Str(data,"PROJECT_BRIEF");
   m_ui->project_brief->setText(tempStr);

   tempStr = convert_Str(data,"PROJECT_NUMBER");
   m_ui->project_version->setText(tempStr);

   tempStr= convert_Str(data, "PROJECT_LOGO");
   m_ui->project_logo->setText(tempStr);

   tempStr = convert_Str(data,"OUTPUT_DIRECTORY");
   m_ui->output_dir->setText(tempStr);

   //
   bool isCpp = true;

   tempBool = convert_Bool(data,"OPTIMIZE_OUTPUT_JAVA");
   m_ui->optimize_java_RB->setChecked(tempBool);

   if (tempBool)  {
      isCpp = false;
   }

   tempBool = convert_Bool(data,"OPTIMIZE_OUTPUT_FOR_C");
   m_ui->optimize_c_RB->setChecked(tempBool);

   if (tempBool)  {
      isCpp = false;
   }

   tempBool = convert_Bool(data,"OPTIMIZE_FOR_FORTRAN");
   m_ui->optimize_fortran_RB->setChecked(tempBool);

   if (tempBool)  {
      isCpp = false;
   }

   tempBool = convert_Bool(data,"OPTIMIZE_FOR_PYTHON");
   m_ui->optimize_python_RB->setChecked(tempBool);

   if (tempBool)  {
      isCpp = false;
   }

   if (isCpp)  {
      m_ui->optimize_cplus_RB->setChecked(true);
   }

   //
   tempBool = convert_Bool(data,"GENERATE_HTML");
   m_ui->gen_html_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_LATEX");
   m_ui->gen_latex_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_RTF");;
   m_ui->gen_rtf_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_MAN");
   m_ui->gen_man_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_XML");
   m_ui->gen_xml_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_DOCBOOK");
   m_ui->gen_docbook_CB1->setChecked(tempBool);

   //
   tempBool = convert_Bool(data, "CLASS_GRAPH");
   m_ui->dot_class_graph_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"COLLABORATION_GRAPH");
   m_ui->dot_collaboration_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data, "GRAPHICAL_HIERARCHY");
   m_ui->dot_hierarchy_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"INCLUDE_GRAPH");
   m_ui->dot_include_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"INCLUDED_BY_GRAPH");
   m_ui->dot_included_by_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"CALL_GRAPH");
   m_ui->dot_call_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"CALLER_GRAPH");
   m_ui->dot_called_by_CB1->setChecked(tempBool);


   //  ***
   // tab 2 - general
   tempBool = convert_Bool(data, "CREATE_SUBDIRS");
   m_ui->create_subdirs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "ALLOW_UNICODE_NAMES");
   m_ui->allow_unicode_names_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "OUTPUT_LANGUAGE");
   tempInt = m_ui->output_language_CM->findText(tempStr);
   m_ui->output_language_CM->setCurrentIndex(tempInt);

   tempBool = convert_Bool(data, "BRIEF_MEMBER_DESC");
   m_ui->brief_member_desc_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "REPEAT_BRIEF");
   m_ui->repeat_brief_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"ABBREVIATE_BRIEF");
   m_ui->abbreviate_brief->setPlainText(tempText);

   tempBool = convert_Bool(data, "ALWAYS_DETAILED_SEC");
   m_ui->always_detailed_sec_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "INLINE_INHERITED_MEMB");
   m_ui->inline_inherited_member_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "FULL_PATH_NAMES");
   m_ui->full_path_names_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"STRIP_FROM_PATH");
   m_ui->strip_from_path->setPlainText(tempText);

   tempText = convert_PlainText(data,"STRIP_FROM_INC_PATH");
   m_ui->strip_from_inc_path->setPlainText(tempText);

   tempBool = convert_Bool(data, "SHORT_NAMES");
   m_ui->short_names_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "JAVADOC_AUTOBRIEF");
   m_ui->javadoc_auto_brief_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "QT_AUTOBRIEF");
   m_ui->qt_auto_brief_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "MULTILINE_CPP_IS_BRIEF");
   m_ui->multiline_cpp_brief_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "INHERIT_DOCS");
   m_ui->inherit_docs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SEPARATE_MEMBER_PAGES");
   m_ui->separate_member_pages_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "TAB_SIZE");   
   m_ui->tab_size_SB->setValue(tempInt);

   tempText = convert_PlainText(data,"ALIASES");
   m_ui->aliases->setPlainText(tempText);

   tempText = convert_PlainText(data,"TCL_SUBST");
   m_ui->tcl_subst->setPlainText(tempText);

   tempText = convert_PlainText(data,"EXTENSION_MAPPING");
   m_ui->language_mapping->setPlainText(tempText);

   tempBool = convert_Bool(data, "MARKDOWN_SUPPORT");
   m_ui->markdown_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "AUTOLINK_SUPPORT");
   m_ui->auto_link_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "BUILTIN_STL_SUPPORT");
   m_ui->built_in_stl_support_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "CPP_CLI_SUPPORT");
   m_ui->cpp_cli_support_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SIP_SUPPORT");
   m_ui->sip_support_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "IDL_PROPERTY_SUPPORT");
   m_ui->idl_support_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "DISTRIBUTE_GROUP_DOC");
   m_ui->duplicate_docs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GROUP_NESTED_COMPOUNDS");
   m_ui->group_nested_compounds_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SUBGROUPING");
   m_ui->allow_sub_grouping_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "INLINE_GROUPED_CLASSES");
   m_ui->inline_grouped_classes_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "INLINE_SIMPLE_STRUCTS");
   m_ui->inline_simple_struct_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "TYPEDEF_HIDES_STRUCT");
   m_ui->use_typedef_name_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "LOOKUP_CACHE_SIZE");
   m_ui->lookup_cache_size_SB->setValue(tempInt);


   // tab 2 - build
   tempBool = convert_Bool(data, "EXTRACT_ALL");
   m_ui->extract_all_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTRACT_PRIVATE");
   m_ui->extract_private_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTRACT_PACKAGE");
   m_ui->extract_package_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTRACT_STATIC");
   m_ui->extract_static_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTRACT_LOCAL_CLASSES");
   m_ui->extract_local_classes_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTRACT_LOCAL_METHODS");
   m_ui->extract_local_methods_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTRACT_ANON_NSPACES");
   m_ui->extract_anon_namespaces_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "HIDE_UNDOC_MEMBERS");
   m_ui->hide_undoc_members_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "HIDE_UNDOC_CLASSES");
   m_ui->hide_undoc_classes_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "HIDE_FRIEND_COMPOUNDS");
   m_ui->hide_friend_compounds_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "HIDE_IN_BODY_DOCS");
   m_ui->hide_in_body_docs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "HIDE_SCOPE_NAMES");
   m_ui->hide_scope_names_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "HIDE_COMPOUND_REFERENCE");
   m_ui->hide_compound_ref_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "INTERNAL_DOCS");
   m_ui->internal_docs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "CASE_SENSE_NAMES");
   m_ui->case_sensitive_fname_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SHOW_INCLUDE_FILES");
   m_ui->show_include_files_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SHOW_GROUPED_MEMB_INC");
   m_ui->show_grouped_members_inc_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "FORCE_LOCAL_INCLUDES");
   m_ui->force_local_includes_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "INLINE_INFO");
   m_ui->inline_info_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SORT_MEMBER_DOCS");
   m_ui->sort_member_docs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SORT_BRIEF_DOCS");
   m_ui->sort_brief_docs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SORT_MEMBERS_CTORS_1ST");
   m_ui->sort_constructors_first_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SORT_GROUP_NAMES");
   m_ui->sort_group_names_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SORT_BY_SCOPE_NAME");
   m_ui->sort_by_scope_name_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "STRICT_PROTO_MATCHING");
   m_ui->strict_sig_matching_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GENERATE_TODOLIST");
   m_ui->gen_todo_list_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GENERATE_TESTLIST");
   m_ui->gen_test_list_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GENERATE_BUGLIST");
   m_ui->gen_bug_list_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GENERATE_DEPRECATEDLIST");
   m_ui->gen_deprecate_list_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"ENABLED_SECTIONS");
   m_ui->enabled_sections->setPlainText(tempText);

   tempInt = convert_Int(data, "MAX_INITIALIZER_LINES");
   m_ui->max_init_lines_SB->setValue(tempInt);

   tempBool = convert_Bool(data, "SHOW_USED_FILES");
   m_ui->show_used_files_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SHOW_FILES");
   m_ui->show_file_page_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SHOW_NAMESPACES");
   m_ui->show_namespace_page_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "FILE_VERSION_FILTER");
   m_ui->file_version_filter->setText(tempStr);

   tempStr = convert_Str(data, "LAYOUT_FILE");
   m_ui->layout_file->setText(tempStr);

   tempText = convert_PlainText(data,"CITE_BIB_FILES");
   m_ui->cite_bib_files->setPlainText(tempText);


   // tab 2 -input
   tempStr = convert_PlainText(data,"INPUT");
   m_ui->input_source->setPlainText(tempStr);

   tempStr = convert_Str(data,"INPUT_ENCODING");
   m_ui->input_encoding->setText(tempStr);

   tempText = convert_PlainText(data,"FILE_PATTERNS");
   m_ui->input_patterns->setPlainText(tempText);

   tempBool = convert_Bool(data,"RECURSIVE");
   m_ui->input_recursive_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"EXCLUDE");
   m_ui->exclude_files->setPlainText(tempText);

   tempBool = convert_Bool(data, "EXCLUDE_SYMLINKS");
   m_ui->exclude_symlinks_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"EXCLUDE_PATTERNS");
   m_ui->exclude_patterns->setPlainText(tempText);

   tempText = convert_PlainText(data,"EXCLUDE_SYMBOLS");
   m_ui->exclude_symbols->setPlainText(tempText);

   tempText = convert_PlainText(data,"EXAMPLE_PATH");
   m_ui->example_source->setPlainText(tempText);

   tempText = convert_PlainText(data,"EXAMPLE_PATTERNS");
   m_ui->example_patterns->setPlainText(tempText);

   tempBool = convert_Bool(data, "EXAMPLE_RECURSIVE");
   m_ui->example_recursive_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"IMAGE_PATH");
   m_ui->image_path->setPlainText(tempText);

   tempStr = convert_Str(data, "INPUT_FILTER");
   m_ui->filter_program->setText(tempStr);

   tempText = convert_PlainText(data,"FILTER_PATTERNS");
   m_ui->filter_patterns->setPlainText(tempText);

   tempBool = convert_Bool(data, "FILTER_SOURCE_FILES");
   m_ui->filter_source_files_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"FILTER_SOURCE_PATTERNS");
   m_ui->filter_source_patterns->setPlainText(tempText);

   tempStr = convert_Str(data, "USE_MDFILE_AS_MAINPAGE");
   m_ui->mdfile_mainpage->setText(tempStr);


   // tab 2 -index
   tempBool = convert_Bool(data, "ALPHABETICAL_INDEX");
   m_ui->alpha_index_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "COLS_IN_ALPHA_INDEX");
   m_ui->cols_in_index_SB->setValue(tempInt);

   tempText = convert_PlainText(data,"IGNORE_PREFIX");
   m_ui->ignore_prefix->setPlainText(tempText);



   // tab 2 - messages
   tempBool = convert_Bool(data, "QUIET");
   m_ui->quiet_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARNINGS");
   m_ui->warnings_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARN_IF_UNDOCUMENTED");
   m_ui->warn_undoc_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARN_IF_DOC_ERROR");
   m_ui->warn_doc_error_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "WARN_NO_PARAMDOC");
   m_ui->warn_undoc_param_CB->setChecked(tempBool);

   tempStr = convert_Str(data,"WARN_FORMAT");
   m_ui->warn_format->setText(tempStr);

   tempStr = convert_Str(data,"WARN_LOGFILE");
   m_ui->warn_logfile->setText(tempStr);


   // tab 2 -source code
   tempBool = convert_Bool(data, "SOURCE_BROWSER");
   m_ui->source_code_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "INLINE_SOURCES");
   m_ui->inline_source_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "STRIP_CODE_COMMENTS");
   m_ui->strip_code_comments_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "REFERENCED_BY_RELATION");
   m_ui->ref_by_relation_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "REFERENCES_RELATION");
   m_ui->ref_relation_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "REFERENCES_LINK_SOURCE");
   m_ui->ref_link_source_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SOURCE_TOOLTIPS");
   m_ui->source_tooltips_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "USE_HTAGS");
   m_ui->use_htags_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "VERBATIM_HEADERS");
   m_ui->verbatim_headers_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "CLANG_ASSISTED_PARSING");
   m_ui->clang_parsing_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"CLANG_OPTIONS");
   m_ui->clang_flags->setPlainText(tempText);


   // tab 2 - preprocess
   tempBool = convert_Bool(data, "ENABLE_PREPROCESSING");
   m_ui->enable_preprocessing_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "MACRO_EXPANSION");
   m_ui->macro_expansion_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXPAND_ONLY_PREDEF");
   m_ui->expand_only_predefined_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "SEARCH_INCLUDES");
   m_ui->search_includes_CB->setChecked(tempBool);

   tempText = convert_PlainText(data,"INCLUDE_PATH");
   m_ui->include_path->setPlainText(tempText);

   tempText = convert_PlainText(data,"INCLUDE_FILE_PATTERNS");
   m_ui->include_patterns->setPlainText(tempText);

   tempText = convert_PlainText(data,"PREDEFINED");
   m_ui->predefined_macros->setPlainText(tempText);

   tempText = convert_PlainText(data,"EXPAND_AS_DEFINED");
   m_ui->expand_as_defined->setPlainText(tempText);

   tempBool = convert_Bool(data, "SKIP_FUNCTION_MACROS");
   m_ui->skip_function_macros_CB->setChecked(tempBool);


   // tab 2 - external
   tempText = convert_PlainText(data,"TAGFILES");
   m_ui->tag_files->setPlainText(tempText);

   tempText = convert_PlainText(data,"GENERATE_TAGFILE");
   m_ui->gen_tagfile->setText(tempText);

   tempBool = convert_Bool(data, "ALLEXTERNALS");
   m_ui->all_externals_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTERNAL_GROUPS");
   m_ui->external_groups_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTERNAL_PAGES");
   m_ui->external_pages_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "PERL_PATH");
   m_ui->perl_path->setText(tempStr);


   // tab 2 - dot
   tempBool = convert_Bool(data,"CLASS_DIAGRAMS");
   m_ui->class_diagrams_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "MSCGEN_PATH");
   m_ui->mscgen_path->setText(tempStr);

   tempStr = convert_Str(data, "DIA_PATH");
   m_ui->dia_path->setText(tempStr);

   tempBool = convert_Bool(data, "HIDE_UNDOC_RELATIONS");
   m_ui->hide_undoc_relations_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "HAVE_DOT");
   m_ui->have_dot_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "DOT_NUM_THREADS");
   m_ui->dot_num_threads_SB->setValue(tempInt);

   tempStr = convert_Str(data, "DOT_FONTNAME");
   m_ui->dot_font_name->setText(tempStr);

   tempInt = convert_Int(data, "DOT_FONTSIZE");
   m_ui->dot_font_size_SB->setValue(tempInt);

   tempStr = convert_Str(data, "DOT_FONTPATH ");
   m_ui->dot_font_path->setText(tempStr);

   tempBool = convert_Bool(data, "GROUP_GRAPHS");
   m_ui->group_graphs_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "UML_LOOK");
   m_ui->uml_look_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "UML_LIMIT_NUM_FIELDS");
   m_ui->uml_limit_num_fields_SB->setValue(tempInt);

   tempBool = convert_Bool(data, "TEMPLATE_RELATIONS");
   m_ui->template_relations_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "DIRECTORY_GRAPH");
   m_ui->directory_graph_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "DOT_IMAGE_FORMAT");
   tempInt = m_ui->dot_image_format_CM->findText(tempStr);
   m_ui->dot_image_format_CM->setCurrentIndex(tempInt);

   tempBool = convert_Bool(data, "INTERACTIVE_SVG");
   m_ui->interactive_svg_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "DOT_PATH");
   m_ui->dot_path->setText(tempStr);

   tempText = convert_PlainText(data,"DOTFILE_DIRS");
   m_ui->dot_file_dirs->setPlainText(tempText);

   tempText = convert_PlainText(data,"MSCFILE_DIRS");
   m_ui->msc_file_dirs->setPlainText(tempText);

   tempText = convert_PlainText(data,"DIAFILE_DIRS");
   m_ui->dia_file_dirs->setPlainText(tempText);

   tempStr = convert_Str(data, "PLANTUML_JAR_PATH");
   m_ui->plantuml_jar_path->setText(tempStr);

   tempStr = convert_Str(data, "PLANTUML_CFG_FILE");
   m_ui->plantuml_cfg_file->setText(tempStr);

   tempStr = convert_Str(data, "PLANTUML_INCLUDE_PATH");
   m_ui->plantuml_inc_path->setPlainText(tempStr);

   tempInt = convert_Int(data, "DOT_GRAPH_MAX_NODES");
   m_ui->dot_graph_max_nodes_SB->setValue(tempInt);

   tempInt = convert_Int(data, "MAX_DOT_GRAPH_DEPTH");
   m_ui->dot_graph_max_depth_SB->setValue(tempInt);

   tempBool = convert_Bool(data, "DOT_TRANSPARENT");
   m_ui->dot_transparent_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "DOT_MULTI_TARGETS");
   m_ui->dot_multiple_targets_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GENERATE_LEGEND");
   m_ui->gen_legend_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "DOT_CLEANUP");
   m_ui->dot_cleanup_CB->setChecked(tempBool);


   //  ***
   // tab 3 - html
   tempStr = convert_Str(data, "HTML_OUTPUT");
   m_ui->html_output->setText(tempStr);

   tempStr = convert_Str(data, "HTML_FILE_EXTENSION");
   m_ui->html_file_extension->setText(tempStr);

   tempStr = convert_Str(data, "HTML_HEADER");
   m_ui->html_header->setText(tempStr);

   tempStr = convert_Str(data, "HTML_FOOTER");
   m_ui->html_footer->setText(tempStr);

   // removed - add to tempText
   tempStr  = convert_Str(data, "HTML_STYLESHEET");
   tempText = convert_PlainText(data,"HTML_EXTRA_STYLESHEET");

   if (! tempStr.isEmpty()) {

      if (tempText.isEmpty()) {
         tempText = tempStr;
      } else {
         tempText = tempStr + ", " + tempText;
      }
   }
   m_ui->html_stylesheets->setPlainText(tempText);

   tempText = convert_PlainText(data,"HTML_EXTRA_FILES");
   m_ui->html_extra_files->setPlainText(tempText);

   tempInt = convert_Int(data, "HTML_COLORSTYLE_HUE");
   m_ui->html_colorstyle_hue->setValue(tempInt);

   tempInt = convert_Int(data, "HTML_COLORSTYLE_SAT");
   m_ui->html_colorstyle_sat->setValue(tempInt);

   tempInt = convert_Int(data, "HTML_COLORSTYLE_GAMMA");
   m_ui->html_colorstyle_gamma->setValue(tempInt);

   tempBool = convert_Bool(data, "HTML_TIMESTAMP");
   m_ui->html_timestamp_CB->setChecked(tempBool);    

   tempBool = convert_Bool(data, "HTML_DYNAMIC_SECTIONS");
   m_ui->html_dynamic_sections_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "HTML_INDEX_NUM_ENTRIES");
   m_ui->html_index_num_entries_SB->setValue(tempInt);

   tempBool = convert_Bool(data,"GENERATE_DOCSET");
   m_ui->gen_docbook_CB1->setChecked(tempBool);

   tempStr = convert_Str(data, "DOCSET_FEEDNAME");
   m_ui->docset_feedname->setText(tempStr);

   tempStr = convert_Str(data, "DOCSET_BUNDLE_ID");
   m_ui->docset_bundle_id->setText(tempStr);

   tempStr = convert_Str(data, "DOCSET_PUBLISHER_ID");
   m_ui->docset_publisher_id->setText(tempStr);

   tempStr = convert_Str(data, "DOCSET_PUBLISHER_NAME");
   m_ui->docset_publisher_name->setText(tempStr);

   tempBool = convert_Bool(data, "GENERATE_HTMLHELP");
   m_ui->gen_chm_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "CHM_FILE");
   m_ui->chm_file->setText(tempStr);

   tempStr = convert_Str(data, "HHC_LOCATION");
   m_ui->hhc_location->setText(tempStr);

   tempBool = convert_Bool(data, "GENERATE_CHI");
   m_ui->gen_chi_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "CHM_INDEX_ENCODING");
   m_ui->chm_index_encoding->setText(tempStr);

   tempBool = convert_Bool(data, "BINARY_TOC");
   m_ui->binary_toc_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "TOC_EXPAND");
   m_ui->toc_expanded_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GENERATE_QHP");
   m_ui->gen_qthelp_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "QCH_FILE");
   m_ui->qch_file->setText(tempStr);

   tempStr = convert_Str(data, "QHP_NAMESPACE");
   m_ui->qhp_namespace->setText(tempStr);

   tempStr = convert_Str(data, "QHP_VIRTUAL_FOLDER");
   m_ui->qhp_virtual_folder->setText(tempStr);

   tempStr = convert_Str(data, "QHP_CUST_FILTER_NAME");
   m_ui->qhp_cust_filter_name->setText(tempStr);

   tempStr = convert_Str(data, "QHP_CUST_FILTER_ATTRS");
   m_ui->qhp_cust_attrib->setPlainText(tempStr);

   tempStr = convert_Str(data, "QHP_SECT_FILTER_ATTRS");
   m_ui->qhp_sect_attrib->setPlainText(tempStr);

   tempStr = convert_Str(data, "QHG_LOCATION");
   m_ui->qthelp_gen_path->setText(tempStr);

   tempBool = convert_Bool(data, "GENERATE_ECLIPSEHELP");
   m_ui->gen_eclipse_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "ECLIPSE_DOC_ID");
   m_ui->eclipse_doc_id->setText(tempStr);

   tempBool = convert_Bool(data, "DISABLE_INDEX");
   m_ui->disable_index_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "GENERATE_TREEVIEW");
   m_ui->gen_treeview_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "ENUM_VALUES_PER_LINE");
   m_ui->enum_values_per_line_SB->setValue(tempInt);

   tempInt = convert_Int(data, "TREEVIEW_WIDTH");
   m_ui->treeview_width_SB->setValue(tempInt);

   tempBool = convert_Bool(data, "EXT_LINKS_IN_WINDOW");
   m_ui->external_links_in_window_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "FORMULA_FONTSIZE");
   m_ui->formula_fontsize_SB->setValue(tempInt);

   tempBool = convert_Bool(data, "FORMULA_TRANSPARENT");
   m_ui->formula_transparent_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "USE_MATHJAX");
   m_ui->use_mathjax_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "MATHJAX_FORMAT");
   tempInt = m_ui->mathjax_format_CM->findText(tempStr);
   m_ui->mathjax_format_CM->setCurrentIndex(tempInt);

   tempStr = convert_Str(data, "MATHJAX_RELPATH");
   m_ui->mathjax_relpath->setText(tempStr);

   tempText = convert_PlainText(data,"MATHJAX_EXTENSIONS");
   m_ui->mathjax_extensions->setPlainText(tempText);

   tempStr = convert_Str(data, "MATHJAX_CODEFILE");
   m_ui->mathjax_codefile->setText(tempStr);

   tempBool = convert_Bool(data, "SEARCHENGINE");
   m_ui->html_search_CB2->setChecked(tempBool);

   tempBool = convert_Bool(data, "SERVER_BASED_SEARCH");
   m_ui->search_server_based_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "EXTERNAL_SEARCH");
   m_ui->search_external_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "SEARCHENGINE_URL");
   m_ui->search_external_url->setText(tempStr);

   tempStr = convert_Str(data, "SEARCHDATA_FILE");
   m_ui->search_data_file->setText(tempStr);

   tempStr = convert_Str(data, "EXTERNAL_SEARCH_ID");
   m_ui->search_external_id->setText(tempStr);

   tempText = convert_PlainText(data,"EXTRA_SEARCH_MAPPING");
   m_ui->search_mappings->setPlainText(tempText);


   // tab 3 - latex
   tempStr = convert_Str(data, "LATEX_OUTPUT");
   m_ui->latex_output->setText(tempStr);

   tempStr = convert_Str(data, "LATEX_CMD_NAME");
   m_ui->latex_cmd_name->setText(tempStr);

   tempStr = convert_Str(data, "MAKEINDEX_CMD_NAME");
   m_ui->make_index_cmd_name->setText(tempStr);

   tempBool = convert_Bool(data, "COMPACT_LATEX");
   m_ui->latex_compact_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "PAPER_TYPE");
   tempInt = m_ui->latex_paper_type_CM->findText(tempStr);
   m_ui->latex_paper_type_CM->setCurrentIndex(tempInt);

   tempText = convert_PlainText(data,"EXTRA_PACKAGES");
   m_ui->latex_extra_packages->setPlainText(tempText);

   tempStr = convert_Str(data, "LATEX_HEADER");
   m_ui->latex_header->setText(tempStr);

   tempStr = convert_Str(data, "LATEX_FOOTER");
   m_ui->latex_footer->setText(tempStr);

   tempText = convert_PlainText(data,"LATEX_EXTRA_STYLESHEET");
   m_ui->latex_stylesheets->setPlainText(tempText);

   tempText = convert_PlainText(data,"LATEX_EXTRA_FILES");
   m_ui->latex_extra_files->setPlainText(tempText);

   // new field
   m_ui->latex_timestamp_CB->setChecked(m_ui->html_timestamp_CB->isChecked());

   tempBool = convert_Bool(data, "PDF_HYPERLINKS");
   m_ui->latex_hyper_pdf_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "USE_PDFLATEX");
   m_ui->latex_pdf_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "LATEX_BATCHMODE");
   m_ui->latex_batch_mode_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "LATEX_HIDE_INDICES");
   m_ui->latex_hide_indices_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "LATEX_SOURCE_CODE");
   m_ui->latex_source_code_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "LATEX_BIB_STYLE");
   m_ui->latex_bib_style->setText(tempStr);


   // tab 3 - rtf
   tempStr = convert_Str(data, "RTF_OUTPUT");
   m_ui->rtf_output->setText(tempStr);

   tempBool = convert_Bool(data, "COMPACT_RTF");
   m_ui->rtf_compact_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "RTF_HYPERLINKS");
   m_ui->rtf_hyperlinks_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "RTF_STYLESHEET_FIL");
   m_ui->rtf_stylesheet->setText(tempStr);

   tempStr = convert_Str(data, "RTF_EXTENSIONS_FILE");
   m_ui->rtf_extension->setText(tempStr);

   tempBool = convert_Bool(data, "RTF_SOURCE_CODE");
   m_ui->rtf_source_code_CB->setChecked(tempBool);


   // tab 3 - man
   tempStr = convert_Str(data, "MAN_OUTPUT");
   m_ui->man_output->setText(tempStr);

   tempStr = convert_Str(data, "MAN_EXTENSION");
   m_ui->man_extension->setText(tempStr);

   tempStr = convert_Str(data, "MAN_SUBDIR");
   m_ui->man_subdir->setText(tempStr);

   tempBool = convert_Bool(data, "MAN_LINKS");
   m_ui->man_links_CB->setChecked(tempBool);


   // tab 3 - perl module
   tempBool = convert_Bool(data, "GENERATE_PERLMOD");
   m_ui->gen_perl_CB->setChecked(tempBool);;

   tempBool = convert_Bool(data, "PERLMOD_LATEX");
   m_ui->perl_latex_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "PERLMOD_PRETTY");
   m_ui->perl_pretty_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "PERLMOD_MAKEVAR_PREFIX");
   m_ui->perl_prefix->setText(tempStr);


   // tab 3 - xml
   tempStr = convert_Str(data, "XML_OUTPUT");
   m_ui->xml_output->setText(tempStr);

   tempBool = convert_Bool(data, "XML_PROGRAMLISTING");
   m_ui->xml_program_listing_CB->setChecked(tempBool);


   // tab 3 - docbook
   tempStr = convert_Str(data, "DOCBOOK_OUTPUT");
   m_ui->docbook_output->setText(tempStr);

   tempBool = convert_Bool(data, "DOCBOOK_PROGRAMLISTING");
   m_ui->docbook_program_listing_CB->setChecked(tempBool);

   // final step
   finalLoad();
}

bool MainWindow::convert_Bool(QByteArray data, QByteArray key)
{
   bool retval = false;
   int posBeg  = data.indexOf(key);

   if (posBeg > 0) {
      QString tempStr;

      int posEnd = data.indexOf("\n", posBeg);      
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");      
      tempStr = x.mid(posBeg + 1).trimmed().toUpper();

      if (tempStr == "YES") {
         retval = true;
      }
   }

   return retval;
}

int MainWindow::convert_Int(QByteArray data, QByteArray key)
{
   int retval;
   int posBeg = data.indexOf(key);

   if (posBeg != -1) {
      QString tempStr;

      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();

      retval = tempStr.toInteger<int>();
   }

   return retval;
}

QString MainWindow::convert_Str(QByteArray data, QByteArray key)
{
   QString tempStr;
   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();
   }

   if (tempStr.startsWith("\"") && tempStr.endsWith("\"")) {
      int len = tempStr.length();
      tempStr = tempStr.mid(1, len - 2);
   }

   return tempStr;
}

QString MainWindow::convert_PlainText(QByteArray data, QByteArray key)
{
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg >= 0) {
      int posEnd    = data.indexOf("\n", posBeg);
      int tempStart = posBeg;

      while (posEnd >= 0) {
         QString x = data.mid(tempStart, posEnd - tempStart);

         if (x.contains("\\")) {
            tempStart = posEnd + 1;
            posEnd    = data.indexOf("\n", tempStart);

         } else {
            break;

         }       
      }

      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();

      // break into parts
      QRegularExpression regexp("\\\\[ \\t]*[\\n\\r]+");
      QStringList list = tempStr.split(regexp, QStringParser::SkipEmptyParts);

      // walk each item
      int maxList = list.size();

      for (int k = 0; k < maxList; ++k) {

         QString tempStr = list.at(k);
         tempStr = tempStr.trimmed();

         if (tempStr.startsWith("\"") && tempStr.endsWith("\"")) {
            int len = tempStr.length();
            tempStr = tempStr.mid(1, len - 2);

            list.replace(k, tempStr);

         } else if (tempStr.contains(" ") )   {
            // have multiple entries on one line
            bool isFirst = true;

            QStringList insertList = tempStr.split(" ", QStringParser::SkipEmptyParts);
            int maxInsertList = insertList.size();

            for (int j = 0; j < maxInsertList; ++j) {
               QString newStr = insertList.at(j);
               newStr = newStr.trimmed();

               if (isFirst)  {
                  list.replace(k, newStr);
                  isFirst = false;

               } else {
                  list.insert(k+j, newStr);

                  // watch these two lines of code
                  ++maxList;
               }
            }

            // watch this
            k += maxInsertList - 1;

         } else {
            list.replace(k, tempStr);

         }
      }

      // turn it back into a string
      tempStr = list.join(", ");
   }

   return tempStr;
}

// **

void MainWindow::importDoxy()
{   
   while (true) {

      QString fname = QFileDialog::getOpenFileName(this, tr("Open Doxygen project file"), m_settings.pathPrior);

      if (fname.isEmpty()) {
         csError(tr("File Open"), tr("No file name was provided"));
         break;
      }

      QFile file(fname);
      if (! file.open(QIODevice::ReadOnly)) {
         csError(tr("Error Opening: ") + fname, tr("Unable to open: ") + file.error());
         break;
      }

      QFileInfo fi(file);
      QString importPath = fi.absolutePath() + QDir::separator() + fi.baseName() + ".json";

      // read file
      QByteArray data;

      data = file.readAll();
      file.close();

      // strip comments
      int posBeg;
      int posEnd;

      while (true) {
         posBeg = data.indexOf("#");

         if (posBeg == -1) {
            break;
         }

         posEnd = data.indexOf("\n",posBeg);
         data.remove(posBeg, posEnd-posBeg);
      }

      // verify a few fields to ensure this is an old project file
      if (! data.contains("PROJECT_NAME") || ! data.contains("OUTPUT_DIRECTORY"))  {
         csError(tr("Convert Project File"), tr("Doxygen project file is missing required information, Convert aborted"));
         break;
      }

      // ** save as new project file
      fname = QFileDialog::getSaveFileName(this, tr("Save as DoxyPress project file"), importPath,
                                           tr("Json Files (*.json)"));

      if (fname.isEmpty()) {
         csError(tr("Convert Project File"), tr("No DoxyPress file name was provided, Convert aborted"));
         break;

      } else {
         QMessageBox quest;
         quest.setWindowTitle(tr("Convert Doxygen Project"));
         quest.setWindowIcon(QIcon("://resources/doxypress.png"));

         quest.setText( tr("If a layout or css file was specified in your project file, please refer to the DoxyPress documentation "
                           "regarding 'Converting to DoxyPress' for additional information. Continue?"));

         quest.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
         quest.setDefaultButton(QMessageBox::No);

         int retval = quest.exec();

         if (retval == QMessageBox::Yes) {            
            m_curFile = fname;
            m_settings.pathPrior = this->pathName(fname);

            clearAllFields();
            convertDoxy(data);

            saveDoxy_Internal();

            json_Write(PATH_PRIOR);

            if (! m_rf_List.contains(m_curFile)) {
               rf_Update();
            }
         }

      }

      // all done, everything is fine
      break;
   }
}

void MainWindow::autoConvert(QString fromFile, QString toFile)
{
   QFile file(fromFile);
   if (! file.open(QIODevice::ReadOnly)) {
      printf("Error: Unable to open %s, error %s", csPrintable(fromFile), csPrintable(file.error()) );
      return;
   }

   // read file
   QByteArray data;

   data = file.readAll();
   file.close();

   // strip comments
   int posBeg;
   int posEnd;

   while (true) {
      posBeg = data.indexOf("#");

      if (posBeg == -1) {
         break;
      }

      posEnd = data.indexOf("\n",posBeg);
      data.remove(posBeg, posEnd-posBeg);
   }

   // verify a few fields to ensure this is an old project file
   if (! data.contains("PROJECT_NAME") || ! data.contains("OUTPUT_DIRECTORY"))  {
      printf("Doxygen project file is missing required information, Convert aborted");
      return;
   }

   m_curFile = toFile;

   convertDoxy(data);
   saveDoxy_Internal();
}

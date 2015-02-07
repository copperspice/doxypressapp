/*************************************************************************
 *
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#include "mainwindow.h"

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
   m_ui->project_number->setText(tempStr);

   tempStr = convert_Str(data, "PROJECT_LOGO");
   m_project_iconFN = setText(tempStr);

   tempStr = convert_Str(data,"OUTPUT_DIRECTORY");
   m_ui->output_dir->setText(tempStr);


/*
   tempBool = convert_Bool(data,"optimize-cplus");
   m_ui->optimize_cplus_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"optimize-cli");
   m_ui->optimize_cli_CB->setChecked(tempBool);
*/


   tempBool = convert_Bool(data,"OPTIMIZE_OUTPUT_JAVA");
   m_ui->optimize_java_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"OPTIMIZE_OUTPUT_FOR_C");
   m_ui->optimize_c_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"OPTIMIZE_FOR_FORTRAN");
   m_ui->optimize_fortran_CB->setChecked(tempBool);

   //
   tempBool = convert_Bool(data,"GENERATE_HTML");
   m_ui->gen_html_CB1->setChecked(tempBool);


/*
   tempBool = convert_Bool(data,"html-plain");
   m_ui->html_plain_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"html-nav");
   m_ui->html_nav_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"html-chm");
   m_ui->html_chm_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"search-enabled");
   m_ui->searchEnabled_CB->setChecked(tempBool);
*/


   //
   tempBool = convert_Bool(data,"GENERATE_LATEX");
   m_ui->gen_latex_CB1->setChecked(tempBool);


/*
   tempBool = convert_Bool(data,"latex-hyper-pdf");
   m_ui->latex_hyper_pdf_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"latex-pdf");
   m_ui->latex_pdf_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"latex-ps");
   m_ui->latex_ps_RB->setChecked(tempBool);
*/

   //
   tempBool = convert_Bool(data,"GENERATE_RTF");;
   m_ui->gen_rtf_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_MAN");
   m_ui->gen_man_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_XML");
   m_ui->gen_xml_CB1->setChecked(tempBool);

   tempBool = convert_Bool(data,"GENERATE_DOCBOOK");
   m_ui->gen_docbook_CB1->setChecked(tempBool);



//??

   tempBool = convert_Bool(data,"DIAGRAM-NONE");
   m_ui->diagram_none_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"DIAGRAM_BUILT_IN");
   m_ui->diagram_built_in_RB->setChecked(tempBool);

   tempBool = convert_Bool(data,"DIAGRAM_DOT");
   m_ui->diagram_dot_RB->setChecked(tempBool);


//??

   tempBool = convert_Bool(data,"CLASS_DIAGRAMS");
   m_ui->dot_class_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"COLLABORATION_GRAPH");
   m_ui->dot_collaboration_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"DOT_OVERALL");
   m_ui->dot_overall_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"INCLUDE_GRAPH");
   m_ui->dot_include_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"INCLUDED_BY_GRAPH");
   m_ui->dot_included_by_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"CALL_GRAPH");
   m_ui->dot_call_CB->setChecked(tempBool);

   tempBool = convert_Bool(data,"CALLER_GRAPH");
   m_ui->dot_called_by_CB->setChecked(tempBool);


   //  ***
   // tab 2 - project
   tempStr = convert_Str(data,"PROJECT_ENCODING");
   m_ui->project_encoding->setText(tempStr);


   m_ui->create_subdirs_CB->setChecked(            object.value("create-subdirs").toBool());
   m_ui->allow_unicode_names_CB->setChecked(       object.value("allow-unicode-names").toBool());

   index = m_ui->output_langugae_CM->findText(     object.value("output-langugae").toString());
   m_ui->output_langugae_CM->setCurrentIndex(index);

   m_ui->brief_member_desc_CB->setChecked(         object.value("brief-member-desc").toBool());
   m_ui->repeat_brief_CB->setChecked(              object.value("repeat-brief").toBool());

   tempText = convert_PlainText(data,"");
   m_ui->abbreviate_brief->setPlainText(           getDataList(object, "abbreviate-brief"));

   m_ui->always_detailed_sec_CB->setChecked(       object.value("always-detailed-sec").toBool());
   m_ui->inline_inherited_member_CB->setChecked(   object.value("inline-inherited-member").toBool());
   m_ui->full_path_names_CB->setChecked(           object.value("full-path-names").toBool());

   tempText = convert_PlainText(data,"");
   m_ui->strip_from_path->setPlainText(            getDataList(object, "strip-from-path"));

   tempText = convert_PlainText(data,"");
   m_ui->strip_from_inc_path->setPlainText(        getDataList(object, "strip-from-inc-path"));

   m_ui->short_names_CB->setChecked(               object.value("short-names").toBool());
   m_ui->javadoc_auto_brief_CB->setChecked(        object.value("javadoc-auto-brief").toBool());
   m_ui->qt_auto_brief_CB->setChecked(             object.value("qt-auto-brief").toBool());
   m_ui->multiline_cpp_brief_CB->setChecked(       object.value("multiline-cpp-brief").toBool());
   m_ui->inherit_docs_CB->setChecked(              object.value("inherit-docs").toBool());
   m_ui->separate_member_pages_CB->setChecked(     object.value("separate-member-pages").toBool());

   tempInt = convert_Int(data, "");
   m_ui->tab_size_SB->setValue(                    object.value("tab-size").toInt());

   tempText = convert_PlainText(data,"");
   m_ui->aliases->setPlainText(                    getDataList(object, "aliases"));

   tempText = convert_PlainText(data,"");
   m_ui->tcl_subst->setPlainText(                  getDataList(object, "tcl-subst"));

   tempText = convert_PlainText(data,"");
   m_ui->extension_mapping->setPlainText(          getDataList(object, "extension-mapping"));

   m_ui->markdown_CB->setChecked(                  object.value("markdown").toBool());
   m_ui->auto_link_CB->setChecked(                 object.value("auto-link").toBool());
   m_ui->built_in_stl_support_CB->setChecked(      object.value("built-in-stl-support").toBool());
   m_ui->cpp_cli_support_CB->setChecked(           object.value("cpp-cli-support").toBool());
   m_ui->sip_support_CB->setChecked(               object.value("sip-support").toBool());
   m_ui->idl_support_CB->setChecked(               object.value("idl-support").toBool());
   m_ui->dist_group_doc_CB->setChecked(            object.value("dist-group-doc").toBool());
   m_ui->allow_sub_grouping_CB->setChecked(        object.value("allow-sub-grouping").toBool());
   m_ui->inline_grouped_classes_CB->setChecked(    object.value("inline-grouped-classes").toBool());
   m_ui->inline_simple_struct_CB->setChecked(      object.value("inline-simple-struct").toBool());
   m_ui->typedef_hids_struct_CB->setChecked(       object.value("typedef-hids-struct").toBool());

   tempInt = convert_Int(data, "");
   m_ui->lookup_cache_size_SB->setValue(           object.value("lookup-cache-size").toInt());


   // tab 2 - build
   m_ui->extract_all_CB->setChecked(               object.value("extract-all").toBool());
   m_ui->extract_private_CB->setChecked(           object.value("extract-private").toBool());
   m_ui->extract_package_CB->setChecked(           object.value("extract-package").toBool());
   m_ui->extract_static_CB->setChecked(            object.value("extract-static").toBool());
   m_ui->extract_local_classes_CB->setChecked(     object.value("extract-local-classes").toBool());
   m_ui->extract_local_methods_CB->setChecked(     object.value("extract-local-methods").toBool());
   m_ui->extract_anon_namespaces_CB->setChecked(   object.value("extract-anon-namespaces").toBool());
   m_ui->hide_undoc_members_CB->setChecked(        object.value("hide-undoc-members").toBool());
   m_ui->hide_undoc_classes_CB->setChecked(        object.value("hide-undoc-classes").toBool());
   m_ui->hide_friend_compounds_CB->setChecked(     object.value("hide-friend-compounds").toBool());
   m_ui->hide_in_body_docs_CB->setChecked(         object.value("hide-in-body-docs").toBool());
   m_ui->hide_scope_names_CB->setChecked(          object.value("hide-scope-names").toBool());

   m_ui->internal_docs_CB->setChecked(             object.value("internal-docs").toBool());
   m_ui->case_sense_names_CB->setChecked(          object.value("case-sense-names").toBool());
   m_ui->show_include_files_CB->setChecked(        object.value("show-include-files").toBool());
   m_ui->show_grouped_members_incl_CB->setChecked( object.value("show-grouped-members-incl").toBool());
   m_ui->force_local_includes_CB->setChecked(      object.value("force-local-includes").toBool());
   m_ui->inline_info_CB->setChecked(               object.value("inline-info").toBool());

   m_ui->sort_member_docs_CB->setChecked(          object.value("sort-member-docs").toBool());
   m_ui->sort_brief_docs_CB->setChecked(           object.value("sort-brief-docs").toBool());
   m_ui->sort_members_ctors_first_CB->setChecked(  object.value("sort-members-ctors-first").toBool());
   m_ui->sort_group_names_CB->setChecked(          object.value("sort-group-names").toBool());
   m_ui->sort_by_scope_name_CB->setChecked(        object.value("sort-by-scope-name").toBool());

   m_ui->strict_proto_matching_CB->setChecked(     object.value("strict-proto-matching").toBool());
   m_ui->gen_todo_list_CB->setChecked(             object.value("gen-todo-list").toBool());
   m_ui->gen_test_list_CB->setChecked(             object.value("gen-test-list").toBool());
   m_ui->gen_bug_list_CB->setChecked(              object.value("gen-bug-list").toBool());
   m_ui->gen_deprecatcated_list_CB->setChecked(    object.value("gen-deprecatcated-list").toBool());

   tempText = convert_PlainText(data,"");
   m_ui->enabled_sections->setPlainText(           getDataList(object, "enabled-sections"));

   tempInt = convert_Int(data, "");
   m_ui->max_init_lines_SB->setValue(              object.value("max-init-lines").toInt());

   m_ui->show_used_files_CB->setChecked(           object.value("show-used-files").toBool());
   m_ui->show_files_CB->setChecked(                object.value("show-files").toBool());
   m_ui->show_namespaces_CB->setChecked(           object.value("show-namespaces").toBool());

   tempStr = convert_Str(data, "");
   m_ui->file_version_filter->setText(             object.value("file-version-filter").toString());

   tempStr = convert_Str(data, "");
   m_ui->layout_file->setText(                     object.value("layout-file").toString());

   tempText = convert_PlainText(data,"");
   m_ui->cite_bib_files->setPlainText(             getDataList(object, "cite-bib-files"));


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
   m_ui->warn_undoc_parm_CB->setChecked(tempBool);

   tempStr = convert_Str(data,"WARN_FORMAT");
   m_ui->warn_forrmat->setText(tempStr);

   tempStr = convert_Str(data,"WARN_LOGFILE");
   m_ui->warn_logfile->setText(tempStr);


   // tab 2 -input
   tempStr = convert_Str(data,"INPUT");
   m_ui->input_source->setPlainText(tempStr);

   tempText = convert_Str(data,"INPUT_ENCODING");
   m_ui->input_encoding->setText(tempText);

   tempText = convert_PlainText(data,"FILE_PATTERNS");
   m_ui->file_patterns->setPlainText(tempStr);

   tempBool = convert_Bool(data,"RECURSIVE");
   m_ui->source_recursive_CB->setChecked(tempBool);


   tempText = convert_PlainText(data,"");
   m_ui->exclude_files->setPlainText(              getDataList(object, "exclude-files"));
   m_ui->exclude_symlinks_CB->setChecked(          object.value("exclude-symlinks").toBool());

   tempText = convert_PlainText(data,"");
   m_ui->exclude_patterns->setPlainText(           getDataList(object, "exclude-patterns"));

   tempText = convert_PlainText(data,"");
   m_ui->exclude_symbols->setPlainText(            getDataList(object, "exclude-symbols"));

   tempText = convert_PlainText(data,"");
   m_ui->example_source->setPlainText(             getDataList(object, "example-source"));

   tempText = convert_PlainText(data,"");
   m_ui->example_patterns->setPlainText(           getDataList(object, "example-pattens"));
   m_ui->example_recursive_CB->setChecked(         object.value("example-recursive").toBool());


   tempText = convert_PlainText(data,"");
   m_ui->image_path->setPlainText(                 getDataList(object, "image-path"));

   tempStr = convert_Str(data, "");
   m_ui->input_filter->setText(                    object.value("input-filter").toString());

   tempText = convert_PlainText(data,"");
   m_ui->filter_patterns->setPlainText(            getDataList(object, "filter-patterns"));
   m_ui->filter_source_files_CB->setChecked(       object.value("filter-source-files").toBool());

   tempText = convert_PlainText(data,"");
   m_ui->filter_source_patterns->setPlainText(     getDataList(object, "filter-source-patterns"));

   tempStr = convert_Str(data, "");
   m_ui->mdfile_mainpage->setText(                 object.value("mdfile-mainpage").toString());


   // tab 2 -browser
   m_ui->source_browser_CB->setChecked(            object.value("source-browser").toBool());
   m_ui->inline_sources_CB->setChecked(            object.value("inline-sources").toBool());
   m_ui->strip_code_comments_CB->setChecked(       object.value("strip-code-comments").toBool());
   m_ui->ref_by_relation_CB->setChecked(           object.value("ref-by-relation").toBool());
   m_ui->ref_relation_CB->setChecked(              object.value("ref-relation").toBool());
   m_ui->ref_link_source_CB->setChecked(           object.value("ref-link-source").toBool());
   m_ui->source_tooltips_CB->setChecked(           object.value("source-tooltips").toBool());
   m_ui->use_htags_CB->setChecked(                 object.value("use-htags").toBool());
   m_ui->verbatim_headers_CB->setChecked(          object.value("verbatim-headers").toBool());
   m_ui->clang_parsing_CB->setChecked(             object.value("clang-parsing").toBool());

   tempText = convert_PlainText(data,"");
   m_ui->clang_options->setPlainText(              getDataList(object, "clang-options"));


   // tab 2 -index
   tempBool = convert_Bool(data, "ALPHABETICAL_INDEX");
   m_ui->alpha_index_CB->setChecked(tempBool);

   tempInt = convert_Int(data, "COLS_IN_ALPHA_INDEX");
   m_ui->cols_in_index_SB->setValue(tempInt);

   tempText = convert_PlainText(data,"IGNORE_PREFIX");
   m_ui->ignore_prefix->setPlainText(tempText);

   // tab 2 - autogen
   tempBool = convert_Bool(data, "GENERATE_AUTOGEN_DEF");
   m_ui->gen_autogen_def_CB->setChecked(tempBool);

   // tab 2 - perlmod
   tempBool = convert_Bool(data, "GENERATE_PERLMOD");
   m_ui->gen_perl_CB->setChecked(tempBool);;

   tempBool = convert_Bool(data, "PERLMOD_LATEX");
   m_ui->perl_latex_CB->setChecked(tempBool);

   tempBool = convert_Bool(data, "PERLMOD_PRETTY");
   m_ui->perl_pretty_CB->setChecked(tempBool);

   tempStr = convert_Str(data, "PERLMOD_MAKEVAR_PREFIX");
   m_ui->perlmod_prefix->setText(tempStr);

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
   m_ui->include_file_patterns->setPlainText(tempText);

   tempText = convert_PlainText(data,"PREDEFINED");
   m_ui->predefined_macros->setPlainText(tempText);

   tempText = convert_PlainText(data,"EXPAND_AS_DEFINED");
   m_ui->expand_as_defined->setPlainText(tempText);

   tempBool = convert_Bool(data, "SKIP_FUNCTION_MACROS");
   m_ui->skip_function_macros_CB->setChecked(tempBool);


   // tab 2 - external
   tempText = convert_PlainText(data,"TAGFILES");
   m_ui->tag_files->setPlainText(tempText);

   tempText = convert_PlainText(data,"TAGFILES");
   m_ui->generate_tagfile->setText(                object.value("generate-tagfile").toString());

   tempBool = convert_Bool(data, "");
   m_ui->all_externals_CB->setChecked(             object.value("all-externals").toBool());

   tempBool = convert_Bool(data, "");
   m_ui->external_groups_CB->setChecked(           object.value("external-groups").toBool());

   tempBool = convert_Bool(data, "");
   m_ui->external_pages_CB->setChecked(            object.value("external-pages").toBool());

   tempStr = convert_Str(data, "PERL_PATH");
   m_ui->perl_path->setText(tempStr);


   // tab 2 - dot

   m_ui->class_diagrams_CB->setChecked(            object.value("class-diagrams").toBool());

   tempStr = convert_Str(data, "");
   m_ui->mscgen_path->setText(                     object.value("mscgen-path").toString());

   tempStr = convert_Str(data, "");
   m_ui->dia_path->setText(                        object.value("dia-path").toString());

   m_ui->hide_undoc_relations_CB->setChecked(      object.value("hide-undoc-relations").toBool());
   m_ui->have_dot_CB->setChecked(                  object.value("have-dot").toBool());

   tempInt = convert_Int(data, "");
   m_ui->dot_num_threads_SB->setValue(             object.value("dot-num-threads").toInt());

   tempStr = convert_Str(data, "");
   m_ui->dot_font_name->setText(                   object.value("dot-font-name").toString());

   tempInt = convert_Int(data, "");
   m_ui->dot_font_size_SB->setValue(               object.value("dot-font-size").toInt());

   tempStr = convert_Str(data, "");
   m_ui->dot_font_path->setText(                   object.value("dot-font-path").toString());

   m_ui->class_graph_CB->setChecked(               object.value("class-graph").toBool());
   m_ui->collaboration_graph_CB->setChecked(       object.value("collaboration-graph").toBool());
   m_ui->group_graphs_CB->setChecked(              object.value("group-graphs").toBool());
   m_ui->uml_look_CB->setChecked(                  object.value("uml-look").toBool());

   tempInt = convert_Int(data, "");
   m_ui->uml_limit_num_fields_SB->setValue(        object.value("uml-limit-num-fields").toInt());

   m_ui->template_relations_CB->setChecked(        object.value("template-relations").toBool());
   m_ui->include_graph_CB->setChecked(             object.value("include-graph").toBool());
   m_ui->included_by_graph_CB->setChecked(         object.value("included-by-graph").toBool());
   m_ui->call_graph_CB->setChecked(                object.value("call-graph").toBool());
   m_ui->caller_graph_CB->setChecked(              object.value("caller-graph").toBool());
   m_ui->graphical_hierarchy_CB->setChecked(       object.value("graphical-hierarchy").toBool());
   m_ui->directory_graph_CB->setChecked(           object.value("directory-graph").toBool());

   index = m_ui->dot_image_format_CM->findText(    object.value("dot-image-format").toString());
   m_ui->dot_image_format_CM->setCurrentIndex(index);

   m_ui->interactive_svg_CB->setChecked(           object.value("interactive_svg").toBool());

   tempStr = convert_Str(data, "");
   m_ui->dot_path->setText(                        object.value("dot-path").toString());

   tempText = convert_PlainText(data,"");
   m_ui->dot_file_dirs->setPlainText(              getDataList(object, "dot-file_dirs"));

   tempText = convert_PlainText(data,"");
   m_ui->msc_file_dirs->setPlainText(              getDataList(object, "msc-file_dirs"));

   tempText = convert_PlainText(data,"");
   m_ui->dia_file_dirs->setPlainText(              getDataList(object, "dia-file_dirs"));

   tempStr = convert_Str(data, "");
   m_ui->platuml_jar_path->setText(                object.value("platuml-jar-path").toString());

   tempInt = convert_Int(data, "");
   m_ui->dot_graph_max_nodes_SB->setValue(         object.value("dot-graph_max-nodes").toInt());

   tempInt = convert_Int(data, "");
   m_ui->dot_graph_max_depth_SB->setValue(         object.value("dot-graph_max-depth").toInt());

   m_ui->dot_transparent_CB->setChecked(           object.value("dot-transparent").toBool());
   m_ui->dot_multple_targets_CB->setChecked(       object.value("dot-multple-targets").toBool());
   m_ui->gen_legend_CB->setChecked(                object.value("gen-legend").toBool());
   m_ui->dot_cleanup_CB->setChecked(               object.value("dot-cleanup").toBool());


   //  ***
   // tab 3 - html

   tempStr = convert_Str(data, "");
   m_ui->html_ouput->setText(                      object.value("html-ouput").toString());

   tempStr = convert_Str(data, "");
   m_ui->html_file_extension->setText(             object.value("html-file-extension").toString());

   tempStr = convert_Str(data, "");
   m_ui->html_header->setText(                     object.value("html-header").toString());

   tempStr = convert_Str(data, "");
   m_ui->html_footer->setText(                     object.value("html-footer").toString());

   tempStr = convert_Str(data, "");
   m_ui->html_stylesheet->setText(                 object.value("html-stylesheet").toString());

   tempText = convert_PlainText(data,"");
   m_ui->html_extra_stylesheet->setPlainText(      getDataList(object, "html-extra-stylesheet"));

   tempText = convert_PlainText(data,"");
   m_ui->html_extra_file->setPlainText(            getDataList(object, "html-extra-file"));

   tempInt = convert_Int(data, "");
   m_ui->html_colorstyle_hue->setValue(            object.value("html-colorstyle-hue").toInt());

   tempInt = convert_Int(data, "");
   m_ui->html_colorstyle_sat->setValue(            object.value("html-colorstyle-sat").toInt());

   tempInt = convert_Int(data, "");
   m_ui->html_colorstyle_gamma->setValue(          object.value("html-colorstyle-gamma").toInt());

   m_ui->html_timestamp_CB->setChecked(            object.value("html-timestamp").toBool());
   m_ui->html_synamic_sections_CB->setChecked(     object.value("htl-synamic-sections").toBool());

   tempInt = convert_Int(data, "");
   m_ui->html_index_num_entries_SB->setValue(      object.value("html-index-num-entries").toInt());

   tempBool = convert_Bool(data,"GENERATE_DOCSET");
   m_ui->gen_docbook_CB1->setChecked(tempBool);


// start here <--------------
tempStr = convert_Str(data, "");


   m_ui->docset_feedname->setText(                 object.value("docset-feedname").toString());
   m_ui->docset_bundle_id->setText(                object.value("docset-bundle-id").toString());
   m_ui->docset_publisher_id->setText(             object.value("docset-publisher-id").toString());
   m_ui->docset_publisher_name->setText(           object.value("docset-publisher-name").toString());
   m_ui->gen_html_help_CB->setChecked(             object.value("gen-html-help").toBool());
   m_ui->chm_file->setText(                        object.value("chm-file").toString());

   m_ui->hhc_location->setText(                    object.value("hhc-location").toString());
   m_ui->gen_chi_CB->setChecked(                   object.value("gen-chi").toBool());
   m_ui->chm_index_encoding->setText(              object.value("chm-index-encoding").toString());
   m_ui->binary_toc_CB->setChecked(                object.value("binary-toc").toBool());
   m_ui->toc_expanded_CB->setChecked(              object.value("toc-expanded").toBool());
   m_ui->gen_qhp_CB->setChecked(                   object.value("gen-qhp").toBool());

   m_ui->qch_file->setText(                        object.value("qch-file").toString());
   m_ui->qhp_namespace->setText(                   object.value("qhp-namespace").toString());
   m_ui->qhp_virtual_folder->setText(              object.value("qhp-virtual-folder").toString());
   m_ui->qhp_cust_filter_name->setText(            object.value("qhp-cust-filter-name").toString());
   m_ui->qhp_cust_filter_attrib->setText(          object.value("qhp-cust-filter-attrib").toString());
   m_ui->qhp_section_filter_name->setText(         object.value("qhp-section-filter-name").toString());
   m_ui->qhg_location->setText(                    object.value("qhg-location").toString());

   m_ui->gen_eclipse_help_CB->setChecked(          object.value("gen-eclipse-help").toBool());
   m_ui->eclipse_doc_id->setText(                  object.value("eclipse-doc-id").toString());
   m_ui->disable_index_CB->setChecked(             object.value("disable-index").toBool());
   m_ui->gen_treeview_CB->setChecked(              object.value("gen-treeview").toBool());

   tempInt = convert_Int(data, "");
   m_ui->enum_values_per_line_SB->setValue(        object.value("enum-values-per-line").toInt());

   tempInt = convert_Int(data, "");
   m_ui->treeview_width_SB->setValue(              object.value("treeview-width").toInt());


   m_ui->external_links_in_window_CB->setChecked(  object.value("external-links-in-window").toBool());

   tempInt = convert_Int(data, "");
   m_ui->formula_fontsize_SB->setValue(            object.value("formula-fontsize").toInt());
   m_ui->formula_transparent_CB->setChecked(       object.value("formula-transparent").toBool());

   m_ui->use_mathjax_CB->setChecked(               object.value("use_mathjax").toBool());

   index = m_ui->mathjax_format_CM->findText(      object.value("mathjax-format").toString());
   m_ui->mathjax_format_CM->setCurrentIndex(index);

   m_ui->mathjax_repath->setText(                  object.value("mathjax-repath").toString());

   tempText = convert_PlainText(data,"");
   m_ui->mathjax_extensions->setPlainText(         getDataList(object, "mathjax-extensions"));

   m_ui->mathjax_codefile->setText(                object.value("mathjax-codefile").toString());

   m_ui->search_engine_CB->setChecked(             object.value("search-engine").toBool());
   m_ui->server_based_search_CB->setChecked(       object.value("server-based-search").toBool());
   m_ui->external_search_CB->setChecked(           object.value("external-search").toBool());
   m_ui->search_engine_url->setText(               object.value("search-engine-url").toString());
   m_ui->search_data_file->setText(                object.value("search-data-file").toString());
   m_ui->external_search_id->setText(              object.value("external-search-id").toString());

   tempText = convert_PlainText(data,"");
   m_ui->extra_search_mappings->setPlainText(      getDataList(object, "extra-search-mappings"));


   // tab 3 - latex

   m_ui->latex_output->setText(                    object.value("latex-output").toString());
   m_ui->latex_cmd_name->setText(                  object.value("latex-cmd-name").toString());
   m_ui->make_index_cmd_name->setText(             object.value("make-index-cmd-name").toString());
   m_ui->compact_latex_CB->setChecked(             object.value("compact-latex").toBool());

   index = m_ui->paper_type_CM->findText(          object.value("paper-type").toString());
   m_ui->paper_type_CM->setCurrentIndex(index);


   tempText = convert_PlainText(data,"");
   m_ui->latex_extra_packages->setPlainText(       getDataList(object, "latex-extra-packages"));
   m_ui->latex_header->setText(                    object.value("latex-header").toString());
   m_ui->latex_footer->setText(                    object.value("latex-footer").toString());

   tempText = convert_PlainText(data,"");
   m_ui->latex_extra_files->setPlainText(          getDataList(object, "latex-extra-files"));

   m_ui->pdf_hyperlinks_CB->setChecked(            object.value("pdf-hyperlinks").toBool());
   m_ui->use_pdf_latex_CB->setChecked(             object.value("use-pdf-latex").toBool());
   m_ui->laxtex_batch_mode_CB->setChecked(         object.value("laxtex-batch-mode").toBool());
   m_ui->laxtex_hide_indices_CB->setChecked(       object.value("laxtex-hide-indices").toBool());
   m_ui->laxtex_source_code_CB->setChecked(        object.value("laxtex-source-code").toBool());
   m_ui->laxtex_bib_style->setText(                object.value("laxtex-bib-style").toString());


   // tab 3 - rtf
   tempStr = convert_Str(data, "");
   m_ui->rtf_output->setText(                      object.value("rtf-output").toString());

   tempBool = convert_Bool(data, "");
   m_ui->rtf_compact_CB->setChecked(               object.value("rtf-compact").toBool());

   tempBool = convert_Bool(data, "");
   m_ui->rtf_hyperlinks_CB->setChecked(            object.value("rtf-hyperlinks").toBool());

   tempStr = convert_Str(data, "");
   m_ui->rtf_stylesheet->setText(                  object.value("rtf-stylesheet").toString());

   tempStr = convert_Str(data, "");
   m_ui->rtf_extension->setText(                   object.value("rtf-extension").toString());


   // tab 3 - man
   tempStr = convert_Str(data, "");
   m_ui->man_output->setText(                      object.value("man-output").toString());

   tempStr = convert_Str(data, "");
   m_ui->man_extension->setText(                   object.value("man-extension").toString());

   tempStr = convert_Str(data, "");
   m_ui->man_subdir->setText(                      object.value("man-subdir").toString());

   tempBool = convert_Bool(data, "");
   m_ui->man_links_CB->setChecked(                 object.value("man-links").toBool());


   // tab 3 - xml
   tempStr = convert_Str(data, "");
   m_ui->xml_output->setText(                      object.value("xml-output").toString());

   tempBool = convert_Bool(data, "");
   m_ui->xml_program_listing_CB->setChecked(       object.value("xml-program-listing").toBool());


   // tab 3 - docbook
   tempStr = convert_Str(data, "");
   m_ui->docbook_output->setText(                  object.value("docbook-output").toString());

   tempBool = convert_Bool(data, "");
   m_ui->docbook_program_listing_CB->setChecked(   object.value("docbook-program-listing").toBool());

   /*
      tempBool = convert_Bool(data, "");
      tempBool);

      tempStr = convert_Str(data, "");
      tempStr);
   */
}


/*

CREATE_SUBDIRS         = NO
ALLOW_UNICODE_NAMES    = NO
OUTPUT_LANGUAGE        = English
BRIEF_MEMBER_DESC      = YES
REPEAT_BRIEF           = YES
ABBREVIATE_BRIEF       = "The $name class" \
ALWAYS_DETAILED_SEC    = NO
INLINE_INHERITED_MEMB  = NO
FULL_PATH_NAMES        = YES
STRIP_FROM_PATH        = 
STRIP_FROM_INC_PATH    = 
SHORT_NAMES            = NO
JAVADOC_AUTOBRIEF      = NO
QT_AUTOBRIEF           = NO
MULTILINE_CPP_IS_BRIEF = NO
INHERIT_DOCS           = YESEeeee
SEPARATE_MEMBER_PAGES  = NO
TAB_SIZE               = 4
ALIASES                = 
TCL_SUBST              = 
EXTENSION_MAPPING      = 
MARKDOWN_SUPPORT       = YES
AUTOLINK_SUPPORT       = YES
BUILTIN_STL_SUPPORT    = NO
CPP_CLI_SUPPORT        = NO
SIP_SUPPORT            = NO
IDL_PROPERTY_SUPPORT   = YES
DISTRIBUTE_GROUP_DOC   = NO
SUBGROUPING            = YES
INLINE_GROUPED_CLASSES = NO
INLINE_SIMPLE_STRUCTS  = NO
TYPEDEF_HIDES_STRUCT   = NO
LOOKUP_CACHE_SIZE      = 0
EXTRACT_ALL            = NO
EXTRACT_PRIVATE        = NO
EXTRACT_PACKAGE        = NO
EXTRACT_STATIC         = NO
EXTRACT_LOCAL_CLASSES  = YES
EXTRACT_LOCAL_METHODS  = NO
EXTRACT_ANON_NSPACES   = NO
HIDE_UNDOC_MEMBERS     = NO
HIDE_UNDOC_CLASSES     = NO
HIDE_FRIEND_COMPOUNDS  = NO
HIDE_IN_BODY_DOCS      = NO
INTERNAL_DOCS          = NO
CASE_SENSE_NAMES       = NO
HIDE_SCOPE_NAMES       = NO
SHOW_INCLUDE_FILES     = YES
SHOW_GROUPED_MEMB_INC  = NO
FORCE_LOCAL_INCLUDES   = NO
INLINE_INFO            = YES
SORT_MEMBER_DOCS       = YES
SORT_BRIEF_DOCS        = NO
SORT_MEMBERS_CTORS_1ST = NO
SORT_GROUP_NAMES       = NO
SORT_BY_SCOPE_NAME     = NO
STRICT_PROTO_MATCHING  = NO
GENERATE_TODOLIST      = YES
GENERATE_TESTLIST      = YES
GENERATE_BUGLIST       = YES
GENERATE_DEPRECATEDLIST= YES
ENABLED_SECTIONS       = 
MAX_INITIALIZER_LINES  = 30
SHOW_USED_FILES        = YES
SHOW_FILES             = YES
SHOW_NAMESPACES        = YES
FILE_VERSION_FILTER    = 
LAYOUT_FILE            = 
CITE_BIB_FILES         = 

EXCLUDE                = 
EXCLUDE_SYMLINKS       = NO
EXCLUDE_PATTERNS       = 
EXCLUDE_SYMBOLS        = 
EXAMPLE_PATH           = 
EXAMPLE_PATTERNS       = *
EXAMPLE_RECURSIVE      = NO
IMAGE_PATH             = 
INPUT_FILTER           = 
FILTER_PATTERNS        = 
FILTER_SOURCE_FILES    = NO
FILTER_SOURCE_PATTERNS = 
USE_MDFILE_AS_MAINPAGE = 
SOURCE_BROWSER         = NO
INLINE_SOURCES         = NO
STRIP_CODE_COMMENTS    = YES
REFERENCED_BY_RELATION = NO
REFERENCES_RELATION    = NO
REFERENCES_LINK_SOURCE = YES
SOURCE_TOOLTIPS        = YES
USE_HTAGS              = NO
VERBATIM_HEADERS       = YES
CLANG_ASSISTED_PARSING = NO
CLANG_OPTIONS          =

HTML_OUTPUT            = html
HTML_FILE_EXTENSION    = .html
HTML_HEADER            = 
HTML_FOOTER            = 
HTML_STYLESHEET        = 
HTML_EXTRA_STYLESHEET  = 
HTML_EXTRA_FILES       = 
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100
HTML_COLORSTYLE_GAMMA  = 80
HTML_TIMESTAMP         = YES
HTML_DYNAMIC_SECTIONS  = NO
HTML_INDEX_NUM_ENTRIES = 100
GENERATE_DOCSET        = NO
DOCSET_FEEDNAME        = "Doxygen generated docs"
DOCSET_BUNDLE_ID       = org.doxygen.Project
DOCSET_PUBLISHER_ID    = org.doxygen.Publisher
DOCSET_PUBLISHER_NAME  = Publisher
GENERATE_HTMLHELP      = NO
CHM_FILE               = 
HHC_LOCATION           = 
GENERATE_CHI           = NO
CHM_INDEX_ENCODING     = 
BINARY_TOC             = NO
TOC_EXPAND             = NO
GENERATE_QHP           = NO
QCH_FILE               = 
QHP_NAMESPACE          = org.doxygen.Project
QHP_VIRTUAL_FOLDER     = doc
QHP_CUST_FILTER_NAME   = 
QHP_CUST_FILTER_ATTRS  = 
QHP_SECT_FILTER_ATTRS  = 
QHG_LOCATION           = 
GENERATE_ECLIPSEHELP   = NO
ECLIPSE_DOC_ID         = org.doxygen.Project
DISABLE_INDEX          = NO
GENERATE_TREEVIEW      = NO
ENUM_VALUES_PER_LINE   = 4
TREEVIEW_WIDTH         = 250
EXT_LINKS_IN_WINDOW    = NO
FORMULA_FONTSIZE       = 10
FORMULA_TRANSPARENT    = YES
USE_MATHJAX            = NO
MATHJAX_FORMAT         = HTML-CSS
MATHJAX_RELPATH        = http://cdn.mathjax.org/mathjax/latest
MATHJAX_EXTENSIONS     = 
MATHJAX_CODEFILE       = 
SEARCHENGINE           = YES
SERVER_BASED_SEARCH    = NO
EXTERNAL_SEARCH        = NO
SEARCHENGINE_URL       = 
SEARCHDATA_FILE        = searchdata.xml
EXTERNAL_SEARCH_ID     = 
EXTRA_SEARCH_MAPPINGS  = 
LATEX_OUTPUT           = latex
LATEX_CMD_NAME         = latex
MAKEINDEX_CMD_NAME     = makeindex
COMPACT_LATEX          = NO
PAPER_TYPE             = a4
EXTRA_PACKAGES         = 
LATEX_HEADER           = 
LATEX_FOOTER           = 
LATEX_EXTRA_FILES      = 
PDF_HYPERLINKS         = YES
USE_PDFLATEX           = YES
LATEX_BATCHMODE        = NO
LATEX_HIDE_INDICES     = NO
LATEX_SOURCE_CODE      = NO
LATEX_BIB_STYLE        = plain   
RTF_OUTPUT             = rtf
COMPACT_RTF            = NO

RTF_HYPERLINKS         = NO
RTF_STYLESHEET_FILE    = 
RTF_EXTENSIONS_FILE    =    

MAN_OUTPUT             = man
MAN_EXTENSION          = .3
MAN_SUBDIR             = 
MAN_LINKS              = NO
XML_OUTPUT             = xml
XML_PROGRAMLISTING     = YES
DOCBOOK_OUTPUT         = docbook
DOCBOOK_PROGRAMLISTING = NO


GENERATE_TAGFILE       = 
ALLEXTERNALS           = NO
EXTERNAL_GROUPS        = YES
EXTERNAL_PAGES         = YES

MSCGEN_PATH            = 
DIA_PATH               = 
HIDE_UNDOC_RELATIONS   = YES
HAVE_DOT               = NO
DOT_NUM_THREADS        = 0
DOT_FONTNAME           = Helvetica
DOT_FONTSIZE           = 10
DOT_FONTPATH           = 
CLASS_GRAPH            = YES

GROUP_GRAPHS           = YES
UML_LOOK               = NO
UML_LIMIT_NUM_FIELDS   = 10
TEMPLATE_RELATIONS     = NO

GRAPHICAL_HIERARCHY    = YES
DIRECTORY_GRAPH        = YES
DOT_IMAGE_FORMAT       = png
INTERACTIVE_SVG        = NO
DOT_PATH               = 
DOTFILE_DIRS           = 
MSCFILE_DIRS           = 
DIAFILE_DIRS           = 
PLANTUML_JAR_PATH      = 
DOT_GRAPH_MAX_NODES    = 50
MAX_DOT_GRAPH_DEPTH    = 0
DOT_TRANSPARENT        = NO
DOT_MULTI_TARGETS      = NO
GENERATE_LEGEND        = YES
DOT_CLEANUP            = YES

*/


bool MainWindow::convert_Bool(QByteArray data, QString key)
{
   bool retval = false;
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();

      if (tempStr == "YES") {
         retval = true;
      }
   }

   return retval;
}

int MainWindow::convert_Int(QByteArray data, QString key)
{
   int retval;
   QString temp;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      temp = data.mid(posBeg, posEnd - posBeg);

      retval = temp.toInt();
   }

   return retval;
}

QString MainWindow::convert_Str(QByteArray data, QString key)
{
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1).trimmed();
   }

   return tempStr;
}

QString MainWindow::convert_PlainText(QByteArray data, QString key)
{
   QString tempStr;

   int posBeg = data.indexOf(key);

   if (posBeg > 0) {
      int posEnd = data.indexOf("\n", posBeg);
      QString x = data.mid(posBeg, posEnd - posBeg);

      posBeg  = x.indexOf("=");
      tempStr = x.mid(posBeg + 1);

      tempStr.replace("\"","");
      tempStr = tempStr.trimmed();
   }

   return tempStr;
}

// **

void MainWindow::importDoxy()
{
   QMessageBox quest;
   quest.setWindowTitle(tr("DoxyPressUI Import"));
   quest.setText( tr("Convert Doxygen configuration file to DoxyPress format"));
   quest.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   quest.setDefaultButton(QMessageBox::No);

   int retval = quest.exec();

   if (retval == QMessageBox::Yes) {

      while (true) {

         QString fname = QFileDialog::getOpenFileName(this, tr("Open (old) Doxygen Config File"), m_struct.pathPrior);

         if (fname.isEmpty()) {
            csError(tr("File Open"), tr("No file name was provided"));
            break;
         }

         QFile file(fname);
         if (! file.open(QIODevice::ReadOnly)) {
            csError(tr("Error Opening: ") + fname, tr("Unable to open: ") + file.error());
            break;
         }

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

/*
         // *** get the file - internal usage
         QString path = "z:\\doxygen_wizard\\deploy";

         QFile fileX(path + "/dox_fields.txt");

         if (! fileX.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, tr("Error Saving: ") + m_ConfigFile, tr("Unable to save: ") + file.error());
            break;
         }

         fileX.write(data);
         fileX.close();

         csMsg("File Saved : " + path + "/dox_fields.txt");
         break;
*/

         // verify a few fields to ensure this is an old project file
         if (! data.contains("PROJECT_NAME") || ! data.contains("OUTPUT_DIRECTORY"))  {
            csError(tr("CS Doxygen Import"), tr("The configuration file is missing project information, import canceled"));
            break;
         }

         // get new file config name
         fname = QFileDialog::getSaveFileName(this, tr("Select name for CS Doxygen Config"), m_struct.pathPrior);

         if (fname.isEmpty()) {
            // are you sure?  might want to loop around
            csError(tr("CS Doxygen Import"), tr("No configuraton file name, import canceled"));
            break;

         } else {
            m_ConfigFile = fname;

            clearAllFields();
            convertDoxy(data);

            saveDoxy_Internal();

            json_Write(PATH_PRIOR);
//          if (! m_struct.XXX.contains(m_ConfigFile)) {
//            addRecentFile(m_ConfigFile);
//          }

         }

         // all done, everything is fine
         break;
      }
   }
}















#include "erl_nif.h"
#include "markdown_info.hpp"
#include "gb_common.hpp"

namespace greenbar {

  MarkdownLeafInfo::MarkdownLeafInfo(MarkdownInfoType info_type) {
    type_ = info_type;
    text_ = "";
    url_ = "";
    level_ = -1;
    alignment_ = MD_ALIGN_NONE;
  }

  MarkdownInfoType MarkdownLeafInfo::get_type() {
    return type_;
  }

  MarkdownAlignment MarkdownLeafInfo::get_alignment() {
    return alignment_;
  }

  void MarkdownLeafInfo::set_alignment(MarkdownAlignment align) {
    alignment_ = align;
  }

  void MarkdownLeafInfo::set_type(MarkdownInfoType type) {
    type_ = type;
  }

  void MarkdownLeafInfo::set_text(std::string text) {
    text_ = text;
  }

  void MarkdownLeafInfo::set_url(std::string url) {
    url_ = url;
  }

  void MarkdownLeafInfo::set_level(int level) {
    level_ = level;
  }

  ERL_NIF_TERM MarkdownLeafInfo::to_erl_term(ErlNifEnv* env) {
    gb_priv_s *priv_data = (gb_priv_s*) enif_priv_data(env);
    ERL_NIF_TERM type_name = type_to_atom(this->type_, priv_data);
    ERL_NIF_TERM retval = enif_make_new_map(env);
    enif_make_map_put(env, retval, priv_data->gb_atom_name, type_name, &retval);
    if (this->get_type() != MD_EOL) {
      ERL_NIF_TERM text;
      auto contents = enif_make_new_binary(env, this->text_.size(), &text);
      memcpy(contents, this->text_.c_str(), this->text_.size());
      enif_make_map_put(env, retval, priv_data->gb_atom_text, text, &retval);

      if (this->type_ == MD_HEADER) {
        ERL_NIF_TERM level = enif_make_int(env, this->level_);
        enif_make_map_put(env, retval, priv_data->gb_atom_level, level, &retval);
      }

      if (this->type_ == MD_LINK) {
        ERL_NIF_TERM url;
        auto contents = enif_make_new_binary(env, this->url_.size(), &url);
        memcpy(contents, this->url_.c_str(), this->url_.size());
        enif_make_map_put(env, retval, priv_data->gb_atom_url, url, &retval);
      }
    }
    if (this->alignment_ != MD_ALIGN_NONE) {
      enif_make_map_put(env, retval, priv_data->gb_atom_alignment, alignment_to_atom(this->alignment_, priv_data), &retval);
    }
    return retval;
  }
}

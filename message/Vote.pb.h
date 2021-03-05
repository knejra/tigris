// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Vote.proto

#ifndef PROTOBUF_Vote_2eproto__INCLUDED
#define PROTOBUF_Vote_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_Vote_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsVoteImpl();
void InitDefaultsVote();
inline void InitDefaults() {
  InitDefaultsVote();
}
}  // namespace protobuf_Vote_2eproto
class Vote;
class VoteDefaultTypeInternal;
extern VoteDefaultTypeInternal _Vote_default_instance_;

// ===================================================================

class Vote : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Vote) */ {
 public:
  Vote();
  virtual ~Vote();

  Vote(const Vote& from);

  inline Vote& operator=(const Vote& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Vote(Vote&& from) noexcept
    : Vote() {
    *this = ::std::move(from);
  }

  inline Vote& operator=(Vote&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Vote& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Vote* internal_default_instance() {
    return reinterpret_cast<const Vote*>(
               &_Vote_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(Vote* other);
  friend void swap(Vote& a, Vote& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Vote* New() const PROTOBUF_FINAL { return New(NULL); }

  Vote* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Vote& from);
  void MergeFrom(const Vote& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Vote* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // int32 serverId = 1;
  void clear_serverid();
  static const int kServerIdFieldNumber = 1;
  ::google::protobuf::int32 serverid() const;
  void set_serverid(::google::protobuf::int32 value);

  // int32 term = 2;
  void clear_term();
  static const int kTermFieldNumber = 2;
  ::google::protobuf::int32 term() const;
  void set_term(::google::protobuf::int32 value);

  // bool voteGranted = 3;
  void clear_votegranted();
  static const int kVoteGrantedFieldNumber = 3;
  bool votegranted() const;
  void set_votegranted(bool value);

  // @@protoc_insertion_point(class_scope:Vote)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::int32 serverid_;
  ::google::protobuf::int32 term_;
  bool votegranted_;
  mutable int _cached_size_;
  friend struct ::protobuf_Vote_2eproto::TableStruct;
  friend void ::protobuf_Vote_2eproto::InitDefaultsVoteImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Vote

// int32 serverId = 1;
inline void Vote::clear_serverid() {
  serverid_ = 0;
}
inline ::google::protobuf::int32 Vote::serverid() const {
  // @@protoc_insertion_point(field_get:Vote.serverId)
  return serverid_;
}
inline void Vote::set_serverid(::google::protobuf::int32 value) {
  
  serverid_ = value;
  // @@protoc_insertion_point(field_set:Vote.serverId)
}

// int32 term = 2;
inline void Vote::clear_term() {
  term_ = 0;
}
inline ::google::protobuf::int32 Vote::term() const {
  // @@protoc_insertion_point(field_get:Vote.term)
  return term_;
}
inline void Vote::set_term(::google::protobuf::int32 value) {
  
  term_ = value;
  // @@protoc_insertion_point(field_set:Vote.term)
}

// bool voteGranted = 3;
inline void Vote::clear_votegranted() {
  votegranted_ = false;
}
inline bool Vote::votegranted() const {
  // @@protoc_insertion_point(field_get:Vote.voteGranted)
  return votegranted_;
}
inline void Vote::set_votegranted(bool value) {
  
  votegranted_ = value;
  // @@protoc_insertion_point(field_set:Vote.voteGranted)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Vote_2eproto__INCLUDED

// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AppendEntries.proto

#ifndef PROTOBUF_AppendEntries_2eproto__INCLUDED
#define PROTOBUF_AppendEntries_2eproto__INCLUDED

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

namespace protobuf_AppendEntries_2eproto {
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
void InitDefaultsAppendEntriesImpl();
void InitDefaultsAppendEntries();
inline void InitDefaults() {
  InitDefaultsAppendEntries();
}
}  // namespace protobuf_AppendEntries_2eproto
class AppendEntries;
class AppendEntriesDefaultTypeInternal;
extern AppendEntriesDefaultTypeInternal _AppendEntries_default_instance_;

// ===================================================================

class AppendEntries : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:AppendEntries) */ {
 public:
  AppendEntries();
  virtual ~AppendEntries();

  AppendEntries(const AppendEntries& from);

  inline AppendEntries& operator=(const AppendEntries& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  AppendEntries(AppendEntries&& from) noexcept
    : AppendEntries() {
    *this = ::std::move(from);
  }

  inline AppendEntries& operator=(AppendEntries&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const AppendEntries& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AppendEntries* internal_default_instance() {
    return reinterpret_cast<const AppendEntries*>(
               &_AppendEntries_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(AppendEntries* other);
  friend void swap(AppendEntries& a, AppendEntries& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline AppendEntries* New() const PROTOBUF_FINAL { return New(NULL); }

  AppendEntries* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const AppendEntries& from);
  void MergeFrom(const AppendEntries& from);
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
  void InternalSwap(AppendEntries* other);
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

  // repeated string entries = 5;
  int entries_size() const;
  void clear_entries();
  static const int kEntriesFieldNumber = 5;
  const ::std::string& entries(int index) const;
  ::std::string* mutable_entries(int index);
  void set_entries(int index, const ::std::string& value);
  #if LANG_CXX11
  void set_entries(int index, ::std::string&& value);
  #endif
  void set_entries(int index, const char* value);
  void set_entries(int index, const char* value, size_t size);
  ::std::string* add_entries();
  void add_entries(const ::std::string& value);
  #if LANG_CXX11
  void add_entries(::std::string&& value);
  #endif
  void add_entries(const char* value);
  void add_entries(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& entries() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_entries();

  // int32 term = 1;
  void clear_term();
  static const int kTermFieldNumber = 1;
  ::google::protobuf::int32 term() const;
  void set_term(::google::protobuf::int32 value);

  // int32 leaderId = 2;
  void clear_leaderid();
  static const int kLeaderIdFieldNumber = 2;
  ::google::protobuf::int32 leaderid() const;
  void set_leaderid(::google::protobuf::int32 value);

  // int32 prevLogIndex = 3;
  void clear_prevlogindex();
  static const int kPrevLogIndexFieldNumber = 3;
  ::google::protobuf::int32 prevlogindex() const;
  void set_prevlogindex(::google::protobuf::int32 value);

  // int32 prevLogTerm = 4;
  void clear_prevlogterm();
  static const int kPrevLogTermFieldNumber = 4;
  ::google::protobuf::int32 prevlogterm() const;
  void set_prevlogterm(::google::protobuf::int32 value);

  // int32 leaderCommit = 6;
  void clear_leadercommit();
  static const int kLeaderCommitFieldNumber = 6;
  ::google::protobuf::int32 leadercommit() const;
  void set_leadercommit(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:AppendEntries)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::std::string> entries_;
  ::google::protobuf::int32 term_;
  ::google::protobuf::int32 leaderid_;
  ::google::protobuf::int32 prevlogindex_;
  ::google::protobuf::int32 prevlogterm_;
  ::google::protobuf::int32 leadercommit_;
  mutable int _cached_size_;
  friend struct ::protobuf_AppendEntries_2eproto::TableStruct;
  friend void ::protobuf_AppendEntries_2eproto::InitDefaultsAppendEntriesImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AppendEntries

// int32 term = 1;
inline void AppendEntries::clear_term() {
  term_ = 0;
}
inline ::google::protobuf::int32 AppendEntries::term() const {
  // @@protoc_insertion_point(field_get:AppendEntries.term)
  return term_;
}
inline void AppendEntries::set_term(::google::protobuf::int32 value) {
  
  term_ = value;
  // @@protoc_insertion_point(field_set:AppendEntries.term)
}

// int32 leaderId = 2;
inline void AppendEntries::clear_leaderid() {
  leaderid_ = 0;
}
inline ::google::protobuf::int32 AppendEntries::leaderid() const {
  // @@protoc_insertion_point(field_get:AppendEntries.leaderId)
  return leaderid_;
}
inline void AppendEntries::set_leaderid(::google::protobuf::int32 value) {
  
  leaderid_ = value;
  // @@protoc_insertion_point(field_set:AppendEntries.leaderId)
}

// int32 prevLogIndex = 3;
inline void AppendEntries::clear_prevlogindex() {
  prevlogindex_ = 0;
}
inline ::google::protobuf::int32 AppendEntries::prevlogindex() const {
  // @@protoc_insertion_point(field_get:AppendEntries.prevLogIndex)
  return prevlogindex_;
}
inline void AppendEntries::set_prevlogindex(::google::protobuf::int32 value) {
  
  prevlogindex_ = value;
  // @@protoc_insertion_point(field_set:AppendEntries.prevLogIndex)
}

// int32 prevLogTerm = 4;
inline void AppendEntries::clear_prevlogterm() {
  prevlogterm_ = 0;
}
inline ::google::protobuf::int32 AppendEntries::prevlogterm() const {
  // @@protoc_insertion_point(field_get:AppendEntries.prevLogTerm)
  return prevlogterm_;
}
inline void AppendEntries::set_prevlogterm(::google::protobuf::int32 value) {
  
  prevlogterm_ = value;
  // @@protoc_insertion_point(field_set:AppendEntries.prevLogTerm)
}

// repeated string entries = 5;
inline int AppendEntries::entries_size() const {
  return entries_.size();
}
inline void AppendEntries::clear_entries() {
  entries_.Clear();
}
inline const ::std::string& AppendEntries::entries(int index) const {
  // @@protoc_insertion_point(field_get:AppendEntries.entries)
  return entries_.Get(index);
}
inline ::std::string* AppendEntries::mutable_entries(int index) {
  // @@protoc_insertion_point(field_mutable:AppendEntries.entries)
  return entries_.Mutable(index);
}
inline void AppendEntries::set_entries(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:AppendEntries.entries)
  entries_.Mutable(index)->assign(value);
}
#if LANG_CXX11
inline void AppendEntries::set_entries(int index, ::std::string&& value) {
  // @@protoc_insertion_point(field_set:AppendEntries.entries)
  entries_.Mutable(index)->assign(std::move(value));
}
#endif
inline void AppendEntries::set_entries(int index, const char* value) {
  GOOGLE_DCHECK(value != NULL);
  entries_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:AppendEntries.entries)
}
inline void AppendEntries::set_entries(int index, const char* value, size_t size) {
  entries_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:AppendEntries.entries)
}
inline ::std::string* AppendEntries::add_entries() {
  // @@protoc_insertion_point(field_add_mutable:AppendEntries.entries)
  return entries_.Add();
}
inline void AppendEntries::add_entries(const ::std::string& value) {
  entries_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:AppendEntries.entries)
}
#if LANG_CXX11
inline void AppendEntries::add_entries(::std::string&& value) {
  entries_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:AppendEntries.entries)
}
#endif
inline void AppendEntries::add_entries(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  entries_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:AppendEntries.entries)
}
inline void AppendEntries::add_entries(const char* value, size_t size) {
  entries_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:AppendEntries.entries)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
AppendEntries::entries() const {
  // @@protoc_insertion_point(field_list:AppendEntries.entries)
  return entries_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
AppendEntries::mutable_entries() {
  // @@protoc_insertion_point(field_mutable_list:AppendEntries.entries)
  return &entries_;
}

// int32 leaderCommit = 6;
inline void AppendEntries::clear_leadercommit() {
  leadercommit_ = 0;
}
inline ::google::protobuf::int32 AppendEntries::leadercommit() const {
  // @@protoc_insertion_point(field_get:AppendEntries.leaderCommit)
  return leadercommit_;
}
inline void AppendEntries::set_leadercommit(::google::protobuf::int32 value) {
  
  leadercommit_ = value;
  // @@protoc_insertion_point(field_set:AppendEntries.leaderCommit)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_AppendEntries_2eproto__INCLUDED

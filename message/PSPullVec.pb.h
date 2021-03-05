// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: PSPullVec.proto

#ifndef PROTOBUF_PSPullVec_2eproto__INCLUDED
#define PROTOBUF_PSPullVec_2eproto__INCLUDED

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

namespace protobuf_PSPullVec_2eproto {
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
void InitDefaultsPSPullVecImpl();
void InitDefaultsPSPullVec();
inline void InitDefaults() {
  InitDefaultsPSPullVec();
}
}  // namespace protobuf_PSPullVec_2eproto
class PSPullVec;
class PSPullVecDefaultTypeInternal;
extern PSPullVecDefaultTypeInternal _PSPullVec_default_instance_;

// ===================================================================

class PSPullVec : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:PSPullVec) */ {
 public:
  PSPullVec();
  virtual ~PSPullVec();

  PSPullVec(const PSPullVec& from);

  inline PSPullVec& operator=(const PSPullVec& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PSPullVec(PSPullVec&& from) noexcept
    : PSPullVec() {
    *this = ::std::move(from);
  }

  inline PSPullVec& operator=(PSPullVec&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const PSPullVec& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PSPullVec* internal_default_instance() {
    return reinterpret_cast<const PSPullVec*>(
               &_PSPullVec_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(PSPullVec* other);
  friend void swap(PSPullVec& a, PSPullVec& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PSPullVec* New() const PROTOBUF_FINAL { return New(NULL); }

  PSPullVec* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const PSPullVec& from);
  void MergeFrom(const PSPullVec& from);
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
  void InternalSwap(PSPullVec* other);
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

  // repeated uint64 keys = 1;
  int keys_size() const;
  void clear_keys();
  static const int kKeysFieldNumber = 1;
  ::google::protobuf::uint64 keys(int index) const;
  void set_keys(int index, ::google::protobuf::uint64 value);
  void add_keys(::google::protobuf::uint64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >&
      keys() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >*
      mutable_keys();

  // int32 timestamp = 2;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 2;
  ::google::protobuf::int32 timestamp() const;
  void set_timestamp(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:PSPullVec)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint64 > keys_;
  mutable int _keys_cached_byte_size_;
  ::google::protobuf::int32 timestamp_;
  mutable int _cached_size_;
  friend struct ::protobuf_PSPullVec_2eproto::TableStruct;
  friend void ::protobuf_PSPullVec_2eproto::InitDefaultsPSPullVecImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PSPullVec

// repeated uint64 keys = 1;
inline int PSPullVec::keys_size() const {
  return keys_.size();
}
inline void PSPullVec::clear_keys() {
  keys_.Clear();
}
inline ::google::protobuf::uint64 PSPullVec::keys(int index) const {
  // @@protoc_insertion_point(field_get:PSPullVec.keys)
  return keys_.Get(index);
}
inline void PSPullVec::set_keys(int index, ::google::protobuf::uint64 value) {
  keys_.Set(index, value);
  // @@protoc_insertion_point(field_set:PSPullVec.keys)
}
inline void PSPullVec::add_keys(::google::protobuf::uint64 value) {
  keys_.Add(value);
  // @@protoc_insertion_point(field_add:PSPullVec.keys)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >&
PSPullVec::keys() const {
  // @@protoc_insertion_point(field_list:PSPullVec.keys)
  return keys_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >*
PSPullVec::mutable_keys() {
  // @@protoc_insertion_point(field_mutable_list:PSPullVec.keys)
  return &keys_;
}

// int32 timestamp = 2;
inline void PSPullVec::clear_timestamp() {
  timestamp_ = 0;
}
inline ::google::protobuf::int32 PSPullVec::timestamp() const {
  // @@protoc_insertion_point(field_get:PSPullVec.timestamp)
  return timestamp_;
}
inline void PSPullVec::set_timestamp(::google::protobuf::int32 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:PSPullVec.timestamp)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_PSPullVec_2eproto__INCLUDED

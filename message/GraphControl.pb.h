// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: GraphControl.proto

#ifndef PROTOBUF_GraphControl_2eproto__INCLUDED
#define PROTOBUF_GraphControl_2eproto__INCLUDED

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

namespace protobuf_GraphControl_2eproto {
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
void InitDefaultsGContorlImpl();
void InitDefaultsGContorl();
inline void InitDefaults() {
  InitDefaultsGContorl();
}
}  // namespace protobuf_GraphControl_2eproto
class GContorl;
class GContorlDefaultTypeInternal;
extern GContorlDefaultTypeInternal _GContorl_default_instance_;

// ===================================================================

class GContorl : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:GContorl) */ {
 public:
  GContorl();
  virtual ~GContorl();

  GContorl(const GContorl& from);

  inline GContorl& operator=(const GContorl& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  GContorl(GContorl&& from) noexcept
    : GContorl() {
    *this = ::std::move(from);
  }

  inline GContorl& operator=(GContorl&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const GContorl& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const GContorl* internal_default_instance() {
    return reinterpret_cast<const GContorl*>(
               &_GContorl_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(GContorl* other);
  friend void swap(GContorl& a, GContorl& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline GContorl* New() const PROTOBUF_FINAL { return New(NULL); }

  GContorl* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const GContorl& from);
  void MergeFrom(const GContorl& from);
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
  void InternalSwap(GContorl* other);
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

  // int32 type = 1;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::google::protobuf::int32 type() const;
  void set_type(::google::protobuf::int32 value);

  // int32 timestamp = 2;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 2;
  ::google::protobuf::int32 timestamp() const;
  void set_timestamp(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:GContorl)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::int32 type_;
  ::google::protobuf::int32 timestamp_;
  mutable int _cached_size_;
  friend struct ::protobuf_GraphControl_2eproto::TableStruct;
  friend void ::protobuf_GraphControl_2eproto::InitDefaultsGContorlImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// GContorl

// int32 type = 1;
inline void GContorl::clear_type() {
  type_ = 0;
}
inline ::google::protobuf::int32 GContorl::type() const {
  // @@protoc_insertion_point(field_get:GContorl.type)
  return type_;
}
inline void GContorl::set_type(::google::protobuf::int32 value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:GContorl.type)
}

// int32 timestamp = 2;
inline void GContorl::clear_timestamp() {
  timestamp_ = 0;
}
inline ::google::protobuf::int32 GContorl::timestamp() const {
  // @@protoc_insertion_point(field_get:GContorl.timestamp)
  return timestamp_;
}
inline void GContorl::set_timestamp(::google::protobuf::int32 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:GContorl.timestamp)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_GraphControl_2eproto__INCLUDED
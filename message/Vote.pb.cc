// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Vote.proto

#include "Vote.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
class VoteDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Vote>
      _instance;
} _Vote_default_instance_;
namespace protobuf_Vote_2eproto {
void InitDefaultsVoteImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::_Vote_default_instance_;
    new (ptr) ::Vote();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::Vote::InitAsDefaultInstance();
}

void InitDefaultsVote() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsVoteImpl);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Vote, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Vote, serverid_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Vote, term_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::Vote, votegranted_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::Vote)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::_Vote_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "Vote.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\nVote.proto\";\n\004Vote\022\020\n\010serverId\030\001 \001(\005\022\014"
      "\n\004term\030\002 \001(\005\022\023\n\013voteGranted\030\003 \001(\010b\006proto"
      "3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 81);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "Vote.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_Vote_2eproto

// ===================================================================

void Vote::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Vote::kServerIdFieldNumber;
const int Vote::kTermFieldNumber;
const int Vote::kVoteGrantedFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Vote::Vote()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_Vote_2eproto::InitDefaultsVote();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:Vote)
}
Vote::Vote(const Vote& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&serverid_, &from.serverid_,
    static_cast<size_t>(reinterpret_cast<char*>(&votegranted_) -
    reinterpret_cast<char*>(&serverid_)) + sizeof(votegranted_));
  // @@protoc_insertion_point(copy_constructor:Vote)
}

void Vote::SharedCtor() {
  ::memset(&serverid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&votegranted_) -
      reinterpret_cast<char*>(&serverid_)) + sizeof(votegranted_));
  _cached_size_ = 0;
}

Vote::~Vote() {
  // @@protoc_insertion_point(destructor:Vote)
  SharedDtor();
}

void Vote::SharedDtor() {
}

void Vote::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Vote::descriptor() {
  ::protobuf_Vote_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_Vote_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Vote& Vote::default_instance() {
  ::protobuf_Vote_2eproto::InitDefaultsVote();
  return *internal_default_instance();
}

Vote* Vote::New(::google::protobuf::Arena* arena) const {
  Vote* n = new Vote;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Vote::Clear() {
// @@protoc_insertion_point(message_clear_start:Vote)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&serverid_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&votegranted_) -
      reinterpret_cast<char*>(&serverid_)) + sizeof(votegranted_));
  _internal_metadata_.Clear();
}

bool Vote::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Vote)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 serverId = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &serverid_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 term = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &term_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool voteGranted = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &votegranted_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Vote)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Vote)
  return false;
#undef DO_
}

void Vote::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Vote)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 serverId = 1;
  if (this->serverid() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->serverid(), output);
  }

  // int32 term = 2;
  if (this->term() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->term(), output);
  }

  // bool voteGranted = 3;
  if (this->votegranted() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->votegranted(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:Vote)
}

::google::protobuf::uint8* Vote::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:Vote)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 serverId = 1;
  if (this->serverid() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->serverid(), target);
  }

  // int32 term = 2;
  if (this->term() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->term(), target);
  }

  // bool voteGranted = 3;
  if (this->votegranted() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(3, this->votegranted(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Vote)
  return target;
}

size_t Vote::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Vote)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // int32 serverId = 1;
  if (this->serverid() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->serverid());
  }

  // int32 term = 2;
  if (this->term() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->term());
  }

  // bool voteGranted = 3;
  if (this->votegranted() != 0) {
    total_size += 1 + 1;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Vote::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:Vote)
  GOOGLE_DCHECK_NE(&from, this);
  const Vote* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Vote>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:Vote)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:Vote)
    MergeFrom(*source);
  }
}

void Vote::MergeFrom(const Vote& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:Vote)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.serverid() != 0) {
    set_serverid(from.serverid());
  }
  if (from.term() != 0) {
    set_term(from.term());
  }
  if (from.votegranted() != 0) {
    set_votegranted(from.votegranted());
  }
}

void Vote::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:Vote)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Vote::CopyFrom(const Vote& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Vote)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Vote::IsInitialized() const {
  return true;
}

void Vote::Swap(Vote* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Vote::InternalSwap(Vote* other) {
  using std::swap;
  swap(serverid_, other->serverid_);
  swap(term_, other->term_);
  swap(votegranted_, other->votegranted_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Vote::GetMetadata() const {
  protobuf_Vote_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_Vote_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
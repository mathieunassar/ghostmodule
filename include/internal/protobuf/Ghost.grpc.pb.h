// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: Ghost.proto
#ifndef GRPC_Ghost_2eproto__INCLUDED
#define GRPC_Ghost_2eproto__INCLUDED

#include "Ghost.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace Ghost {
namespace internal {
namespace protobuf {

class PublisherSubscriberService final {
 public:
  static constexpr char const* service_full_name() {
    return "Ghost.internal.protobuf.PublisherSubscriberService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientReaderInterface< ::Ghost::internal::protobuf::GhostMessage>> publications(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::Ghost::internal::protobuf::GhostMessage>>(publicationsRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::Ghost::internal::protobuf::GhostMessage>> Asyncpublications(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::Ghost::internal::protobuf::GhostMessage>>(AsyncpublicationsRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::Ghost::internal::protobuf::GhostMessage>> PrepareAsyncpublications(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::Ghost::internal::protobuf::GhostMessage>>(PrepareAsyncpublicationsRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientReaderInterface< ::Ghost::internal::protobuf::GhostMessage>* publicationsRaw(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::Ghost::internal::protobuf::GhostMessage>* AsyncpublicationsRaw(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::Ghost::internal::protobuf::GhostMessage>* PrepareAsyncpublicationsRaw(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientReader< ::Ghost::internal::protobuf::GhostMessage>> publications(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::Ghost::internal::protobuf::GhostMessage>>(publicationsRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::Ghost::internal::protobuf::GhostMessage>> Asyncpublications(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::Ghost::internal::protobuf::GhostMessage>>(AsyncpublicationsRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::Ghost::internal::protobuf::GhostMessage>> PrepareAsyncpublications(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::Ghost::internal::protobuf::GhostMessage>>(PrepareAsyncpublicationsRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientReader< ::Ghost::internal::protobuf::GhostMessage>* publicationsRaw(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request) override;
    ::grpc::ClientAsyncReader< ::Ghost::internal::protobuf::GhostMessage>* AsyncpublicationsRaw(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReader< ::Ghost::internal::protobuf::GhostMessage>* PrepareAsyncpublicationsRaw(::grpc::ClientContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_publications_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status publications(::grpc::ServerContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader* request, ::grpc::ServerWriter< ::Ghost::internal::protobuf::GhostMessage>* writer);
  };
  template <class BaseClass>
  class WithAsyncMethod_publications : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_publications() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_publications() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status publications(::grpc::ServerContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader* request, ::grpc::ServerWriter< ::Ghost::internal::protobuf::GhostMessage>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestpublications(::grpc::ServerContext* context, ::Ghost::internal::protobuf::GhostMessageHeader* request, ::grpc::ServerAsyncWriter< ::Ghost::internal::protobuf::GhostMessage>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(0, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_publications<Service > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_publications : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_publications() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_publications() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status publications(::grpc::ServerContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader* request, ::grpc::ServerWriter< ::Ghost::internal::protobuf::GhostMessage>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  typedef Service StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_publications : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithSplitStreamingMethod_publications() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::SplitServerStreamingHandler< ::Ghost::internal::protobuf::GhostMessageHeader, ::Ghost::internal::protobuf::GhostMessage>(std::bind(&WithSplitStreamingMethod_publications<BaseClass>::Streamedpublications, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithSplitStreamingMethod_publications() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status publications(::grpc::ServerContext* context, const ::Ghost::internal::protobuf::GhostMessageHeader* request, ::grpc::ServerWriter< ::Ghost::internal::protobuf::GhostMessage>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status Streamedpublications(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::Ghost::internal::protobuf::GhostMessageHeader,::Ghost::internal::protobuf::GhostMessage>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_publications<Service > SplitStreamedService;
  typedef WithSplitStreamingMethod_publications<Service > StreamedService;
};

class ServerClientService final {
 public:
  static constexpr char const* service_full_name() {
    return "Ghost.internal.protobuf.ServerClientService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>> connect(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>>(connectRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>> Asyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>>(AsyncconnectRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>> PrepareAsyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>>(PrepareAsyncconnectRaw(context, cq));
    }
  private:
    virtual ::grpc::ClientReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* connectRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* AsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* PrepareAsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>> connect(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>>(connectRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>> Asyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>>(AsyncconnectRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>> PrepareAsyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>>(PrepareAsyncconnectRaw(context, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* connectRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* AsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* PrepareAsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_connect_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_connect : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_connect() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_connect() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* stream) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestconnect(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_connect<Service > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_connect : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_connect() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_connect() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::Ghost::internal::protobuf::GhostMessage, ::Ghost::internal::protobuf::GhostMessage>* stream) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  typedef Service StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef Service StreamedService;
};

}  // namespace protobuf
}  // namespace internal
}  // namespace Ghost


#endif  // GRPC_Ghost_2eproto__INCLUDED

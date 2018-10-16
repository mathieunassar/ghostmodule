// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: Ghost.proto
#ifndef GRPC_Ghost_2eproto__INCLUDED
#define GRPC_Ghost_2eproto__INCLUDED

#include "Ghost.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
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

namespace ghost {
namespace internal {
namespace protobuf {

class ServerClientService final {
 public:
  static constexpr char const* service_full_name() {
    return "ghost.internal.protobuf.ServerClientService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>> connect(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>>(connectRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>> Asyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>>(AsyncconnectRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>> PrepareAsyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>>(PrepareAsyncconnectRaw(context, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
    };
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>* connectRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>* AsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::google::protobuf::Any, ::google::protobuf::Any>* PrepareAsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>> connect(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>>(connectRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>> Asyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>>(AsyncconnectRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>> PrepareAsyncconnect(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>>(PrepareAsyncconnectRaw(context, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* connectRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* AsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* PrepareAsyncconnectRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_connect_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* stream);
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
    ::grpc::Status connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestconnect(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
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
    ::grpc::Status connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_connect : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithRawMethod_connect() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_connect() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status connect(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::google::protobuf::Any, ::google::protobuf::Any>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void Requestconnect(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef Service StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef Service StreamedService;
};

}  // namespace protobuf
}  // namespace internal
}  // namespace ghost


#endif  // GRPC_Ghost_2eproto__INCLUDED

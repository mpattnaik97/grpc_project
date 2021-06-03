#include <grpcpp/grpcpp.h>
#include <fstream>
#include <iostream>
#include "emotech.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;

using emotech::ChunkRequest;
using emotech::ChunkResponse;
using emotech::EmotechService;
using emotech::IntRequest;
using emotech::IntResponse;
using emotech::StringRequest;
using emotech::StringResponse;

using namespace std;

class EmotechServiceImpl final : public EmotechService::Service {
  Status exchangeNumber(ServerContext* context, const IntRequest* request,
                        IntResponse* response) override {
    std::cout << "Server: Received number -> " << request->number() << endl;

    int res = request->number();

    res = res * res;

    std::cout << "Server: Sending the square of the number back i.e. -> " << res
              << endl;

    response->set_number(res);

    return Status::OK;
  }

  Status exchangeString(ServerContext* context, const StringRequest* request,
                        StringResponse* response) override {
    std::cout << "Server: Received string -> " << request->requesttext()
              << endl;
    
    string reverse = request->requesttext();

    int n = reverse.length();
    
    for (int i = 0; i < n / 2; i++){

      swap(reverse[i], reverse[n-i-1]);

    }
  
    std::cout << "Server: Sending its reverse back to server -> "
              << reverse << endl;

    response->set_responsetext(reverse);

    return Status::OK;
  }

  Status exchangeFile(ServerContext* context,
                      ServerReader<ChunkRequest>* reader,
                      ChunkResponse* response) override {
    ChunkRequest request;

    ofstream fout;

    string path;

    cout << "Server: Received file. Initializing IO process..." << endl;

    // reader->Read(&request);
    while (reader->Read(&request)) {
      if (fout.is_open()) {

        fout << request.content();

      } else {
        path = request.content();

        string filename = path.substr(path.find_last_of("/\\") + 1);

        path = "clientfiles/" + filename;

        cout << "Server: Setting path to " << path << endl;

        cout << "Server: Creating file ..." << endl;
        fout.open(path, ios::out);
      }
    }
    std::cout << "Server: Read complete, closing the file" << std::endl;

    fout.close();

    response->set_response("File Written Successfully");

    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:5000");

  EmotechServiceImpl service;

  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server Listening on port port: " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}

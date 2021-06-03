#include <grpcpp/grpcpp.h>
#include <string.h>
#include <fstream>
#include "emotech.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientWriter;
using grpc::Status;

using emotech::ChunkRequest;
using emotech::ChunkResponse;
using emotech::EmotechService;
using emotech::IntRequest;
using emotech::IntResponse;
using emotech::StringRequest;
using emotech::StringResponse;

using namespace std;

class EmotechClient {
 public:
  EmotechClient(std::shared_ptr<Channel> channel)
      : stub_(EmotechService::NewStub(channel)) {}

  void exchangeNumber(int num) {
    IntRequest req;
    req.set_number(num);

    IntResponse res;
    ClientContext context;

    std::cout << "Client: Sending number -> " << req.number() << endl;

    Status status = stub_->exchangeNumber(&context, req, &res);

    if (status.ok()) {
      std::cout << "Client: Received number from server -> " << res.number()
                << endl;
      return;

    } else {
      std::cerr << endl
                << "Client: Error -> "<< status.error_code() << " - " << status.error_message()
                << std::endl << endl;
      return;
    }
  }

  void exchangeString(std::string alpha) {
    StringRequest req;
    req.set_requesttext(alpha);

    StringResponse res;
    ClientContext context;

    std::cout << "Client: Sending string -> " << req.requesttext() << endl;

    Status status = stub_->exchangeString(&context, req, &res);

    if (status.ok()) {
      std::cout << "Client: Received string from server -> "
                << res.responsetext() << endl;
      return;

    } else {
      std::cerr << endl
                << "Client: Error -> "<< status.error_code() << " - " << status.error_message()
                << std::endl << endl;
      return;
    }
  }

  void exchangeFile(std::string path) {
    ChunkRequest req;
    ChunkResponse res;

    ClientContext context;

    std::unique_ptr<ClientWriter<ChunkRequest>> writer(
        stub_->exchangeFile(&context, &res));

    std::ifstream fin;

    fin.open(path, std::ios::in);

    req.set_content(path);

    std::cout << "Client: Sending file path ->" << req.content() << endl;

    writer->Write(req);

    cout << "Client: Streaming file content in bytes" << endl;

    while (!fin.eof()) {
      char* text = new char[2048];

      fin.read(text, 2048);

      req.clear_content();
      req.set_content(std::string(text, 2048));

      writer->Write(req);

      free(text);

      if (fin.eof()) {
        break;
      }
    }

    fin.close();

    writer->WritesDone();

    Status status = writer->Finish();

    if (status.ok()) {
      cout << "Client: File contents sent, closing stream" << endl;
      cout << "Client: Response from the server -> " << res.response() << endl;
    } else {
      std::cerr << endl
                << "Client: Error -> "<< status.error_code() << " - " << status.error_message()
                << std::endl << endl;
    }
    return;
  }

 private:
  unique_ptr<EmotechService::Stub> stub_;
};

void RunClient(int intReq, string stringReq, string path, int arg) {
  string target_address("0.0.0.0:5000");

  EmotechClient client(
      grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));

  switch (arg) {
    case 1:

      client.exchangeNumber(intReq);

      break;
    case 2:

      client.exchangeString(stringReq);

      break;
    case 3:

      client.exchangeFile(path);

      break;

    case 4:
      client.exchangeNumber(intReq);
      client.exchangeString(stringReq);

      break;

    case 5:
      client.exchangeNumber(intReq);
      client.exchangeFile(path);
      break;

    case 6:
      client.exchangeString(stringReq);
      client.exchangeFile(path);
      break;

    case 0:
    case 10:

      client.exchangeNumber(intReq);
      client.exchangeString(stringReq);
      client.exchangeFile(path);

      break;
    default:
      break;
  }
}

bool is_number(string param) {
  for (char const& c : param) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}

bool validateParams(char** params) {
  ifstream fin(params[3]);

  bool correctParams = false;

  correctParams = is_number(params[1]) && fin.is_open();
  fin.close();

  return correctParams;
}

int parse_params(int count, char** params) {
  ifstream fin(params[2]);
  if (!is_number(params[1])) {
    if (fin.is_open()) {
      return 3;
    }
    fin.close();
    return 0;
  }
  if (fin.is_open()) {
    fin.close();
    return 2;
  }
  fin.close();
  return 1;
}

void noParams() {
  std::cout << "Client: No params provided, sending request with test params"
            << endl;

  RunClient(10, "perseverance is key", "test_image.png", 0);
  return;
}

void oneParam(char** argv) {
  ifstream fin;
  fin.open(argv[1]);

  if (is_number((argv[1]))) {
    fin.close();
    std::cout
        << "Client: Integer argument detected, proceeding with the request"
        << endl;

    RunClient(atoi(argv[1]), "", "", 1);

    return;
  }

  if (fin.is_open()) {
    fin.close();

    std::cout << "Client: File Path argument detected, proceeding with "
                 "the request"
              << endl;

    RunClient(0, "", argv[1], 3);

    return;
  }

  std::cout << "Client: String argument detected, proceeding with the request"
            << endl;
  RunClient(0, argv[1], "", 2);

  fin.close();
  return;
}

void twoParams(char** argv) {
  int type = parse_params(2, argv);

  if (type == 1) {
    std::cout
        << "Client: Int and String argument detected, proceeding with the "
           "request"
        << endl;

    RunClient(atoi(argv[1]), argv[2], "", 4);

  } else if (type == 2) {
    std::cout << "Client: Int and File path argument detected, proceeding with "
                 "the "
                 "request"
              << endl;
    RunClient(atoi(argv[1]), "", argv[2], 5);

  } else if (type == 3) {
    std::cout << "Client: String and File path argument detected, proceeding "
                 "with the "
                 "request"
              << endl;
    RunClient(0, argv[1], argv[2], 6);

  } else {
    std::cout << "Client: Invalid argument detected, please follow the sytax "
                 "for argmunets in the readme"
              << endl;
  }
}

void threeParams(char** argv) {
  if (validateParams(argv)) {
    std::cout << "Client: Int, String and File Path detected, proceeding with "
                 "request"
              << endl;

    RunClient(atoi(argv[1]), argv[2], argv[3], 10);

  } else {
    std::cout << "Client: Invalid argument detected, please follow the sytax "
                 "for argmunets in the readme"
              << endl;
  }
}

int main(int argc, char** argv) {
  
  cout << endl;
  
  switch (argc) {
    case 1:

      noParams();

      break;

    case 2:

      oneParam(argv);

      break;

    case 3:

      twoParams(argv);

      break;

    case 4:

      threeParams(argv);

      break;
  }

  return 0;
}

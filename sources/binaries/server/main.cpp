#include <iostream>
#include <caffe-datamining/system_config.hpp>
#include <devfest2016/classifier.hpp>
#include <devfest2016/signal.hpp>

#include <boost/program_options.hpp>

#include <nx/nx.hpp>
#include <nx/multipart.hpp>

#include <multipart/Parser.h>

#include <regex>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <experimental/string_view>

namespace po = boost::program_options;

namespace std {
    using string_view = experimental::string_view;
}

int main(int argc, char** argv)
{
    caffe_datamining::system_config sc;
    auto share_dir = sc.share_dir();

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("model_file", po::value<std::string>()->default_value(share_dir + "/cifar10_quick.prototxt"), "Model file descriptor")
        ("trained_file", po::value<std::string>()->default_value(share_dir + "/cifar10_quick_iter_5000.caffemodel.h5"), "trained file")
        ("mean_file", po::value<std::string>()->default_value(share_dir + "/mean.binaryproto"), "Mean file")
        ("labels_file", po::value<std::string>()->default_value(share_dir + "/batches.meta.txt"), "Labels file")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    auto model_file = vm["model_file"].as<std::string>();
    auto trained_file = vm["trained_file"].as<std::string>();
    auto mean_file = vm["mean_file"].as<std::string>();
    auto labels_file = vm["labels_file"].as<std::string>();

    std::string bind_addr = "0.0.0.0";
    uint16_t    bind_port = 8081;

    auto classifier = std::make_unique<devfest2016::classifier>(
        model_file,
        trained_file,
        mean_file,
        labels_file
    ); 

    using namespace nx;

    httpd srv;
    MPFD::Parser  part_parser;
    part_parser.SetTempDirForFileUpload("/tmp");
    part_parser.SetMaxCollectedDataLength(10*1024*1024);

    srv(GET) / "hello" = [&](const request& req, buffer& data, reply& rep) {
        rep 
            << text_plain
            << "Hello World"
            ; 
    };

    srv(POST) / "predict_from_file" = [&](const request& req, buffer& data, reply& rep) {
        try {
            auto content_type = req.h("Content-Type");
            part_parser.SetContentType(content_type);
            part_parser.AcceptSomeData(&(data[0]), data.size());
            auto fields = part_parser.GetFieldsMap();
       

            for (const auto& f : fields)  {
                
                if (f.second->GetType() == MPFD::Field::FileType) {
                    std::cout << "Evaluate filename: " << f.second->GetTempFileName() << std::endl;

                    auto result = classifier->classify(f.second->GetTempFileName());

                    cxxu::rmfile(f.second->GetTempFileName());
                    rep << 
                        nx::json(result)
                        ;
                }
            }
        }
        catch(MPFD::Exception& e)  {
            std::cerr << "Exception parser: " << e.GetError() << std::endl;
        }

        rep 
            << text_plain
            << "File uploaded file size: " << data.size()
            ; 
    };

    srv(make_endpoint(bind_addr, bind_port));
    std::cout << "Serve at " << bind_addr << ":" << bind_port << std::endl;

    devfest2016::signal sig;
    sig();
    
    std::exit(0);
}

#include <iostream>
#include <caffe-datamining/system_config.hpp>
#include <devfest2016/classifier.hpp>
#include <devfest2016/signal.hpp>

#include <boost/program_options.hpp>

#include <nx/nx.hpp>
#include <nx/multipart.hpp>

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
        ("file", po::value<std::string>()->default_value(""), "Image file to parse")
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

    auto file = vm["file"].as<std::string>();
    if (file.empty() || !cxxu::exists(file)) {
        std::cerr << "Not file " << file << " found";
        std::exit(1);
    }

    auto classifier = std::make_unique<devfest2016::classifier>(
         model_file,
         trained_file,
         mean_file,
         labels_file
    ); 

    auto result = classifier->classify(file);
    std::sort(result.predictions.begin(), result.predictions.end(), [](const auto& a, const auto& b) { 
        return a.second > b.second;
     });

    std::cout << "Result: " << std::endl;
    for (const auto& r : result.predictions) {
        std::cout << r.first << ": " << std::fixed << std::setprecision(4) << r.second << std::endl;
    }

    std::exit(0);
}

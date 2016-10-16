#include <iostream>
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
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("model_file", po::value<std::string>()->default_value(""), "Model file descriptor")
        ("trained_file", po::value<std::string>()->default_value(""), "trained file")
        ("mean_file", po::value<std::string>()->default_value(""), "Mean file")
        ("labels_file", po::value<std::string>()->default_value(""), "Labels file")
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


    // auto classifier = std::make_unique<devfest2016::classifier>(
    //     model_file,
    //     trained_file,
    //     mean_file,
    //     labels_file
    // ); 

    std::exit(0);
}

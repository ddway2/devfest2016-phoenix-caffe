#include <iostream>
#include <devfest2016/classifier.hpp>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("model_file", po::value<std::string>(), "Model file descriptor")
        ("trained_file", po::value<std::string>(), "trained file")
        ("mean_file", po::value<std::string>(), "Mean file")
        ("labels_file", po::value<std::string>(), "Labels file")
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

    auto classifier = st::make_unique<devfest2016::classifier>(
        model_file,
        trained_file,
        mean_file,
        labels_file
    ); 
}

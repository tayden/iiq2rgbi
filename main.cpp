#include <iostream>
#include <P1Image.hpp>
#include <string>


int main(int argc, const char **argv) {
    std::cout << "Open IIQ file" << std::endl;

    std::string input_file = "/home/taylor/CLionProjects/iiq2rgbi/testfiles/2023-03-18362_RGB_233769.IIQ";
    std::string output_file = "/home/taylor/CLionProjects/iiq2rgbi/testfiles/output.tiff";

    // Open an IIQ-file (edit the name and path to an IIQ-file)
    P1::ImageSdk::RawImage iiq(input_file);

    // Setup a convert config with the description about how to convert image into Rgb domain
    P1::ImageSdk::ConvertConfig config;
    config.SetOutputScale(.05);

    std::cout << "Do the conversion..." << std::endl;
    P1::ImageSdk::BitmapImage bitmap = iiq.Convert(config);

    std::cout << "Write image to tiff file..." << std::endl;
    // The resulting bitmap can now be stored to a tiff file.
    P1::ImageSdk::TiffConfig tiff;
    P1::ImageSdk::TiffWriter(output_file, bitmap, iiq, tiff);

    std::cout << "Done!" << std::endl;
    return 0;
}
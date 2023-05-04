#include <iostream>
#include <string>

#include <P1Image.hpp>
#include <P1ImageStacking.hpp>
#include <P1ImageLensDistortion.hpp>
#include <P1ImageExceptions.hpp>

int main(int argc, const char **argv) {
    std::cout << "Open IIQ file" << std::endl;

    try {
        // TODO: These values need to be dynamic
        std::string rgb_file = R"(..\..\testfiles\2023-03-18362_RGB_233769.IIQ)";
        std::string nir_file = R"(..\..\testfiles\2023-03-18362_NIR_298107.IIQ)";
        std::string output_file = R"(..\..\testfiles\output.tiff)";
        P1::ImageSdk::GeometricCorrection rgb_cal{
                .pixelSize = 0.0046,
                .focalLength = 51.8176,
                .xp = -0.0186,
                .yp = -0.1029,
                .k1 = 0.0000156691,
                .k2 = -5.15017e-9,
                .k3 = 7.67762e-13,
                .p1 = 5.69591e-7,
                .p2 = -0.00000166097,
                .b1 = -9.65496e-8,
                .b2 = -6.61227e-9,
        };
        P1::ImageSdk::GeometricCorrection nir_cal{
                .pixelSize = 0.0046,
                .focalLength = 51.8057,
                .xp = -0.0579,
                .yp = 0.0524,
                .k1 = 0.000014827,
                .k2 = -3.64981e-9,
                .k3 = -8.11134e-14,
                .p1 = -1.36228e-7,
                .p2 = -0.00000214019,
                .b1 = -0.00000755085,
                .b2 = 0.0000399366,
        };

        // Open an IIQ-file (edit the name and path to an IIQ-file)
        P1::ImageSdk::RawImage rgb_iiq(rgb_file);
        P1::ImageSdk::RawImage nir_iiq(nir_file);

        // Set up a convert config with the description about how to convert image into Rgb domain
        P1::ImageSdk::ConvertConfig config;
        config.SetOutputScale(0.25);  // Use 1/4 scale

        std::cout << "Do the conversion..." << std::endl;
        P1::ImageSdk::BitmapImage rgb_bitmap = rgb_iiq.Convert(config);
        P1::ImageSdk::BitmapImage nir_bitmap = nir_iiq.Convert(config);

        std::cout << "Do the stacking..." << std::endl;
        P1::ImageSdk::StackingOutput output;
        output.hasRgbiBitmap = true;

        P1::ImageSdk::Stacking stacking;
        stacking.DoStacking(rgb_bitmap, rgb_cal, nir_bitmap, nir_cal, output);

        std::cout << "Write image to tiff file..." << std::endl;
        P1::ImageSdk::TiffConfig tiff;
        P1::ImageSdk::TiffWriter(output_file, output.RgbiBitmap, rgb_iiq, tiff);

        std::cout << "Done!" << std::endl;
        return 0;
    }
    catch (P1::ImageSdk::SdkException &exception) {
        // Exception from ImageSDK
        std::cout << "Exception: " << exception.what() << " Code:" << exception.mCode
                  << std::endl;
        return 1;
    }
    catch (...) {
        // Any other exception - just in case
        std::cout << "Argh - we got an exception" << std::endl;
        return 1;
    }
}
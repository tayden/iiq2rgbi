#include <iostream>
#include <string>

#include <P1Image.hpp>
#include <P1ImageStacking.hpp>
#include <P1ImageBitmapImage.hpp>

int main(int argc, const char **argv) {
    // TODO: These values need to be dynamic
    std::string rgb_file = R"(..\..\testfiles\2023-03-18362_RGB_233769.IIQ)";
    std::string nir_file = R"(..\..\testfiles\2023-03-18362_NIR_298107.IIQ)";
    std::string output_file = R"(..\..\testfiles\output.tiff)";

    try {
        // Open an IIQ-file (edit the name and path to an IIQ-file)
        std::cout << "Open IIQ files" << std::endl;
        P1::ImageSdk::RawImage rgb_iiq(rgb_file);
        P1::ImageSdk::RawImage nir_iiq(nir_file);

        // Set up a convert config with the description about how to convert image into Rgb domain
        float scale = 1.0;

        P1::ImageSdk::GeometricCorrection rgb_cal{.pixelSize = 0.0046, .focalLength = 51.8176, .xp = -0.0186, .yp = -0.1029, .k1 = 0.0000156691, .k2 = -5.15017e-9, .k3 = 7.67762e-13, .p1 = 5.69591e-7, .p2 = -0.00000166097, .b1 = -9.65496e-8, .b2 = -6.61227e-9,};
        P1::ImageSdk::ConvertConfig rgb_config;
        rgb_config.SetGeometricCorrection(rgb_cal);
        rgb_config.SetGeomerticCorrectionEnabled(true);
        // rgb_config.SetOutputFormat(P1::ImageSdk::BitmapFormat::Rgb24);
        rgb_config.SetOutputScale(scale);

        P1::ImageSdk::GeometricCorrection nir_cal{.pixelSize = 0.0046, .focalLength = 51.8057, .xp = -0.0579, .yp = 0.0524, .k1 = 0.000014827, .k2 = -3.64981e-9, .k3 = -8.11134e-14, .p1 = -1.36228e-7, .p2 = -0.00000214019, .b1 = -0.00000755085, .b2 = 0.0000399366,};
        P1::ImageSdk::ConvertConfig nir_config;
        nir_config.SetGeometricCorrection(nir_cal);
        nir_config.SetGeomerticCorrectionEnabled(true);
        // nir_config.SetOutputFormat(P1::ImageSdk::BitmapFormat::Grey8);
        nir_config.SetOutputScale(scale);

        std::cout << "Do the conversion..." << std::endl;
        P1::ImageSdk::BitmapImage rgb_bitmap = rgb_iiq.Convert(rgb_config);
        P1::ImageSdk::BitmapImage nir_bitmap = nir_iiq.Convert(nir_config);

        std::cout << "Do the stacking..." << std::endl;
        P1::ImageSdk::StackingOutput output;
        output.hasRgbiBitmap = true;

        P1::ImageSdk::Stacking stacking;
        P1::ImageSdk::GeometricCorrection rgb_cal2{.pixelSize = 0.0046, .focalLength = 51.8176};
        P1::ImageSdk::GeometricCorrection nir_cal2{.pixelSize = 0.0046, .focalLength = 51.8057};
        stacking.DoStacking(rgb_bitmap, rgb_cal2, nir_bitmap, nir_cal2, output);

        std::cout << "Write image to tiff file..." << std::endl;
        P1::ImageSdk::TiffConfig tiff;
        P1::ImageSdk::TiffWriter(output_file, output.RgbiBitmap, rgb_iiq, tiff);

        std::cout << "Done!" << std::endl;
        return 0;
    } catch (P1::ImageSdk::SdkException &exception) {
        // Exception from ImageSDK
        std::cout << "Exception: " << exception.what() << " Code:" << exception.mCode << std::endl;
        return 1;
    } catch (...) {
        // Any other exception - just in case
        std::cout << "Argh - we got an exception" << std::endl;
        return 1;
    }
}
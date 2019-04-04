/*
Author: Brejvinder
LSB Steganography
Usage:
	./main.exe encode -i <input> -o <output> -f <file>
	./main.exe decode -i <input> -o <output>

Example:
	./main.exe encode -i image.png -o imageOut.png -f message.txt
	./main.exe decode -i imageOut.png -o output.txt
	It is important to state the format along with the name
*/
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

std::ostream& output_usage(std::ostream& os, int /*argc*/, char *argv[]) {
	os << "Usage:\n"
	   << "\t" << argv[0] << " encode -i <inputImage> -o <outputImageName> -f <file>\n"
	   << "\t" << argv[0] << " decode -i <inputImage> -o <outputFileName>\n";
	return os;
}

bool isBitSet(char ch, int pos) {
	ch = ch >> pos;
	return (ch & 1);
}

int main(int argc, char *argv[]) {
	using namespace std;
	using namespace cv;

	if ( !(argc == 6 || argc == 8 )) {
		output_usage(cerr, argc, argv)
		        << "\tNOTE: Invalid argument(s) provided\n";
		return 1;
	}

	Mat image = imread(argv[3]);
	if (image.empty()) {
		cerr << "Image Error\n";
		return 1;
	}

	char ch;
	auto bit_count = 0;

	string keyword = argv[1];
	if (keyword == "encode") {
		// Encode file INTO image and write it to the output
		auto last_null_char = false, encoded = false;

		ifstream file(argv[7]);
		if (!file.is_open()) {
			cerr << "File couldn't be opened.\n";
			return 1;
		}
		file.get(ch);

		for (auto row = 0; row < image.rows && !encoded; ++row) {
			for (auto col = 0; col < image.cols && !encoded; ++col) {
				for (auto color = 0; color < 3 && !encoded; ++color) {

					Vec3b pixel = image.at<Vec3b>(Point(row, col));

					if (isBitSet(ch, 7 - bit_count))
						(pixel.val[color] |= 1);
					else
						(pixel.val[color] &= ~1);

					image.at<Vec3b>(Point(row, col)) = pixel;
					++bit_count;

					if (last_null_char && bit_count == 8) {
						encoded  = true;
						break;
					}

					if (bit_count == 8) {
						bit_count = 0;
						file.get(ch);
						if (file.eof()) {
							last_null_char = true;
							ch = '\0';
						}
					}

				}
			}
		}

		imwrite(argv[5], image);

	} else if (keyword == "decode") {
		// Decode file FROM image and write it to the output
		ch = 0;
		auto decoded = false;

		ofstream file(argv[5]);
		if (!file.is_open()) {
			cerr << "File Error\n";
			return 1;
		}

		for (auto row = 0; row < image.rows && !decoded; ++row) {
			for (auto col = 0; col < image.cols && !decoded; ++col) {
				for (auto color = 0; color < 3 && !decoded; ++color) {

					Vec3b pixel = image.at<Vec3b>(Point(row, col));

					if (isBitSet(pixel.val[color], 0))
						ch |= 1;

					++bit_count;
					if (bit_count == 8) {
						if (ch == '\0') {
							decoded = true;
							break;
						}

						bit_count = 0;
						file << ch;
						// cout << ch;
						ch = 0;
					} else {
						ch = ch << 1;
					}

				}
			}
		}
	} else {
		output_usage(cerr, argc, argv)
		        << "\tNOTE: Invalid keyword provided\n";
		return 1;
	}

	return 0;
}


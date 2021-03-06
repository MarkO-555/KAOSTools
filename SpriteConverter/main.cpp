//	Sprite Conversion Utility for KAOS on the Color Computer III
//	------------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <FlicLib/flic.h>
#include <KAOS/Common/Utilities.h>
#include <KAOS/Common/Logging.h>
#include <loadpng/lodepng.h>
#include <deque>
#include <string>
#include <optional>
#include <iostream>
#include <vector>
#include <filesystem>
#include <io.h>	//	FIXME: Dropt his and use std::filesystem::exists instead
#include <fstream>
#include <sstream>
#include <iomanip>


bool ConvertFlic(const std::string& filepath, const std::string& baseOutputDirectory, bool useNameAsSubdirectory)
{
	std::ifstream input(filepath, std::ios::binary);
	if (!input)
	{
		std::cerr << "Unable to open `" << filepath << "`\n";
		return false;
	}

	flic::StreamInputInterface flicInput(input);
	flic::Decoder decoder(&flicInput);

	flic::Header header;
	if (!decoder.readHeader(header))
	{
		std::cerr << "Unable to read header from `" << filepath << "`\n";
		return false;
	}

	const auto baseFilename(KAOS::Common::GetFilenameFromPath(filepath, false));
	std::vector<std::pair<std::string, std::string>> generatedFramePaths;
	flic::Frame frame(header.width, header.height, header.width);
	for (auto i(0); i < header.frames; ++i)
	{
		if (!decoder.readFrame(frame))
		{
			std::cerr << "Unable to read frame #" << i << " from `" << filepath << "`\n";
			return false;
		}


		std::vector<unsigned char> imageData(header.width * header.height * 4);
		const unsigned char* srcPtr(frame.pixels);
		unsigned char* dstPtr(&imageData[0]);
		for (auto y(0); y < header.height; ++y)
		{
			for (auto x(0); x < header.width; ++x)
			{
				const auto& color(frame.colormap[*srcPtr]);
				*(dstPtr++) = color.r;
				*(dstPtr++) = color.g;
				*(dstPtr++) = color.b;
				*(dstPtr++) = 255;

				++srcPtr;
			}

			srcPtr += (frame.rowstride - frame.width);
		}


		auto outputDirectory(baseOutputDirectory);
		if (useNameAsSubdirectory)
		{
			outputDirectory += "/" + baseFilename;
		}

		std::ostringstream outputFilepath;
		outputFilepath << outputDirectory << "/" << baseFilename << "-" << std::setfill('0') << std::setw(4) << i << ".png";

		std::vector<unsigned char> buffer;
		lodepng::State state;
		lodepng_state_init(&state);

		state.info_raw.colortype = LCT_RGBA;
		state.info_raw.bitdepth = 8;
		state.info_png.color.colortype = LCT_RGBA;
		state.info_png.color.bitdepth = 8;

		//	FIXME: Check for errors
		lodepng::encode(buffer, imageData, frame.rowstride, frame.height, state);
		lodepng_state_cleanup(&state);

		KAOS::Common::CreateDirectory(outputDirectory);
		lodepng_save_file(buffer.data(), buffer.size(), outputFilepath.str().c_str());

		const auto targetPath(KAOS::Common::GetRelativePathFromFilePath(baseOutputDirectory, true, outputFilepath.str(), false));
		if (!targetPath.has_value())
		{
			std::cerr
				<< "Unable to get relative path for `"
				<< outputFilepath.str()
				<< "` from `"
				<< baseOutputDirectory << "`\n";
			return false;
		}

		generatedFramePaths.emplace_back(baseFilename, KAOS::Common::ConvertToForwardSlashes(*targetPath));
	}



	std::string frameLine;
	for (const auto& path : generatedFramePaths)
	{
		if (!frameLine.empty())
		{
			frameLine += ",\n";
		}
		frameLine += "\t\t\t\"" + path.second + '"';
	}


	const auto outputFilepath(baseOutputDirectory + "/" + baseFilename + ".json");
	std::ofstream outputFile(outputFilepath);
	if (!outputFile.is_open())
	{
		std::cerr << "Unable to open `" << outputFilepath << "`\n";
		return false;
	}

	outputFile
		<< "{\n"
		<< "\t\"sprite\": {\n"
		<< "\t\t\"name\": \"" << baseFilename << "\",\n"
		<< "\t\t\"frameduration\": " << header.speed << ",\n"
		<< "\t\t\"frameimages\": [\n" << frameLine << "\n"
		<< "\t\t]\n"
		<< "\t}\n"
		<< "}\n";


	return false;
}

int main(int argc, const char **argv)
{
	std::deque<std::string> args(argv + 1, argv + argc);
	std::optional<std::string> outputDirectory;
	std::vector<std::string> inputFiles;
	bool hasError(false);


	while (!args.empty())
	{
		const auto originalArg(args.front());
		args.pop_front();
		if (originalArg.empty())
		{
			continue;
		}

		if (originalArg[0] == '-')
		{
			auto arg(originalArg);

			arg.erase(arg.begin());
			if (arg.empty() || arg[0] != '-')
			{
				std::cerr << "Unknown option `" << arg << "`\n";
				return EXIT_FAILURE;
			}
			arg.erase(arg.begin());


			const auto argOffset(arg.find('='));
			std::string value;

			if (argOffset != arg.npos)
			{
				value = arg.substr(argOffset + 1);
				arg.resize(argOffset);
			}


			if (arg == "output")
			{
				if (outputDirectory.has_value())
				{
					KAOS::Logging::Warn("Output directory file already set to `" + *outputDirectory + "`");
				}
				else
				{
					outputDirectory = value;
				}
			}
			else if (arg == "objectlist")
			{
			}
			else if (arg == "emptyid")
			{
			}
			else
			{
				std::cerr << "Unknown argument `" << originalArg << "`\n";
				return EXIT_FAILURE;
			}
		}

		else
		{
			inputFiles.push_back(originalArg);
		}
	}


	//	Errors
	if (inputFiles.empty())
	{
		std::cerr << "No input files specified\n";
		hasError = true;
	}

	if (!outputDirectory.has_value())
	{
		std::cerr << "No output directory specified.\n";
		hasError = true;
	}

	if (outputDirectory->empty())
	{
		std::cerr << "Output directory cannot be empty.\n";
		hasError = true;
	}

	for (const auto& filename : inputFiles)
	{
		//	FIXME: This should use std::filesystem::exists bit it's still stuck
		//	in std::experimental in VS2017.
		if(_access(filename.c_str(), 0) == -1)
		{
			std::cerr << "Unable to access `" << filename << "`.\n";
			hasError = false;
		}
	}

	if (hasError)
	{
		return EXIT_FAILURE;
	}


	for (const auto& filename : inputFiles)
	{
		//	FIXME: This should use std::filesystem::exists bit it's still stuck
		//	in std::experimental in VS2017.
		if(!ConvertFlic(filename, *outputDirectory, inputFiles.size() > 1))
		{
			hasError = false;
		}
	}

	if (hasError)
	{
		return EXIT_FAILURE;
	}


	//	DONE
}




//	Copyright (c) 2018 Chet Simpson
//	
//	Permission is hereby granted, free of charge, to any person
//	obtaining a copy of this software and associated documentation
//	files (the "Software"), to deal in the Software without
//	restriction, including without limitation the rights to use,
//	copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following
//	conditions:
//	
//	The above copyright notice and this permission notice shall be
//	included in all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//	OTHER DEALINGS IN THE SOFTWARE.

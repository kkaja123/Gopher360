#include "ConfigFile.h"
#include <iostream>
#include <fstream> 
#include <windows.h>

void ConfigFile::removeComment(std::string &line) const
{
	if (line.find('#') != line.npos)
		line.erase(line.find('#'));
}

bool ConfigFile::onlyWhitespace(const std::string &line) const
{
	return (line.find_first_not_of(' ') == line.npos);
}
bool ConfigFile::validLine(const std::string &line) const
{
	std::string temp = line;
	temp.erase(0, temp.find_first_not_of("\t "));
	if (temp[0] == '=')
		return false;

	for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
		if (temp[i] != ' ')
			return true;

	return false;
}

void ConfigFile::extractKey(std::string &key, size_t const &sepPos, const std::string &line) const
{
	key = line.substr(0, sepPos);
	if (key.find('\t') != line.npos || key.find(' ') != line.npos)
		key.erase(key.find_first_of("\t "));
}
void ConfigFile::extractValue(std::string &value, size_t const &sepPos, const std::string &line) const
{
	value = line.substr(sepPos + 1);
	value.erase(0, value.find_first_not_of("\t "));
	value.erase(value.find_last_not_of("\t ") + 1);
}

void ConfigFile::extractContents(const std::string &line)
{
	std::string temp = line;
	temp.erase(0, temp.find_first_not_of("\t "));
	size_t sepPos = temp.find('=');

	std::string key, value;
	extractKey(key, sepPos, temp);
	extractValue(value, sepPos, temp);

	if (!keyExists(key))
		contents.insert(std::pair<std::string, std::string>(key, value));
	else
		exitWithError("CFG: Can only have unique key names!\n");
}

void ConfigFile::parseLine(const std::string &line, size_t const lineNo)
{
	if (line.find('=') == line.npos)
		exitWithError("CFG: Couldn't find separator on line: " + Convert::T_to_string(lineNo) + "\n");

	if (!validLine(line))
		exitWithError("CFG: Bad format for line: " + Convert::T_to_string(lineNo) + "\n");

	extractContents(line);
}

void ConfigFile::ExtractKeys()
{
	std::ifstream file;
	file.open(fName.c_str());


	if (!file)
	{
		
		printf("%s not found! Building a fresh one... ", fName.c_str());

		std::ofstream outfile("config.ini");

		//begin cfg dump
		outfile << "#	GOPHER DEFAULT CONFIGURATION rev1.0 - Auto generated by Gopher360." << std::endl;
		outfile << "#	If you want a fresh one, just DELETE THIS and re-run Gopher360." << std::endl;
		outfile << "#	Set which controller buttons will activate the configuration events." << std::endl;
		outfile << "#	SET 0 FOR NO FUNCTION." << std::endl;
		outfile << "#	AVAILABLE VALUES AT https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad(v=vs.85).aspx" << std::endl;
		outfile << "#	TIP: Sum the hex value for double button shortcuts eg. 0x0010(START) 0x0020(BACK) so 0x0030(START+BACK) will trigger the event only when both are pressed." << std::endl;
		outfile << "\n" << std::endl;
		outfile << "CONFIG_MOUSE_LEFT = 0x1000	# Left mouse button" << std::endl;
		outfile << "CONFIG_MOUSE_RIGHT = 0x4000	# Right mouse button" << std::endl;
		outfile << "CONFIG_MOUSE_MIDDLE = 0x0040	# Middle mouse button" << std::endl;
		outfile << "CONFIG_HIDE = 0x8000		# Hides the terminal" << std::endl;
		outfile << "CONFIG_DISABLE = 0x0030		# Disables the Gopher" << std::endl;
		outfile << "CONFIG_DISABLE_VIBRATION = 0x0011 # Disables Gopher Vibrations" << std::endl;
		outfile << "CONFIG_SPEED_CHANGE =  0x0300	# Change speed" << std::endl;
		outfile << "#CONFIG_OSK = 0x0020   # Toggle on-screen keyboard" << std::endl;
		outfile << "\n" << std::endl;
		outfile << "#	KEYBOARD SHORTCUTS ON CONTROLLER BUTTONS" << std::endl;
		outfile << "#	SET 0 FOR NO FUNCTION" << std::endl;
		outfile << "#	AVAILABLE VALUES AT> https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731" << std::endl;
		outfile << "\n" << std::endl;
		outfile << "GAMEPAD_DPAD_UP = 0x26" << std::endl;
		outfile << "GAMEPAD_DPAD_DOWN = 0x28" << std::endl;
		outfile << "GAMEPAD_DPAD_LEFT = 0x25" << std::endl;
		outfile << "GAMEPAD_DPAD_RIGHT = 0x27" << std::endl;
		outfile << "GAMEPAD_START = 0x5B" << std::endl;
		outfile << "GAMEPAD_BACK = 0xA8" << std::endl;
		outfile << "GAMEPAD_LEFT_THUMB = 0" << std::endl;
		outfile << "GAMEPAD_RIGHT_THUMB = 0x71" << std::endl;
		outfile << "GAMEPAD_LEFT_SHOULDER = 0" << std::endl;
		outfile << "GAMEPAD_RIGHT_SHOULDER = 0" << std::endl;
		outfile << "GAMEPAD_A = 0" << std::endl;
		outfile << "GAMEPAD_B = 0x0D" << std::endl;
		outfile << "GAMEPAD_X = 0" << std::endl;
		outfile << "GAMEPAD_Y = 0" << std::endl;
		outfile << "GAMEPAD_TRIGGER_LEFT = 0" << std::endl;
		outfile << "GAMEPAD_TRIGGER_RIGHT = 0" << std::endl;
		outfile << "\n" << std::endl;
		outfile << "#  ALLOWED CURSOR SPEEDS, FIRST WILL BE CHOSEN BY DEFAULT.  VALUES > 1.0 WILL BE IGNORED.  NO SPACES." << std::endl;
		outfile << "CURSOR_SPEED = ULTRALOW=0.005,LOW=0.015,MED=0.025,HIGH=0.04" << std::endl;
		outfile << "#  SET ACCELERATION FACTOR FOR NON-LINEAR CURSOR SPEED" << std::endl;
		outfile << "# ACCELERATION_FACTOR = 3" << std::endl;
		//end cfg dump

		outfile.close();
		
		file.open(fName.c_str());

		if (!file)
		{

			exitWithError("\nERROR! Configuration file " + fName + " still couldn't be found!\n");
		}
		else
		{

			printf("Success!\nNow using %s.\n", fName.c_str());
		}
	}

	//exitWithError("\nSafety exit!\n");
		

	std::string line;
	size_t lineNo = 0;
	while (std::getline(file, line))
	{
		lineNo++;
		std::string temp = line;

		if (temp.empty())
			continue;

		removeComment(temp);
		if (onlyWhitespace(temp))
			continue;

		parseLine(temp, lineNo);
	}

	file.close();
}

ConfigFile::ConfigFile(const std::string &fName)
{
	this->fName = fName;
	ExtractKeys();
}

bool ConfigFile::keyExists(const std::string &key) const
{
	return contents.find(key) != contents.end();
}

void ConfigFile::exitWithError(const std::string &error)
{
	std::cout << error;
	std::cin.ignore();
	std::cin.get();

	exit(EXIT_FAILURE);
}

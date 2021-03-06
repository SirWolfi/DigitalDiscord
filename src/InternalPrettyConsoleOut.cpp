#include "InternalPrettyConsoleOut.h"


void InternalPCO::waitForInput() {
	std::getchar();
}

std::string& InternalPCO::SlowPrintc::getID() {
	return id;
}

InternalPCO::SlowPrintc* InternalPCO::SlowPrintc::operator<<(const char* inp) {
	for (size_t i = 0; i < strlen(inp); ++i) {
		std::cout << inp[i];
		mac::tsleep(sleeptime);
	}
	SlowPrintClassPrintEvent::trigger(std::pair<std::string,SlowPrintc*>(inp,this));
	return this;
}

InternalPCO::SlowPrintc* InternalPCO::SlowPrintc::operator<<(std::string inp) {
	for (size_t i = 0; i < inp.length(); ++i) {
		std::cout << inp[i];
		mac::tsleep(sleeptime);
	}
	SlowPrintClassPrintEvent::trigger(std::pair<std::string,SlowPrintc*>(inp,this));
	return this;
}

InternalPCO::SlowPrintc* InternalPCO::SlowPrintc::operator<<(int inp) {
	for (long i = 0; i < inp; ++i) {
		std::cout << (std::to_string(inp))[i];
		mac::tsleep(sleeptime);
	}
	SlowPrintClassPrintEvent::trigger(std::pair<std::string,SlowPrintc*>(std::to_string(inp),this));
	return this;
}

InternalPCO::SlowPrintc* InternalPCO::SlowPrintc::operator<<(char inp) {
	std::cout << inp;
	mac::tsleep(sleeptime);
	SlowPrintClassPrintEvent::trigger(std::pair<std::string,SlowPrintc*>(std::to_string(inp),this));
	return this;
}

InternalPCO::SlowPrintc* InternalPCO::SlowPrintc::operator<<(long long inp) {
	for (long i = 0; i < inp; ++i) {
		std::cout << std::to_string(inp)[i];
		mac::tsleep(sleeptime);
	}
	SlowPrintClassPrintEvent::trigger(std::pair<std::string,SlowPrintc*>(std::to_string(inp),this));
	return this;
}

void InternalPCO::SlowPrintc::operator=(int time) {
	this->sleeptime = time;
}

void InternalPCO::SlowPrintc::operator++() {
	this->sleeptime += 1;
}

void InternalPCO::SlowPrintc::operator--() {
	this->sleeptime -= 1;
}

void InternalPCO::SlowPrintc::setSleeptime(int newsleeptime) {
	sleeptime = newsleeptime;
}


InternalPCO::LoadingScreen::LoadingScreen(long length, int clearWaitTime, bool clearS, char logo) {
	this->length = length;
	this->clearS = clearS;
	this->clearWaitTime = clearWaitTime;
	this->logo = logo;
}

bool InternalPCO::LoadingScreen::next(std::string message) {
	LoadingScreenUpdateEvent::trigger(this);
	tmp_len += 1;
	std::cout << "\r";
	for(size_t i = 0; i < length + 40; ++i) {
		std::cout << " "; //clear line
	}
	std::cout << "\r[";
	for (long i = 0; i < length; ++i) {
		if (i > tmp_len) {
			std::cout << " ";
		}
		else {
			std::cout << logo;
		}
	}
	std::cout << "] " << message;

	if (tmp_len == length) {
		length = 0;
		tmp_len = 0;

		if (clearS) {
			mac::sleep(clearWaitTime);
			mac::clearScreen();
		}
		return true;
	}

	return false;
}

void InternalPCO::LoadingScreen::clear(bool cls) {
	if(cls) { system("cls"); }
	tmp_len = 0;
}


InternalPCO::VisualCounter::VisualCounter(long hight) {
	this->hight = hight;
}

bool InternalPCO::VisualCounter::next(std::string message) {
	tmp_len += 1;
	std::cout << "\r" << tmp_len << "/" << hight << "  " << message;

	if (tmp_len >= hight) {
		std::cout << "\n";
		hight = 0;
		tmp_len = 0;
		return true;
	}
	return false;
}

void InternalPCO::VisualCounter::clear() {
	this->tmp_len = 0;
}


InternalPCO::VisualTimer::VisualTimer(long time, float delay) { //delay in seconds
	this->time = time;
	this->delay = 100 * delay;
}

void InternalPCO::VisualTimer::operator=(VisualTimer& timer) {
	this->delay = timer.delay;
	this->time = timer.time;
	this->tmp_long = 0;
}

void InternalPCO::VisualTimer::operator+=(VisualTimer& timer) {
	this->delay += timer.delay;
	this->time += timer.time;
	this->tmp_long = 0;
}

void InternalPCO::VisualTimer::operator++() {
	time += 1;
}

void InternalPCO::VisualTimer::operator--() {
	time -= 1;
}

void InternalPCO::VisualTimer::start() {
	tmp_long = 0;
	for (; tmp_long < time + 1; ++tmp_long) {
		std::cout << "\rT - " << (time - tmp_long) << " ";
		mac::sleep(delay);
			}
	std::cout << "\n";
}

bool InternalPCO::VisualTimer::is_end() {
	return tmp_long == time ? true : false;
}

void InternalPCO::VisualTimer::reset() {
	tmp_long = 0;
}


std::string InternalPCO::corruptedLine(long length) {
	std::string ret;
	for (long i = 0; i < length; ++i) {
		srand(time(NULL) + i);
		char ch = (static_cast<char>(rand() - i / 5));
		ret += ch != '\n' ? ch : (static_cast<char>(rand() - i / 5));
	}
	return ret;
}

void InternalPCO::errorMessage(std::string message, int stime) {
	mac::clearScreen();
	std::cout << "Ouh, there is an error ;-;\n\n";
	std::cout << " ----" << message << "\n";

	mac::sleep(stime);
	mac::clearScreen();
}


InternalPCO::HubChoice::HubChoice(void (*func)(),std::string name, std::string id) {
	this->id = id;
	this->name = name;
	this->func = func;
}

void InternalPCO::HubChoice::run() {
	func();
	HubChoiceChooseEvent::trigger(this);
}


InternalPCO::Hub::Hub(std::string name, std::vector<HubChoice> choices, std::string customLayout /*= "STD" */) {
	this->name = name;
	layout = customLayout;
	this->choices = choices;
}

void InternalPCO::Hub::show() {
	hiding = false;
	mac::clearScreen();
	HubOpenEvent::trigger(this);

	if(layout == "STD") {
		std::cout << "-------------# Welcome to " << name << " #-------------\n";		  
	}
	else { //not STD
		std::cout <<
			IL::Parser()
				.setIdents('{','}')
				->addPlaceHolder("name",this->name)
				->addPlaceHolder("time",std::to_string(time(NULL)))
				->addPlaceHolder("Ccount",std::to_string(choices.size()))
				->addPlaceHolder("nl","\n")
				->addPlaceHolder("{","{")
				->addPlaceHolder("}","}")
			->parse(layout);
	}

	for(size_t i = 0; i < choices.size(); ++i) {
		std::cout << "       " << i+1 << ".)  " << choices[i].name << "\n";
	}

	std::cout << "\n\n"
			  << "Your choice:";
	std::string inp;
	std::getline(std::cin,inp);
	mac::sleep(100);
	mac::clearScreen();

	try {
		DEBUG_MESSAGE("stoi: " << std::stoi(inp))
		if(std::stoi(std::string(1,inp[0])) > choices.size()) {
			ERROR_IF(true)
			errorMessage("Invaild Input in " + name + "-Hub");
			HubChoiceFailEvent::trigger(this);
			show();
		}
		choices[std::stoi(inp)-1].run();
	}
	catch(...) {
		errorMessage("Invaild Input in " + name + "-Hub");
		HubChoiceFailEvent::trigger(this);
		show();
	}
				
	hiding = true;
}

void InternalPCO::Hub::save() {
	std::string layout =
	"name = <name>					"
	"Ccount = <Ccount>				"
	"layout_NO_REP = \"<layout>\"	";

	layout = InternalLib::Parser()
		.addPlaceHolder("name",this->name)
		->addPlaceHolder("Ccount",std::to_string(this->choices.size()))
		->addPlaceHolder("layout",this->layout)
		->setIdents('<','>')
		->parse(layout);
	
	layout += "\n";
}

InternalPCO::Hub InternalPCO::Hub::call(fs::path path) {

}
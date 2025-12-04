#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <exception>
#include <stdio.h>
#include <concepts>
#include <queue>
#include <variant>
#include <conio.h>
#include <filesystem>
#include <functional>
#include <cctype> 

#define SIZE_BUFF 64
#define OpenFileERROR "There's the OPEN_FILE ERROR..."
#define EmptyERROR "There's the EMPTY ERROR..."

using namespace std;

namespace smart_system
{
	template<typename T>
	class FileSystem; //{}
	class SmartSmth;
	class User;
	class Thermostat;
	class SmartLight;
	class SecurityCamera;
	class SmartHomeInteraction;
	class Date;
	enum class SmartType;
	class UserLocation;

	using DeviceVariant = std::variant<SmartLight, Thermostat, SecurityCamera>;

	template<typename T>
	concept smartDeviceType =
		same_as<T, SecurityCamera> ||
		same_as<T, Thermostat> ||
		same_as<T, SmartLight>;

	enum class SmartType{ Light, Thermo, SecCamera};
	enum class UserRole { Guest_, User_, Admin_ };
	using enum SmartType;
	using enum UserRole;

	class Date final //+~
	{
		unsigned short year_;
		short month_;
		short day_;
		friend ostream& operator<<(ostream& out, const Date& date);
		friend istream& operator>>(istream& in, Date& date);
		friend bool operator==(const Date& lhs, const Date& rhs);
		friend bool operator<(const Date& lhs, const Date& rhs);
	public:
		Date(); 
		Date(const unsigned short year, const short month, const short day);//+~
		bool isLeapYear(const unsigned short year) const;//+
		bool validDate(const unsigned short year, const short month, const short day) const;//+

		void setYear(const unsigned short year);//+ 
		void setMonth(const short month);//+
		void setDay(const short day); //+
		void setDate(const short year, const short month, const short day); //+

		unsigned short getYear()const;//+
		short getMonth()const;//+
		short getDay()const;//+

		~Date() = default;
	};

	class SmartSmth abstract
	{
	protected:
		char owner_password[SIZE_BUFF];
		char title[SIZE_BUFF];
		SmartType anyType;
		bool isOnline;
		Date release_date;
		friend ostream& operator<<(ostream& out, const SmartSmth& device);
		friend istream& operator>>(istream& in, SmartSmth& device);
		friend bool operator==(const SmartSmth& lhs, const SmartSmth& rhs);
	public:
		bool getOnline() const;
		void setTitle(const string& title);
		void setOnline(const bool online);
		void setPurchaseDate(const Date& purchase_date);
		Date getPurchaseDate() const;
		SmartType getType() const;
		string getTitle() const;
		string getPassword() const;
		void setPassword(const string& password);
		SmartSmth(const string& owner_password, const string& title, const SmartType& anyType, const bool online, const Date& purchase_date);
		SmartSmth();
		~SmartSmth() = default;
	};

	class UserLocation final //+
	{
		char country[SIZE_BUFF];
		char city[SIZE_BUFF];
		char street[SIZE_BUFF];
		friend ostream& operator<<(ostream& out, const UserLocation& location);
		friend istream& operator>>(istream& in, UserLocation& location);
		friend bool operator==(const UserLocation& lhs, const UserLocation& rhs);
	public:
		UserLocation();
		UserLocation(const string& country, const string& city, const string& street);//+
		void setCountry(const string& country);//+
		void setCity(const string& city);//+
		void setStreet(const string& street);//+
		string getCountry() const;//+
		string getCity() const;//+
		string getStreet() const; //+
	};

	class User
	{
		char hashed_password[SIZE_BUFF];
		char user_name[SIZE_BUFF];
		UserLocation location_;
		Date date_;
		UserRole anyRole;

		friend ostream& operator<<(ostream& out, const User& user);
		friend istream& operator>>(istream& in, User& user);
		friend bool operator==(const User& lhs, const User& rhs);
	public:
		User(const string& username, const string& password, const UserLocation& location, const Date& date, UserRole role);
		friend string hashPassword(const string& user_name,const string& password);
		void setRole(const UserRole& anyRole);
		void setUserDate(const Date& date);
		void setUserLocation(const UserLocation& location);
		void setPassword(const string& password);
		void setUserName(const string& user_name);

		string getUserName() const;
		string getPassword() const;
		UserRole getRole() const;
		UserLocation getLocation() const;
		Date getUserDate() const;
	};

	class SmartLight : public SmartSmth
	{
		int brightness_;
		char color[SIZE_BUFF];
		friend ostream& operator<<(ostream& out, const SmartLight& light);
		friend istream& operator>>(istream& in, SmartLight& light);
		friend bool operator==(const SmartLight& lhs, const SmartLight& rhs);
	public:
		SmartLight(const string& owner_password, const string& title, const bool online, const Date& purchase_date, const int brightness, const string& color);
		int getBrightness() const;
		string getColor() const;
		void setBrightness(const int brightness);
		void setColor(const string& color);
	};

	class Thermostat : public SmartSmth
	{
		double currentTemperature;
		double targetTemperature;
		char mode[SIZE_BUFF]; // heating, cooling, auto
		friend ostream& operator<<(ostream& out, const Thermostat& thermostat);
		friend istream& operator>>(istream& in, Thermostat& thermostat);
		friend bool operator==(const Thermostat& lhs, const Thermostat& rhs);
	public:
		Thermostat(const string& owner_password, const string& title, const bool online, const Date& purchase_date, const double currentTemperature, const double targetTemperature, const string& mode);
		string getMode() const;
		double getCurTemperature() const;
		double getTargetTemperature() const;
		void setTargetTemperature(double temp);
		void setCurTemperature(double temp);
		void setMode(const string& newMode);
	};

	class SecurityCamera : public SmartSmth
	{
		bool isRecording;
		int resolution;
		bool motionDetectionEnabled;
		friend ostream& operator<<(ostream& out, const SecurityCamera& camera);
		friend istream& operator>>(istream& in, SecurityCamera& camera);
		friend bool operator==(const SecurityCamera& lhs, const SecurityCamera& rhs);
	public:
		SecurityCamera(const string& owner_password, const string& title, const bool online, const Date& purchase_date, const int resolution, const bool recording, const bool motionDetectionEnabled); // в реализации

		bool getRecording() const;
		int getResolution() const;
		bool getMotion() const;

		//void setResolution(const int resolution);
	};

	template<smartDeviceType T>
	class DeviceScript;

	using DeviceScriptVariant = std::variant<
		DeviceScript<SmartLight>,
		DeviceScript<Thermostat>,
		DeviceScript<SecurityCamera>
	>;

	template<smartDeviceType T>
	class DeviceScript
	{
		T smart_device;
		short duration_ = 0;
		char script[SIZE_BUFF];
		friend ostream& operator<<(ostream& out, const DeviceScript<T>& deviceScript) {
			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {

				//SmartType type = deviceScript.smart_device.getType();
				//out.write(reinterpret_cast<const char*>(&type), sizeof(type));


				out << deviceScript.smart_device;

				out.write(reinterpret_cast<const char*>(&deviceScript.duration_), sizeof(deviceScript.duration_));

				size_t script_size = strlen(deviceScript.script);
				out.write(reinterpret_cast<const char*>(&script_size), sizeof(script_size));
				out.write(deviceScript.script, script_size);
			}
			else {
				out << "Device Script: " << deviceScript.script << "\n";
				out << "Duration: " << deviceScript.duration_ << " minutes\n";
				out << "Device: " << deviceScript.smart_device << "\n";
			}
			return out;
		}
		friend istream& operator>>(istream& in, DeviceScript<T>& deviceScript) {
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {
				// Файловый ввод - бинарный формат
				// Читаем тип устройства (но он должен соответствовать T)
				//SmartType type;
				//in.read(reinterpret_cast<char*>(&type), sizeof(type));

				// Читаем устройство
				in >> deviceScript.smart_device;

				// Читаем длительность
				in.read(reinterpret_cast<char*>(&deviceScript.duration_), sizeof(deviceScript.duration_));

				// Читаем скрипт
				size_t script_size;
				in.read(reinterpret_cast<char*>(&script_size), sizeof(script_size));
				in.read(deviceScript.script, script_size);
				deviceScript.script[script_size] = '\0';
			}
			else {
				// Консольный ввод - текстовый формат
				cout << "Enter script description: ";
				in.ignore();
				in.getline(deviceScript.script, SIZE_BUFF);

				cout << "Enter duration (minutes): ";
				in >> deviceScript.duration_;

				cout << "Enter device details:\n";
				in >> deviceScript.smart_device;
			}
			return in;
		}
		friend ostream& operator<<(ostream& out, const DeviceScriptVariant& variant);
		friend istream& operator>>(istream& in, DeviceScriptVariant& variant);
	public:

		DeviceScript();
		DeviceScript(const T& smart_device,const string& script, const short duration);
		T getDevice() const;
		short getDuration() const;
		std::string getScript() const;
		void setScript(const string& script);
		void setDuration(const short duration);
	};
	


	template<>
	class FileSystem<DeviceVariant>
	{
		fstream smartFile;
		string file_name;
	public:

		bool checkDevice(const DeviceVariant& device);
		int readF();
		DeviceVariant chooseCertainDevice(const int count);
		void writeF(const DeviceVariant& device);
		/*void editF(const DeviceVariant& device);*/
		DeviceVariant editF(int pos,const DeviceVariant& new_device);
		void searchF(const string& dev_name);
		void filter(std::function<bool(const DeviceVariant&)> predicate, const std::string& filter_name);
		DeviceVariant removeF(int pos);
		bool sortF(std::function<bool(const DeviceVariant&, const DeviceVariant&)> comp);
		FileSystem();
		void setFileName(const string& file_name);
		~FileSystem() = default;
		string getFileName() const;

	};

	template<>
	class FileSystem<User>
	{
		fstream smartFile;
		string file_name;
	/*	friend void operator<<(ostream& out, FileSystem&);*/
	public:
		User chooseUser(const string& name);
		short checkUser(User& user_to_check);
		void readF(const User& user);
		void readF(UserRole role, const User& user);
		void writeF(const User& user);
		//void removeF(const User& user);
		void removeF(const User& user);
		FileSystem();
		
		~FileSystem() = default;
		string getFileName() const;
	};

	template<>
	class FileSystem<DeviceScriptVariant>
	{
		fstream smartFile;
		string file_name;
		//friend void operator<<(ostream& out, FileSystem&);
	public:
		//void readF(const User& user);
		//void readF(UserRole role, const User& user);
		std::queue<DeviceScriptVariant> unloadScripts(const User& user);
		void writeF(queue<DeviceScriptVariant>& queue);
		void removeF(const User& user);
		FileSystem();
		~FileSystem() = default;
		string getFileName() const;
		//void searchF(const DeviceScriptVariant& dev_script);

	};

	static class SmartHomeInteraction final
	{
		/*		void setFormat(); */// setf флаги формат.
		static void removeScript(const DeviceVariant& device);
		static bool showAccountMenu();
		static bool showAdminOperationsMenu();
		static shared_ptr<User> current_user;
		static std::queue<DeviceScriptVariant> script_subsequence;
		static DeviceVariant chooseDevice();
		static void printScripts();
		static void showUserMenu();
		static void showAdminMenu();
		static void showGuestMenu();
		static void showMainMenu();
		static void showAuthorMenu();
		static void showSmartHomeMenu();
		static void showDeviceCatalogHeaderMenu();
		static void showSHOHeaderMenu();
		static void showRoleHeaderMenu();
		static void showRegistrationMenu();
		static void showLoginMenu();
		static FileSystem<DeviceVariant> device_file;
		static FileSystem<DeviceScriptVariant> script_file;
		static FileSystem<User> user_file;
	public:
		~SmartHomeInteraction() = default;
		SmartHomeInteraction() = default; 
		static void showEntryMenu();
	};
}
	#include "prototype.h"
	#pragma warning(disable:4996)
	namespace smart_system
	{

		std::string getPasswordWithDots() {
			std::string password;
			char ch;

			std::cout << "Введите пароль: ";

			while ((ch = _getch()) != '\r') {
				if (ch == '\b') {
					if (!password.empty()) {
						password.pop_back();
						std::cout << "\b \b";
					}
				}
				else if (ch >= 32 && ch <= 126) {
					password += ch;
					std::cout << '•';
				}
			}
			std::cout << std::endl;
			return password;
		}

		string hashPassword(const string& user_name, const string& password)
		{
			return string(user_name) + password + "_" + std::to_string(password.size() * 12345);
			/*return "salted_" + password + "_" + std::to_string(password.size() * 12345);*/
		}

		//SmartSmth
		SmartSmth::SmartSmth() 
		{
			memset(title, '\0', SIZE_BUFF); 
			memset(owner_password, '\0', SIZE_BUFF);
			anyType = Light;
			isOnline = false;
			release_date = Date();
		}
		bool SmartSmth::getOnline() const { return isOnline; }
		string SmartSmth::getTitle() const { return string(title); }
		SmartType SmartSmth::getType() const { return anyType; }
		void SmartSmth::setOnline(const bool online) { isOnline = online; }
		void SmartSmth::setPurchaseDate(const Date& purchase_date)
		{
			this->release_date = purchase_date;
		}
		Date SmartSmth::getPurchaseDate() const { return release_date; }
		void SmartSmth::setTitle(const string& title)
		{
			if (title.length() >= SIZE_BUFF) {
				throw std::invalid_argument("Неверное значение название smart-девайса.");
			}
			strncpy_s(this->title, title.c_str(), SIZE_BUFF - 1);
			this->title[SIZE_BUFF - 1] = '\0';
		}
		string SmartSmth::getPassword() const { return string(owner_password); }
		void SmartSmth::setPassword(const string& password)
		{

			strncpy_s(owner_password, password.c_str(), SIZE_BUFF - 1);
			owner_password[SIZE_BUFF - 1] = '\0';
		}
		SmartSmth::SmartSmth(const string& owner_password, const string& title, const SmartType& anyType, const bool online, const Date& purchase_date) : isOnline(online), anyType(anyType)
		{
			setPurchaseDate(purchase_date);
			setTitle(title);
			setPassword(owner_password);
		}

		ostream& operator<<(ostream& out, const SmartSmth& device)
		{

			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {
	
				out.write(reinterpret_cast<const char*>(&device.anyType), sizeof(device.anyType));
				size_t title_size = strlen(device.title);
				out.write(reinterpret_cast<const char*>(&title_size), sizeof(title_size));
				out.write(device.title, title_size);

				size_t password_size = strlen(device.owner_password);
				out.write(reinterpret_cast<const char*>(&password_size), sizeof(password_size));
				out.write(device.owner_password, password_size);

				out.write(reinterpret_cast<const char*>(&device.isOnline), sizeof(device.isOnline));

				out << device.release_date;
			}
			else {

				out << setw(20) << left << "Название устройства: " << device.title << "\n";
				out << setw(20) << left << "Тип: ";
				switch (device.anyType) {
				case SmartType::Light: out << " Лампа"; break;
				case SmartType::Thermo: out << " Термостат"; break;
				case SmartType::SecCamera: out << " Камера безопасности"; break;
				}
				out << "\n";
				out << setw(20) << left << "Онлайн: " << (device.isOnline ? " Да" : " Нет") << "\n";
				out << setw(21) << left << "Дата покупки: " << device.release_date << "\n";
			}
			return out;
		}	

		istream& operator>>(istream& in, SmartSmth& device)
		{

			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {

				if (!in.read(reinterpret_cast<char*>(&device.anyType), sizeof(device.anyType))) {
					in.setstate(ios::failbit);
					return in;
				}

				size_t title_size = 0;
				if (!in.read(reinterpret_cast<char*>(&title_size), sizeof(title_size)) || title_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(device.title, title_size);
				device.title[title_size] = '\0';

				size_t password_size = 0;
				if (!in.read(reinterpret_cast<char*>(&password_size), sizeof(password_size)) || password_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(device.owner_password, password_size);
				device.owner_password[password_size] = '\0';

				if (!in.read(reinterpret_cast<char*>(&device.isOnline), sizeof(device.isOnline))) {
					in.setstate(ios::failbit);
					return in;
				}

				in >> device.release_date;
			}
			else {

				cout << "Введите название устройства: ";
				in.getline(device.title, SIZE_BUFF);

				cout << "Устройство онлайн? (0-Нет, 1-Да): ";
				while (true) {
					in >> device.isOnline;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите 0 или 1: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}

				cout << "Введите дату покупки (дд.мм.гггг): ";
				in >> device.release_date;
			}
			return in;
		}

		bool operator==(const SmartSmth& lhs, const SmartSmth& rhs)
		{
			return strcmp(lhs.title, rhs.title) == 0 &&
				strcmp(lhs.owner_password, rhs.owner_password) == 0 &&
				lhs.anyType == rhs.anyType &&
				lhs.isOnline == rhs.isOnline &&
				lhs.release_date == rhs.release_date;
		}

		//UserBirthDate

		Date::Date()
		{
			year_ = 0;
			month_ = 0;
			day_ = 0;
		}
		short Date::getDay() const { return day_; }
		short Date::getMonth() const { return month_; }
		unsigned short Date::getYear() const { return year_; }

		bool isLeapYear(const unsigned short year)
		{
			if (year % 400 == 0)
				return true;
			else if (year % 100 == 0)
				return false;
			else if (year % 4 == 0)
				return true;
			else
				return false;
		}

		bool validDate(const unsigned short year, const short month, const short day)
		{
			if (year < 1940 || year > 2025)
			{
				cerr << "Год должен быть в диапазоне 1940-2025" << endl;
				return false;
			}

			if (month < 1 || month > 12)
			{
				cerr << "Месяц должен быть в диапазоне 1-12" << endl;
				return false;
			}

			if (day < 1 || day > 31)
			{
				cerr << "День должен быть в диапазоне 1-31" << endl;
				return false;
			}

			const short daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			short maxDays = daysInMonth[month - 1];


			if (month == 2 && isLeapYear(year))
				maxDays = 29;


			if (day > maxDays)
			{
				cerr << "В выбранном месяце не может быть " << day << " дней" << endl;
				return false;
			}

			return true;
		}

		void Date::setDay(const short day) { day_ = day; }
		void Date::setMonth(const short month) { month_ = month; }
		void Date::setYear(const unsigned short year) { year_ = year; }
		void Date::setDate(const short year, const short month, const short day)
		{
			//if (validDate(year, month, day))
				//throw std::invalid_argument("Invalid date");
			setYear(year);
			setMonth(month);
			setDay(day);
		}

		Date::Date(const unsigned short year, const short month, const short day)
		{
			//if (validDate(year, month, day))
				//throw std::invalid_argument("Invalid date");
			year_ = year;
			month_ = month;
			day_ = day;
		}

		ostream& operator<<(ostream& out, const Date& date)
		{
			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {
	
				out.write(reinterpret_cast<const char*>(&date.year_), sizeof(date.year_));
				out.write(reinterpret_cast<const char*>(&date.month_), sizeof(date.month_));
				out.write(reinterpret_cast<const char*>(&date.day_), sizeof(date.day_));
			}
			else {
	
				out << date.day_ << "." << date.month_ << "." << date.year_;
			}
			return out;
		}

		istream& operator>>(istream& in, Date& date)
		{
			// Пытаемся преобразовать к ifstream (для бинарного файла)
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {
				// Это файловый поток - бинарное чтение
				if (!in.read(reinterpret_cast<char*>(&date.year_), sizeof(date.year_))) {
					in.setstate(ios::failbit);
					return in;
				}
				if (!in.read(reinterpret_cast<char*>(&date.month_), sizeof(date.month_))) {
					in.setstate(ios::failbit);
					return in;
				}
				if (!in.read(reinterpret_cast<char*>(&date.day_), sizeof(date.day_))) {
					in.setstate(ios::failbit);
					return in;
				}
			}
			else {

				bool valid_date = false;

				while (!valid_date) {
					cout << "Введите день: ";
					while (true) {
						in >> date.day_;
						if (cin.fail() || date.day_ < 1 || date.day_ > 31) {
							cin.clear();
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							cout << "Ошибка ввода! Введите число от 1 до 31: ";
						}
						else {
							break;
						}
					}

					cout << "Введите месяц: ";
					while (true) {
						in >> date.month_;
						if (cin.fail() || date.month_ < 1 || date.month_ > 12) {
							cin.clear();
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							cout << "Ошибка ввода! Введите число от 1 до 12: ";
						}
						else {
							break;
						}
					}

					cout << "Введите год: ";
					while (true) {
						in >> date.year_;
						if (cin.fail() || date.year_ < 1940 || date.year_ > 2025) {
							cin.clear();
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							cout << "Ошибка ввода! Введите число от 1940 до 2025: ";
						}
						else {
							break;
						}
					}

					if (validDate(date.year_, date.month_, date.day_)) {
						valid_date = true;
					}
					else {
						cout << "Некорректная дата! Пожалуйста, введите заново." << endl;
					}
				}
			}

			return in;
		}

		bool operator==(const Date& lhs, const Date& rhs)
		{
			return lhs.year_ == rhs.year_ &&
				lhs.month_ == rhs.month_ &&
				lhs.day_ == rhs.day_;
		}

		bool operator<(const Date& lhs, const Date& rhs)
		{

			if (lhs.year_ != rhs.year_) {
				return lhs.year_ < rhs.year_;
			}

			if (lhs.month_ != rhs.month_) {
				return lhs.month_ < rhs.month_;
			}

			
			return lhs.day_ < rhs.day_;
		}

		//UserLocation

		UserLocation::UserLocation()
		{
			memset(this->country, '\0', SIZE_BUFF);
			memset(this->city, '\0', SIZE_BUFF);
			memset(this->street, '\0', SIZE_BUFF);
		}

		UserLocation::UserLocation(const string& country, const string& city, const string& street)
		{
			setCountry(country);
			setCity(city);
			setStreet(street);
		}

		void UserLocation::setCountry(const string& country)
		{
			strncpy(this->country, country.c_str(), SIZE_BUFF - 1);
			this->country[SIZE_BUFF - 1] = '\0';
		}
		void UserLocation::setCity(const string& city)
		{
			strncpy(this->city, city.c_str(), SIZE_BUFF - 1);
			this->city[SIZE_BUFF - 1] = '\0';
		}
		void UserLocation::setStreet(const string& street)
		{
			strncpy(this->street, street.c_str(), SIZE_BUFF - 1);
			this->street[SIZE_BUFF - 1] = '\0';
		}
		string UserLocation::getCountry() const { return string(country); }
		string UserLocation::getCity() const { return string(city); }
		string UserLocation::getStreet() const { return string(street); }

		ostream& operator<<(ostream& out, const UserLocation& location)
		{

			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {

				size_t country_size = strlen(location.country);
				out.write(reinterpret_cast<const char*>(&country_size), sizeof(country_size));
				out.write(location.country, country_size);

				size_t city_size = strlen(location.city);
				out.write(reinterpret_cast<const char*>(&city_size), sizeof(city_size));
				out.write(location.city, city_size);

				size_t street_size = strlen(location.street);
				out.write(reinterpret_cast<const char*>(&street_size), sizeof(street_size));
				out.write(location.street, street_size);
			}
			else {
				out << "Страна: " << setw(15) << left << location.country
					<< "Город: " << setw(15) << left << location.city
					<< "Улица: " << location.street;
			}
			return out;
		}

		istream& operator>>(istream& in, UserLocation& location)
		{
			// Пытаемся преобразовать к ifstream (для бинарного файла)
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {
				// Это файловый поток - бинарное чтение
				size_t country_size = 0;
				if (!in.read(reinterpret_cast<char*>(&country_size), sizeof(country_size)) || country_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(location.country, country_size);
				location.country[country_size] = '\0';

				size_t city_size = 0;
				if (!in.read(reinterpret_cast<char*>(&city_size), sizeof(city_size)) || city_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(location.city, city_size);
				location.city[city_size] = '\0';

				size_t street_size = 0;
				if (!in.read(reinterpret_cast<char*>(&street_size), sizeof(street_size)) || street_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(location.street, street_size);
				location.street[street_size] = '\0';
			}
			else {
				cout << "Введите страну: ";
				in.getline(location.country, SIZE_BUFF);
				cout << "Введите город: ";
				in.getline(location.city, SIZE_BUFF);
				cout << "Введите улицу: ";
				in.getline(location.street, SIZE_BUFF);
			}
			return in;
		}

		bool operator==(const UserLocation& lhs, const UserLocation& rhs)
		{
			return strcmp(lhs.country, rhs.country) == 0 &&
				strcmp(lhs.city, rhs.city) == 0 &&
				strcmp(lhs.street, rhs.street) == 0;
		}

		//User

		User::User(const string& username = "User", const string& password = "random", const UserLocation& location = UserLocation(), const Date& date = Date(), UserRole role = User_) : location_(location), date_(date), anyRole(role)
		{
			setUserName(username);
			string temp_password = hashPassword(username, password);
			setPassword(temp_password);
		}

		UserLocation User::getLocation() const { return location_; }
		string User::getPassword() const { return string(hashed_password); }
		UserRole User::getRole() const { return anyRole; }
		Date User::getUserDate() const { return date_; }

		void User::setRole(const UserRole& role) { anyRole = role; }
		void User::setUserDate(const Date& date) { date_ = date; }
		void User::setUserLocation(const UserLocation& location) { location_ = location; }

		void User::setPassword(const string& password)
		{
			/*string hashed_password = hashPassword(string(this->user_name),password);*/
			strncpy_s(this->hashed_password, password.c_str(), SIZE_BUFF - 1);
			this->hashed_password[SIZE_BUFF - 1] = '\0';
		}

		void User::setUserName(const string& user_name)
		{
			//if (user_name.length() >= SIZE_BUFF) {
			//	throw std::invalid_argument("Неверное значение название smart-девайса.");
			//}
			strncpy_s(this->user_name, user_name.c_str(), SIZE_BUFF - 1);
			this->user_name[SIZE_BUFF - 1] = '\0';
		}

		string User::getUserName() const
		{
			return user_name;
		}

		ostream& operator<<(ostream& out, const User& user)
		{

			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {

				size_t name_size = strlen(user.user_name);
				out.write(reinterpret_cast<const char*>(&name_size), sizeof(name_size));
				out.write(user.user_name, name_size);

				size_t pass_size = strlen(user.hashed_password);
				out.write(reinterpret_cast<const char*>(&pass_size), sizeof(pass_size));
				out.write(user.hashed_password, pass_size);

				out << user.location_;
				out << user.date_;

				out.write(reinterpret_cast<const char*>(&user.anyRole), sizeof(user.anyRole));
			}
			else {

				out << setw(20) << left << "Имя пользователя: " << user.user_name << "\n";
				out << setw(20) << left << "Местоположение: " << user.location_ << "\n";
				out << setw(20) << left << "Дата рождения: " << user.date_ << "\n";
				out << setw(20) << left << "Роль: ";
				switch (user.anyRole) {
				case UserRole::Admin_: out << "Администратор"; break;
				case UserRole::User_: out << "Пользователь"; break;
				case UserRole::Guest_: out << "Гость"; break;
				}
				out << "\n";
			}

			return out;
		}

		istream& operator>>(istream& in, User& user)
		{
	
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {
	
				size_t name_size = 0;
				if (!in.read(reinterpret_cast<char*>(&name_size), sizeof(name_size)) || name_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(user.user_name, name_size);
				user.user_name[name_size] = '\0';

				size_t pass_size = 0;
				if (!in.read(reinterpret_cast<char*>(&pass_size), sizeof(pass_size)) || pass_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(user.hashed_password, pass_size);
				user.hashed_password[pass_size] = '\0';

				in >> user.location_;
				in >> user.date_;

				if (!in.read(reinterpret_cast<char*>(&user.anyRole), sizeof(user.anyRole))) {
					in.setstate(ios::failbit);
					return in;
				}
			}
			else {

				cout << "Введите имя пользователя: ";
				in.getline(user.user_name, SIZE_BUFF);


				string temp_password;

				temp_password = getPasswordWithDots();
				temp_password = hashPassword(user.getUserName(),temp_password);
				user.setPassword(temp_password);

				cout << "Введите данные о местоположении:" << endl;
				in >> user.location_;

				cout << "Введите дату рождения (дд.мм.гггг): " << endl;
				in >> user.date_;
			}

			return in;
		}

		bool operator==(const User& lhs, const User& rhs)
		{
			return strcmp(lhs.user_name, rhs.user_name) == 0 &&
				strcmp(lhs.hashed_password, rhs.hashed_password) == 0 &&
				lhs.location_ == rhs.location_ &&          
				lhs.date_ == rhs.date_ &&                   
				lhs.anyRole == rhs.anyRole;
		}

		//SmartLight

		SmartLight::SmartLight(const string& owner_password = "", const string& title = "", const bool online = false, const Date& purchase_date = Date(), const int brightness = 50, const string& color = "white") :
			SmartSmth(owner_password, title, anyType = Light, online, purchase_date), brightness_(brightness) {
			setColor(color);
		}

		int SmartLight::getBrightness() const { return brightness_; }

		string SmartLight::getColor() const { return string(color); }

		void SmartLight::setBrightness(const int brightness)
		{
			if (brightness_ < 0 || brightness_ > 100)
			{ }

			brightness_ = brightness_;
		}

		void SmartLight::setColor(const string& newColor) 
		{
			strncpy_s(color, newColor.c_str(), SIZE_BUFF - 1);
			color[SIZE_BUFF - 1] = '\0';
		}

		ostream& operator<<(ostream& out, const SmartLight& light)
		{
			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {

				out << static_cast<const SmartSmth&>(light);


				out.write(reinterpret_cast<const char*>(&light.brightness_), sizeof(light.brightness_));

				size_t color_size = strlen(light.color);
				out.write(reinterpret_cast<const char*>(&color_size), sizeof(color_size));
				out.write(light.color, color_size);
			}
			else {

				out << static_cast<const SmartSmth&>(light);


				out << setw(21) << left << "Яркость: " << light.brightness_ << "%\n";
				out << setw(21) << left << "Цвет: " << light.color << "\n";
			}
			return out;
		}

		istream& operator>>(istream& in, SmartLight& light)
		{
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {

				in >> static_cast<SmartSmth&>(light);

				if (!in.read(reinterpret_cast<char*>(&light.brightness_), sizeof(light.brightness_))) {
					in.setstate(ios::failbit);
					return in;
				}

				size_t color_size = 0;
				if (!in.read(reinterpret_cast<char*>(&color_size), sizeof(color_size)) || color_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(light.color, color_size);
				light.color[color_size] = '\0';
			}
			else {
	
				in >> static_cast<SmartSmth&>(light);


				cout << "Введите яркость (0-100): ";
				while (true) {
					in >> light.brightness_;
					if (cin.fail() || light.brightness_ < 0 || light.brightness_ > 100) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число от 0 до 100: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}

				cout << "Введите цвет: ";
				in.getline(light.color, SIZE_BUFF);
			}
			return in;
		}

		bool operator==(const SmartLight& lhs, const SmartLight& rhs)
		{

			const SmartSmth& base_lhs = lhs;
			const SmartSmth& base_rhs = rhs;
			if (!(base_lhs == base_rhs)) return false;

			return lhs.getBrightness() == rhs.getBrightness() &&
				lhs.getColor() == rhs.getColor();
		}

		// Thermostat

		Thermostat::Thermostat(const string& owner_password = "", const string& title = "", const bool online = false, const Date& purchase_date = Date(), const double currentTemperature = 0.0, const double targetTemperature = 0.0, const string& mode = "cooling")
			: SmartSmth(owner_password, title, Thermo, online, purchase_date), currentTemperature(21.0), targetTemperature(22.0)
		{
			setMode(mode);
		}

		double Thermostat::getCurTemperature() const { return currentTemperature; }

		void Thermostat::setTargetTemperature(double temp) {
			if (temp < -50.0 || temp > 50.0) {

			}
			targetTemperature = temp;
		}

		void Thermostat::setCurTemperature(double temp)
		{
			currentTemperature = temp;
		}

		double Thermostat::getTargetTemperature() const { return targetTemperature; }

		void Thermostat::setMode(const string& newMode)
		{
			if (newMode.length() >= SIZE_BUFF) {
				
			}
			strncpy_s(mode, newMode.c_str(), SIZE_BUFF - 1);
		}

		string Thermostat::getMode() const { return mode; }

		ostream& operator<<(ostream& out, const Thermostat& thermostat)
		{
			
			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {
				
				out << static_cast<const SmartSmth&>(thermostat);

				
				out.write(reinterpret_cast<const char*>(&thermostat.currentTemperature), sizeof(thermostat.currentTemperature));
				out.write(reinterpret_cast<const char*>(&thermostat.targetTemperature), sizeof(thermostat.targetTemperature));

				size_t mode_size = strlen(thermostat.mode);
				out.write(reinterpret_cast<const char*>(&mode_size), sizeof(mode_size));
				out.write(thermostat.mode, mode_size);
			}
			else {
				
				out << static_cast<const SmartSmth&>(thermostat);

				
				out << setw(21) << left << "Текущая температура: " << thermostat.currentTemperature << "C\n";
				out << setw(21) << left << "Целевая температура: " << thermostat.targetTemperature << "C\n";
				out << setw(21) << left << "Режим: " << thermostat.mode << "\n";
			}
			return out;
		}

		istream& operator>>(istream& in, Thermostat& thermostat)
		{
			
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {
				
				
				in >> static_cast<SmartSmth&>(thermostat);

				
				if (!in.read(reinterpret_cast<char*>(&thermostat.currentTemperature), sizeof(thermostat.currentTemperature))) {
					in.setstate(ios::failbit);
					return in;
				}

				if (!in.read(reinterpret_cast<char*>(&thermostat.targetTemperature), sizeof(thermostat.targetTemperature))) {
					in.setstate(ios::failbit);
					return in;
				}

				size_t mode_size = 0;
				if (!in.read(reinterpret_cast<char*>(&mode_size), sizeof(mode_size)) || mode_size >= SIZE_BUFF) {
					in.setstate(ios::failbit);
					return in;
				}
				in.read(thermostat.mode, mode_size);
				thermostat.mode[mode_size] = '\0';
			}
			else {
				
				in >> static_cast<SmartSmth&>(thermostat);

				
				cout << "Введите текущую температуру: ";
				while (true) {
					in >> thermostat.currentTemperature;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}

				cout << "Введите целевую температуру: ";
				while (true) {
					in >> thermostat.targetTemperature;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}

				cout << "Введите режим (обогрев/охлаждение/авто): ";
				in.getline(thermostat.mode, SIZE_BUFF);
			}
			return in;
		}

		bool operator==(const Thermostat& lhs, const Thermostat& rhs)
		{
	
			const SmartSmth& base_lhs = lhs;
			const SmartSmth& base_rhs = rhs;
			if (!(base_lhs == base_rhs)) return false;

	
			return lhs.currentTemperature == rhs.currentTemperature &&
				lhs.targetTemperature == rhs.targetTemperature &&
				strcmp(lhs.mode, rhs.mode) == 0;
		}

		// SecurityCamera

		SecurityCamera::SecurityCamera(const string& owner_password = "", const string& title = "", const bool online = false, const Date& purchase_date = Date(), const int resolution = 1080, const bool recording = false, const bool motionDetectionEnabled = false)
			: SmartSmth(owner_password, title, SecCamera, online, purchase_date), isRecording(false), motionDetectionEnabled(true), resolution(resolution) {}

		int SecurityCamera::getResolution() const { return resolution; }
		bool SecurityCamera::getMotion() const { return motionDetectionEnabled; }
		bool SecurityCamera::getRecording() const { return isRecording; }

		ostream& operator<<(ostream& out, const SecurityCamera& camera)
		{
		
			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {
				
				out << static_cast<const SmartSmth&>(camera);

				
				out.write(reinterpret_cast<const char*>(&camera.isRecording), sizeof(camera.isRecording));
				out.write(reinterpret_cast<const char*>(&camera.resolution), sizeof(camera.resolution));
				out.write(reinterpret_cast<const char*>(&camera.motionDetectionEnabled), sizeof(camera.motionDetectionEnabled));
			}
			else {
				
				out << static_cast<const SmartSmth&>(camera);

				out << setw(21) << left << "Запись: " << (camera.isRecording ? "Да" : "Нет") << "\n";
				out << setw(21) << left << "Разрешение: " << camera.resolution << "p\n";
				out << setw(21) << left << "Детекция движения: " << (camera.motionDetectionEnabled ? "Включена" : "Выключена") << "\n";
			}
			return out;
		}

		istream& operator>>(istream& in, SecurityCamera& camera)
		{
		
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {
				
				in >> static_cast<SmartSmth&>(camera);

				if (!in.read(reinterpret_cast<char*>(&camera.isRecording), sizeof(camera.isRecording))) {
					in.setstate(ios::failbit);
					return in;
				}

				if (!in.read(reinterpret_cast<char*>(&camera.resolution), sizeof(camera.resolution))) {
					in.setstate(ios::failbit);
					return in;
				}

				if (!in.read(reinterpret_cast<char*>(&camera.motionDetectionEnabled), sizeof(camera.motionDetectionEnabled))) {
					in.setstate(ios::failbit);
					return in;
				}
			}
			else {
				in >> static_cast<SmartSmth&>(camera);

				
				cout << "Идет запись? (0-Нет, 1-Да): ";
				while (true) {
					in >> camera.isRecording;
					if (cin.fail() || (camera.isRecording != 0 && camera.isRecording != 1)) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите 0 или 1: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}

				cout << "Введите разрешение (720/1080/1440/2160): ";
				while (true) {
					in >> camera.resolution;
					if (cin.fail() || (camera.resolution != 720 && camera.resolution != 1080 &&
						camera.resolution != 1440 && camera.resolution != 2160)) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите 720, 1080, 1440 или 2160: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}

				cout << "Детекция движения включена? (0-Нет, 1-Да): ";
				while (true) {
					in >> camera.motionDetectionEnabled;
					if (cin.fail() || (camera.motionDetectionEnabled != 0 && camera.motionDetectionEnabled != 1)) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите 0 или 1: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
			}
			return in;
		}

		bool operator==(const SecurityCamera& lhs, const SecurityCamera& rhs)
		{
			
			const SmartSmth& base_lhs = lhs;
			const SmartSmth& base_rhs = rhs;
			if (!(base_lhs == base_rhs)) return false;

			return lhs.isRecording == rhs.isRecording &&
				lhs.resolution == rhs.resolution &&
				lhs.motionDetectionEnabled == rhs.motionDetectionEnabled;
		}

		//DeviceVariant

		inline istream& operator>>(istream& in, DeviceVariant& variant) {
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {

				SmartType type;
				streampos pos = in.tellg();


				in.read(reinterpret_cast<char*>(&type), sizeof(type));

				in.seekg(pos);

				switch (type) {
				case Light: {
					SmartLight light;
					in >> light;
					variant = light;
					break;
				}
				case Thermo: {
					Thermostat thermo;
					in >> thermo;
					variant = thermo;
					break;
				}
				case SecCamera: {
					SecurityCamera camera;
					in >> camera;
					variant = camera;
					break;
				}
				}
			}
			else {
				std::visit([&in](auto&& dev) {
					in >> dev;
					}, variant);
			}
			return in;
		}

		inline ostream& operator<<(ostream& out, const DeviceVariant& variant) {
			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {
				std::visit([&out](const auto& device) {
					out << device; 
					}, variant);
			}
			else {

				std::visit([&out](const auto& device) {
					out << device;  
					}, variant);
			}
			return out;
		}

		inline bool operator==(const DeviceVariant& lhs, const DeviceVariant& rhs) {
			return std::visit([](const auto& device1, const auto& device2) -> bool
				{
				if (typeid(device1) != typeid(device2))
					return false;
				return device1 == device2;
				}, lhs, rhs);
		}

		//DeviceScript

		template<smartDeviceType T>
		DeviceScript<T>::DeviceScript()
			: smart_device(), duration_(0)
		{
			memset(script, '\0', SIZE_BUFF);
		}

		template<smartDeviceType T>
		DeviceScript<T>::DeviceScript(const T& smart_device, const string& script, const short duration)
		{
			this->smart_device = smart_device;
			setScript(script);
			duration_ = duration;
		}

		template<smartDeviceType T>
		void DeviceScript<T>::setScript(const string& script)
		{
			/*if ()
			{

			}*/
			strncpy(this->script, script.c_str(), SIZE_BUFF - 1);
			this->script[SIZE_BUFF - 1] = '\0';
		}

		template<smartDeviceType T>
		void DeviceScript<T>::setDevice(const T& smart_device)
		{
			this->smart_device = smart_device;
		}

		template<smartDeviceType T>
		void DeviceScript<T>::setDuration(const short duration)
		{
			this->duration_ = duration;
		}

		template<smartDeviceType T>
		std::string DeviceScript<T>::getScript() const
		{
			return script;
		}

		template<smartDeviceType T>
		T DeviceScript<T>::getDevice() const
		{
			return smart_device;
		}

		template<smartDeviceType T>
		short DeviceScript<T>::getDuration() const
		{
			return duration_;
		}

		inline ostream& operator<<(ostream& out, const DeviceScriptVariant& variant) {
			ofstream* ofs = dynamic_cast<ofstream*>(&out);
			fstream* fs = dynamic_cast<fstream*>(&out);

			if (ofs != nullptr || fs != nullptr) {

				std::visit([&out](const auto& script) {
					out << script;  
					}, variant);
			}
			else {
				
				std::visit([&out](const auto& script) {
					using T = std::decay_t<decltype(script)>;
					if constexpr (std::is_same_v<T, DeviceScript<SmartLight>>) {
						out << "[Скрипт умной лампы]\n";
					}
					else if constexpr (std::is_same_v<T, DeviceScript<Thermostat>>) {
						out << "[Скрипт термостата]\n";
					}
					else if constexpr (std::is_same_v<T, DeviceScript<SecurityCamera>>) {
						out << "[Скрипт камеры безопасности]\n";
					}
					out << script;  
					}, variant);
			}
			return out;
		}

		inline istream& operator>>(istream& in, DeviceScriptVariant& variant) {
			ifstream* ifs = dynamic_cast<ifstream*>(&in);
			fstream* fs = dynamic_cast<fstream*>(&in);

			if (ifs != nullptr || fs != nullptr) {

				SmartType type;
				in.read(reinterpret_cast<char*>(&type), sizeof(type));

				in.seekg(-static_cast<int>(sizeof(type)), ios::cur);

				switch (type) {
				case Light: {
					DeviceScript<SmartLight> light_script;
					in >> light_script;
					variant = light_script;
					break;
				}
				case Thermo: {
					DeviceScript<Thermostat> thermo_script;
					in >> thermo_script;
					variant = thermo_script;
					break;
				}
				case SecCamera: {
					DeviceScript<SecurityCamera> camera_script;
					in >> camera_script;
					variant = camera_script;
					break;
				}
				}
			}
			return in;
		}

		//FileSystem<User>

		string FileSystem<User>::getFileName() const
		{
			return file_name;
		}

		FileSystem<User>::FileSystem()
		{
			file_name = "users_storage.dat";
		}

		short FileSystem<User>::checkUser(User& user_to_check)
		{
			smartFile.open(file_name, std::ios::in | std::ios::binary);

			if (!smartFile.is_open()) {
				smartFile.open(file_name, std::ios::out | std::ios::binary);
				if (smartFile.is_open()) {
					smartFile.close();
					return false;
				}
				else {
					throw std::runtime_error("Не удалось создать файл пользователей: " + file_name);
				}
			}

			smartFile.seekg(0, std::ios::end);
			if (smartFile.tellg() == 0) {
				smartFile.close();
				return false;
			}
			smartFile.seekg(0, std::ios::beg);

			User temp_user;
			while (smartFile >> temp_user) {
				if (user_to_check.getUserDate() != Date() && user_to_check.getLocation() != UserLocation())
				{
					if (temp_user.getUserName() == user_to_check.getUserName())
					{
						if (temp_user == user_to_check)
						{
							smartFile.close();
							return 1;
						}
						else
						{
							smartFile.close();
							return 2;

						}
					}
					
				}
				else if (temp_user.getPassword() == user_to_check.getPassword() && temp_user.getUserName() == user_to_check.getUserName() && temp_user.getRole() == user_to_check.getRole())
				{
					user_to_check = temp_user;
					smartFile.close();
					return 1;
				}
			}

			if (smartFile.fail() && !smartFile.eof()) {
				smartFile.close();
				throw std::runtime_error("Ошибка чтения файла пользователей");
			}

			smartFile.close();
			return 0;
		}

		void FileSystem<User>::writeF(const User& user)
		{
			smartFile.open(file_name, std::ios::out | std::ios::binary | std::ios::app); // ate

			if (!smartFile.is_open()) {
				throw std::runtime_error("Не удалось открыть файл для записи: " + file_name);
			}

				smartFile << user;

				if (smartFile.fail()) 
				{
					smartFile.close();
					throw std::runtime_error("Ошибка записи пользователя в файл");
				}

			smartFile.close();
		}

		void FileSystem<User>::readF_a()
		{
			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open())
			{
				throw runtime_error(OpenFileERROR);
			}

			smartFile.seekg(0, ios::beg);

			cout << "\n=== ВСЕ ПОЛЬЗОВАТЕЛИ ===\n";
			int user_count = 0;
			User current_user;

			while (smartFile >> current_user)
			{
				user_count++;
				cout << "\nПользователь #" << user_count << ":\n";
				cout << current_user;
				cout << "----------------------------------------\n";
			}

			smartFile.close();

			cout << "Всего пользователей: " << user_count << "\n";
			
		}

		User FileSystem<User>::chooseUser_a(const string& name)
		{
			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open())
			{
				throw runtime_error(OpenFileERROR);
			}

			smartFile.seekg(0, ios::beg);

			User current_user;
			bool found = false;

			while (smartFile >> current_user)
			{
				if (current_user.getUserName() == name && current_user.getRole() != Admin_)
				{
					found = true;
					break;
				}
			}

			smartFile.close();

			if (!found)
			{
				cout << "Пользователь с именем '" + name + "' не найден";
				return User();
			}

			return current_user;
		}

		/*void FileSystem<User>::removeF(const User& user_to_delete)
		{
			smartFile.open(file_name, ios::in | ios::out | ios::binary);
			if (!smartFile.is_open())
			{
				throw runtime_error(OpenFileERROR);
			}

			smartFile.seekg(0, ios::end);
			std::streampos file_size = smartFile.tellg();

			if (file_size <= 0)
			{
				smartFile.close();
				throw runtime_error("Файл пользователей пуст");
			}

			smartFile.seekg(0, ios::beg);

			bool user_found = false;
			std::streampos delete_position = 0;
			User current_user;


			while (smartFile >> current_user)
			{
				std::streampos current_pos = smartFile.tellg();

				if (!user_found && current_user == user_to_delete)
				{
					user_found = true;
					delete_position = current_pos;

					delete_position -= static_cast<std::streamoff>(sizeof(User));
					break;
				}
			}

			if (!user_found)
			{
				smartFile.close();
				throw runtime_error("Пользователь не найден");
			}


			smartFile.clear();
			std::streampos read_position = delete_position;
			read_position += static_cast<std::streamoff>(sizeof(User));


			if (read_position < file_size)
			{
				User temp_user;
				std::streampos write_position = delete_position;

				while (read_position < file_size)
				{

					smartFile.seekg(read_position);
					if (!(smartFile >> temp_user))
						break;

					smartFile.seekp(write_position);
					smartFile << temp_user;

					read_position += static_cast<std::streamoff>(sizeof(User));
					write_position += static_cast<std::streamoff>(sizeof(User));

					if (read_position >= file_size)
						break;
				}
			}

			smartFile.close();

			std::streampos new_size = file_size;
			new_size -= static_cast<std::streamoff>(sizeof(User));

			std::filesystem::resize_file(file_name, new_size);

			cout << "Пользователь успешно удален\n";
		}*/

		void FileSystem<User>::removeF(const User& user_to_delete)
		{

			ifstream in_file(file_name, ios::in | ios::binary);
			if (!in_file.is_open())
			{
				throw runtime_error(OpenFileERROR);
			}

			string temp_filename = "temp_" + file_name;
			ofstream temp_file(temp_filename, ios::out | ios::binary);
			if (!temp_file.is_open())
			{
				in_file.close();
				throw runtime_error("Не удалось создать временный файл");
			}


			User current_user;
			bool user_found = false;
			int remaining_users = 0;

			while (in_file >> current_user)
			{
				if (current_user == user_to_delete)
				{
					user_found = true;
					continue;
				}

				temp_file << current_user;
				remaining_users++;
			}

			in_file.close();
			temp_file.close();

			if (!user_found)
			{

				filesystem::remove(temp_filename);
				throw runtime_error("Пользователь не найден");
			}

			if (!filesystem::remove(file_name))
			{
				filesystem::remove(temp_filename);
				throw runtime_error("Не удалось удалить старый файл");
			}

			filesystem::rename(temp_filename, file_name);

			cout << "Пользователь успешно удален. Осталось пользователей: "
				<< remaining_users << "\n";
		}

		//FileSystem<DeviceVariant>

		bool FileSystem<DeviceVariant>::sortF(std::function<bool(const DeviceVariant&, const DeviceVariant&)> comp)
		{
			fstream file(file_name, ios::in | ios::out | ios::binary);
			if (!file.is_open()) {
				cout << "Файл устройств не найден\n";
				return false;
			}

			vector<DeviceVariant> devices;
			DeviceVariant device;

			while (file >> device) {
				devices.push_back(device);
			}

			if (devices.empty()) {
				cout << "Файл устройств пуст\n";
				file.close();
				return false;
			}


			std::sort(devices.begin(), devices.end(), comp);

			file.close();
			file.open(file_name, ios::out | ios::trunc | ios::binary);

			if (!file.is_open()) {
				cout << "Ошибка при открытии файла для записи\n";
				return false;
			}

			for (const auto& dev : devices) {
				file << dev;
			}

			file.close();
			cout << "Устройства отсортированы\n";
			return true;
		}

		bool FileSystem<DeviceVariant>::checkDevice(const DeviceVariant& device)
		{
			smartFile.open(file_name, std::ios::in | std::ios::binary);

			if (!smartFile.is_open())
				return false;


			smartFile.seekg(0, std::ios::end);
			if (smartFile.tellg() == 0) {
				smartFile.close();
				return false;
			}
			smartFile.seekg(0, std::ios::beg);

			DeviceVariant temp_device;
			bool found = false;

				while (smartFile >> temp_device)
				{
					if (temp_device == device) {
						found = true;
						break;
					}
				}

			smartFile.close();
			return found;
		}

		int FileSystem<DeviceVariant>::readF()
		{

			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				cout << "Файл устройств не найден или пуст\n";
				return 0;
			}

			smartFile.seekg(0, ios::end);
			if (smartFile.tellg() == 0) {
				cout << "У вас пока нет устройств\n";
				smartFile.close();
				return 0;
			}
			smartFile.seekg(0, ios::beg);

			cout << "Ваши устройства\n";
			int device_count = 0;

			while (!smartFile.eof()) {
				DeviceVariant device;
				smartFile >> device;

				if (smartFile.good()) {
					cout << "Устройство " << ++device_count << ":\n";
					cout << device;
					cout << string(40, '-') << endl;
				}
				else {
					if (!smartFile.eof()) {
						smartFile.clear();
						smartFile.ignore(1024, '\n');
					}
				}

				if (smartFile.eof()) break;
			}

			cout << "Всего устройств: " << device_count << "\n\n";
			
			smartFile.close();
			return device_count;
		}

		DeviceVariant FileSystem<DeviceVariant>::chooseCertainDevice(const int count)
		{
			if (count <= 0) {
				throw std::runtime_error("Нет доступных устройств для выбора");
			}

			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				throw std::runtime_error("Не удалось открыть файл устройств: " + file_name);
			}

			int choice;
			cout << "Выберите устройство (1-" << count << "): ";

			while (true) {
				cin >> choice;
				if (cin.fail() || choice < 1 || choice > count) {
					cin.clear();
					cin.ignore((numeric_limits<streamsize>::max)(), '\n');
					cout << "Ошибка ввода! Введите число от 1 до " << count << ": ";
				}
				else {
					cin.ignore((numeric_limits<streamsize>::max)(), '\n');
					break;
				}
			}


			DeviceVariant selected_device;
			int current = 0;

			while (current < choice && smartFile >> selected_device) {
				current++;
			}

			smartFile.close();

			if (current != choice) {
				throw std::runtime_error("Ошибка: не удалось прочитать выбранное устройство");
			}

			return selected_device;
		}

		string FileSystem<DeviceVariant>::getFileName() const
		{
			return file_name;
		}

		FileSystem<DeviceVariant>::FileSystem()
		{
			file_name = "device_storage.dat";
		}

		void FileSystem<DeviceVariant>::setFileName(const string& file_name)
		{
			this->file_name = file_name + ".dat";
		}

		void FileSystem<DeviceVariant>::writeF(const DeviceVariant& device) {
			smartFile.open(file_name, std::ios::out | std::ios::binary | std::ios::app);

			if (!smartFile.is_open()) {
				throw std::runtime_error("Не удалось открыть файл для записи: " + file_name);
			}

			smartFile << device;

			if (smartFile.fail())
			{
				smartFile.close();
				throw std::runtime_error("Ошибка записи устройства в файл");
			}

			smartFile.close();
		}

		DeviceVariant FileSystem<DeviceVariant>::removeF(int pos)
		{

			smartFile.open(file_name, ios::in | ios::out | ios::binary);
			if (!smartFile.is_open()) {
				throw std::runtime_error("Не удалось открыть файл устройств: " + file_name);
			}

			smartFile.seekg(0, ios::end);
			std::streampos fileSize = smartFile.tellg();

			smartFile.seekg(0, ios::beg);

			DeviceVariant device_to_remove;
			int current_pos = 1;
			std::streampos remove_position = 0;


			while (current_pos < pos) {
				remove_position = smartFile.tellg();

				DeviceVariant temp;
				if (!(smartFile >> temp)) {
					smartFile.close();
					throw std::runtime_error("Ошибка чтения устройства на позиции " + std::to_string(current_pos));
				}
				current_pos++;
			}


			remove_position = smartFile.tellg();

			if (!(smartFile >> device_to_remove)) {
				smartFile.close();
				throw std::runtime_error("Не удалось прочитать устройство для удаления на позиции " + std::to_string(pos));
			}


			std::streampos next_position = smartFile.tellg();

			if (next_position < fileSize) {
				DeviceVariant temp_device;
				std::streampos current_read_pos = next_position;
				std::streampos current_write_pos = remove_position;

				while (current_read_pos < fileSize) {
					smartFile.seekg(current_read_pos);
					if (!(smartFile >> temp_device)) {
						break;
					}

					smartFile.seekp(current_write_pos);
					smartFile << temp_device;

					current_read_pos = smartFile.tellg();
					current_write_pos = smartFile.tellp();

					if (current_read_pos >= fileSize) break;
				}
			}

			smartFile.close();

			std::streampos new_size;
			if (next_position >= fileSize) {

				new_size = remove_position;
			}
			else {

				new_size = fileSize - (next_position - remove_position);
			}


			std::filesystem::resize_file(file_name, new_size);

			cout << "Устройство на позиции " << pos << " успешно удалено\n";
			return device_to_remove;
		}

		/*DeviceVariant FileSystem<DeviceVariant>::editF(int pos, const DeviceVariant& new_device)
		{
			if (pos < 1) {
				throw std::invalid_argument("Позиция должна быть положительным числом");
			}

			smartFile.open(file_name, ios::in | ios::out | ios::binary);
			if (!smartFile.is_open()) {
				throw std::runtime_error("Не удалось открыть файл устройств: " + file_name);
			}

			smartFile.seekg(0, ios::end);
			std::streampos fileSize = smartFile.tellg();
			if (fileSize <= 0) {
				smartFile.close();
				throw std::runtime_error("Файл устройств пуст");
			}
			smartFile.seekg(0, ios::beg);

			DeviceVariant old_device;
			int current_pos = 1;
			std::streampos edit_position = 0;

			while (current_pos < pos) {
				edit_position = smartFile.tellg();

				DeviceVariant temp;
				if (!(smartFile >> temp)) {
					smartFile.close();
					throw std::runtime_error("Ошибка чтения устройства на позиции " + std::to_string(current_pos));
				}
				current_pos++;
			}


			edit_position = smartFile.tellg();


			if (!(smartFile >> old_device)) {
				smartFile.close();
				throw std::runtime_error("Не удалось прочитать устройство для редактирования на позиции " + std::to_string(pos));
			}

			smartFile.seekp(edit_position);


			smartFile << new_device;

			smartFile.close();

			cout << "Устройство на позиции " << pos << " успешно отредактировано\n";
			cout << "Возвращаемое старое устройство можно использовать для удаления в сценариях при необходимости\n";

			return old_device;
		}*/

		/*DeviceVariant FileSystem<DeviceVariant>::editF(const DeviceVariant& old_device, const DeviceVariant& new_device)
		{
			if (pos < 1) {
				throw std::invalid_argument("Позиция должна быть положительным числом");
			}

			std::ifstream inFile(file_name, std::ios::binary);
			if (!inFile.is_open()) {
				throw std::runtime_error("Не удалось открыть файл устройств: " + file_name);
			}

			std::string temp_file_name = file_name + ".tmp";
			std::ofstream outFile(temp_file_name, std::ios::binary);
			if (!outFile.is_open()) {
				inFile.close();
				throw std::runtime_error("Не удалось создать временный файл");
			}

			DeviceVariant old_device;
			int current_pos = 0;
			bool device_found = false;

			while (inFile.peek() != EOF) {
				current_pos++;

				if (current_pos == pos) {

					inFile >> old_device;

					if (!inFile.good() && !inFile.eof()) {
						inFile.close();
						outFile.close();
						std::filesystem::remove(temp_file_name);
						throw std::runtime_error("Ошибка чтения устройства на позиции " + std::to_string(pos));
					}

					device_found = true;
					outFile << new_device;

					if (!outFile.good()) {
						inFile.close();
						outFile.close();
						std::filesystem::remove(temp_file_name);
						throw std::runtime_error("Ошибка записи нового устройства во временный файл");
					}
				}
				else {

					DeviceVariant device;
					inFile >> device;

					if (!inFile.good() && !inFile.eof()) {
						inFile.close();
						outFile.close();
						std::filesystem::remove(temp_file_name);
						throw std::runtime_error("Ошибка чтения устройства на позиции " + std::to_string(current_pos));
					}

					outFile << device;

					if (!outFile.good()) {
						inFile.close();
						outFile.close();
						std::filesystem::remove(temp_file_name);
						throw std::runtime_error("Ошибка записи устройства во временный файл");
					}
				}
			}

			inFile.close();
			outFile.close();

			if (!device_found) {
				std::filesystem::remove(temp_file_name);
				throw std::runtime_error("Устройство на позиции " + std::to_string(pos) + " не найдено");
			}


				std::filesystem::copy(temp_file_name, file_name, std::filesystem::copy_options::overwrite_existing);
				std::filesystem::remove(temp_file_name);
		
			std::cout << "Устройство на позиции " << pos << " успешно отредактировано\n";
			return old_device;
		}*/

		void FileSystem<DeviceVariant>::editF(const DeviceVariant& old_device, const DeviceVariant& new_device)
		{

			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open())
			{
				throw runtime_error(OpenFileERROR);
			}

			string temp_file_name = "temp_" + file_name;
			ofstream tempFile(temp_file_name, ios::out | ios::binary);
			if (!tempFile.is_open())
			{
				smartFile.close();
				throw runtime_error("Не удалось создать временный файл.");
			}

			bool found = false;
			DeviceVariant current_device;

			while (smartFile >> current_device)
			{
				if (!found && current_device == old_device)
				{
					found = true;
					tempFile << new_device;
				}
				else
				{
					tempFile << current_device;
				}
			}

			smartFile.close();
			tempFile.close();


				if (!filesystem::remove(file_name))
				{
					filesystem::remove(temp_file_name);
					throw runtime_error("Не удалось удалить старый файл.");
				}

				filesystem::rename(temp_file_name, file_name);
				return;

		}

		void FileSystem<DeviceVariant>::searchF(const string& dev_name)
		{
			std::ifstream file(file_name, ios::binary);
			if (!file.is_open()) {
				cout << "Файл устройств не найден\n";
				return;
			}

			cout << "Результаты поиска: '" << dev_name << "' \n";
			int found_count = 0;
			DeviceVariant device;

			while (file >> device) {
				std::visit([&](const auto& dev) {
						string title = dev.getTitle();

						if (title.find(dev_name) != string::npos) {
							cout << "Найденное устройство " << ++found_count << ":\n";
							cout << dev;
							cout << string(40, '-') << endl;
						}
					}
					, device);
			}

			if (found_count == 0) {
				cout << "Устройства не найдены\n";
			}
		}

		void FileSystem<DeviceVariant>::filter(std::function<bool(const DeviceVariant&)> predicate,
			const std::string& filter_name)
		{
			std::ifstream file(file_name, ios::binary);
			if (!file.is_open()) {
				cout << "Файл устройств не найден\n";
				return;
			}

			file.seekg(0, ios::end);
			if (file.tellg() == 0) {
				cout << "Файл устройств пуст\n";
				file.close();
				return;
			}
			file.seekg(0, ios::beg);

			cout << " Фильтр: " << filter_name << " \n";
			int filtered_count = 0;
			DeviceVariant device;

			while (file >> device) {
				if (predicate(device)) {
					cout << "Устройство " << ++filtered_count << ":\n";
					cout << device;
					cout << string(40, '-') << endl;
				}
			}

			if (filtered_count == 0) {
				cout << "Устройства не найдены\n";
			}
			else {
				cout << "Найдено устройств: " << filtered_count << "\n";
			}

			file.close();
		}

		void FileSystem<DeviceVariant>::generateDeviceReport(const string& report_file_name)
		{
			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open())
			{
				throw runtime_error(OpenFileERROR);
			}

			ofstream report_file(report_file_name, ios::out | ios::app);
			if (!report_file.is_open())
			{
				smartFile.close();
				throw runtime_error("Не удалось открыть файл отчета");
			}

			report_file << string(40, '=') << "\n";
			report_file << "УСТРОЙСТВА:\n";
			report_file << string(40, '=') << "\n";

			smartFile.seekg(0, ios::end);
			if (smartFile.tellg() == 0)
			{
				report_file << "Нет подключенных устройств\n";
				smartFile.close();
				report_file.close();
				return;
			}
			smartFile.seekg(0, ios::beg);



			vector<DeviceVariant> devices;
			DeviceVariant device;


			while (smartFile >> device)
			{
				devices.push_back(device);
			}
			smartFile.close();

			int device_count = 0;
			for (const auto& dev : devices)
			{
				device_count++;
				report_file << "\nУстройство #" << device_count << ":\n";

				stringstream device_ss;
				device_ss << dev;
				report_file << device_ss.str();

				report_file << string(40, '-') << "\n";
			}

			report_file << string(40, '=') << "\n";
			report_file << "СТАТИСТИКА УСТРОЙСТВ:\n";
			report_file << string(40, '=') << "\n";
			report_file << "Всего устройств: " << devices.size() << "\n";

			int lights = 0, thermos = 0, cameras = 0, online = 0;

			for (const auto& dev : devices)
			{
				SmartType type = std::visit([](auto&& arg) {
					return arg.getType();
					}, dev);

				switch (type)
				{
				case SmartType::Light: lights++; break;
				case SmartType::Thermo: thermos++; break;
				case SmartType::SecCamera: cameras++; break;
				}

				if (std::visit([](auto&& arg) { return arg.getOnline(); }, dev))
				{
					online++;
				}
			}

			report_file << "  • Умных ламп: " << lights << "\n";
			report_file << "  • Термостатов: " << thermos << "\n";
			report_file << "  • Камер безопасности: " << cameras << "\n";
			report_file << "  • Онлайн: " << online << " из " << devices.size() << " ("
				<< fixed << setprecision(1)
				<< (devices.size() > 0 ? (online * 100.0 / devices.size()) : 0)
				<< "%)\n";

			report_file.close();
		}

		//FileSystem<DeviceScriptVariant>

		string FileSystem<DeviceScriptVariant>::getFileName() const { return file_name; }

		FileSystem<DeviceScriptVariant>::FileSystem() { file_name = "device_scripts_storage.dat"; }

		void FileSystem<DeviceScriptVariant>::writeF(queue<DeviceScriptVariant>& queue)
		{
			if (queue.empty()) {
				cout << "Очередь сценариев пуста\n";
				return;
			}

			smartFile.open(file_name, ios::out | ios::binary | ios::app);
			if (!smartFile.is_open()) {
				throw runtime_error("Не удалось открыть файл сценариев для записи");
			}

			while (!queue.empty()) {
				smartFile << queue.front();
				queue.pop();
			}

			smartFile.close();
			cout << "Очередь сценариев успешно записана в файл\n";
		}

		void FileSystem<DeviceScriptVariant>::removeF(const User& user)
		{
			fstream file(file_name, ios::in | ios::out | ios::binary);
			if (!file.is_open()) {
				cout << "Файл сценариев не найден\n";
				return;
			}

			file.seekg(0, ios::end);
			std::streampos fileSize = file.tellg();
			if (fileSize <= 0) {
				cout << "Файл сценариев пуст\n";
				file.close();
				return;
			}
			file.seekg(0, ios::beg);

			int removed_count = 0;
			std::streampos read_pos = 0;
			std::streampos write_pos = 0;

			while (read_pos < fileSize) {
				file.seekg(read_pos);
				DeviceScriptVariant script;

				if (!(file >> script)) {

					read_pos += static_cast<std::streamoff>(1);
					continue;
				}

				std::streampos after_read = file.tellg();
				if (after_read <= read_pos) {
					break;
				}

				bool belongs_to_user = false;
				std::visit([&user, &belongs_to_user](const auto& scr) {
					auto device = scr.getDevice();
					if (device.getPassword() == user.getPassword()) {
						belongs_to_user = true;
					}
					}, script);

				if (belongs_to_user) {

					removed_count++;
					read_pos = after_read;
				}
				else {

					if (write_pos != read_pos) {

						file.seekg(read_pos);
						file.seekp(write_pos);


						std::streamsize block_size = after_read - read_pos;
						std::vector<char> buffer(block_size);

						file.seekg(read_pos);
						file.read(buffer.data(), block_size);

						file.seekp(write_pos);
						file.write(buffer.data(), block_size);
					}


					write_pos += (after_read - read_pos);
					read_pos = after_read;
				}
			}

			file.close();

			if (removed_count == 0) {
				cout << "Сценарии пользователя " << user.getUserName() << " не найдены\n";
				return;
			}

			if (write_pos < fileSize) {
				std::filesystem::resize_file(file_name, write_pos);
			}

			cout << "Удалено сценариев пользователя " << user.getUserName() << ": " << removed_count << "\n";
		}

		std::queue<DeviceScriptVariant> FileSystem<DeviceScriptVariant>::unloadScripts(const User& user)
		{
			std::queue<DeviceScriptVariant> user_scripts;

			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				cout << "Файл сценариев не найден\n";
				return user_scripts;
			}

			smartFile.seekg(0, ios::end);
			if (smartFile.tellg() == 0) {
				cout << "Файл сценариев пуст\n";
				smartFile.close();
				return user_scripts;
			}
			smartFile.seekg(0, ios::beg);

			cout << "Выгрузка сценариев пользователя " << user.getUserName() << "...\n";
			int loaded_count = 0;

			while (!smartFile.eof()) {
				DeviceScriptVariant script;
				smartFile >> script;

				if (smartFile.good()) {
	
					bool belongs_to_user = false;
					std::visit([&user, &belongs_to_user](const auto& scr) {
						auto device = scr.getDevice();
						if (device.getPassword() == user.getPassword())
						{
							belongs_to_user = true;
						}
						}, script);

					if (belongs_to_user) {
						user_scripts.push(script);
						loaded_count++;
					}
				}
				else {
					smartFile.clear();
					smartFile.ignore(1024, '\n');
				}

				if (smartFile.eof()) break;
			}
			if (!user_scripts.empty())
			{
				smartFile.close();
				removeF(user);
				return user_scripts;
			}
			smartFile.close();
			cout << "Выгружено сценариев: " << loaded_count << "\n";
			return user_scripts;
		}

		//SmartHomeInteraction

		shared_ptr<User> SmartHomeInteraction::current_user = nullptr;
		std::queue<DeviceScriptVariant> SmartHomeInteraction::script_subsequence;
		FileSystem<DeviceVariant> SmartHomeInteraction::device_file;
		FileSystem<DeviceScriptVariant> SmartHomeInteraction::script_file;
		FileSystem<User> SmartHomeInteraction::user_file;
		bool SmartHomeInteraction::admin_action = false;

		void SmartHomeInteraction::removeScript(const DeviceVariant& target_device)
		{
			if (script_subsequence.empty()) {
				cout << "Очередь сценариев пуста\n";
				return;
			}

			std::queue<DeviceScriptVariant> temp_queue;
			int removed_count = 0;

			while (!script_subsequence.empty()) {
				DeviceScriptVariant script = script_subsequence.front();
				script_subsequence.pop();

				bool should_remove = std::visit([&](const auto& script_obj) -> bool {
					auto device_in_script = script_obj.getDevice();
					return device_in_script == target_device;
					}, script);

				if (!should_remove) {
					temp_queue.push(script);
				}
				else {
					removed_count++;
				}
			}

			script_subsequence = temp_queue;

			if (removed_count > 0) {
				cout << "Удалено сценариев для указанного устройства: " << removed_count << "\n";
			}
			else {
				cout << "Сценарии для указанного устройства не найдены\n";
			}
		}

		int SmartHomeInteraction::printScripts()
		{
			if (script_subsequence.empty()) {
				cout << "Очередь сценариев пуста.\n";
				return 0;
			}

			cout << "Очередь сценариев\n";
			cout << "Всего сценариев: " << script_subsequence.size() << "\n\n";

			std::queue<DeviceScriptVariant> temp_queue = script_subsequence;
			int counter = 0;

			while (!temp_queue.empty()) {
				cout<< string(17, '-') << "Сценарий " << ++counter << string(17, '-') << "\n";
				cout << temp_queue.front() << "\n";
				temp_queue.pop();
			}

			return counter;
		}

		void SmartHomeInteraction::editScript()
		{

			int script_count = printScripts();

			if (script_count == 0)
			{
				cout << "Нет сценариев для редактирования\n";
				return;
			}

			std::vector<DeviceScriptVariant> scripts_vector;
			std::queue<DeviceScriptVariant> temp_queue = script_subsequence;

			while (!temp_queue.empty())
			{
				scripts_vector.push_back(temp_queue.front());
				temp_queue.pop();
			}

			int choice;
			cout << "\nВыберите сценарий для редактирования (1-" << script_count << "): ";
			while (true) {
				cin >> choice;
				if (cin.fail() || choice < 1 || choice > script_count) {
					cin.clear();
					cin.ignore((numeric_limits<streamsize>::max)(), '\n');
					cout << "Ошибка ввода! Введите число от 1 до " << script_count << ": ";
				}
				else {
					cin.ignore((numeric_limits<streamsize>::max)(), '\n');
					break;
				}
			}

			DeviceScriptVariant& selected_script = scripts_vector[choice - 1];

			cout << "\nРЕДАКТИРОВАНИЕ СЦЕНАРИЯ\n";
			cout << "Текущие данные сценария:\n";

			std::visit([](const auto& script) {
				cout << script << "\n";
				}, selected_script);

			cout << "\nВведите новое описание сценария: ";
			string new_script;
			getline(cin,new_script);

			cout << "Введите новую длительность (в минутах): ";
			short new_duration;
			cin >> new_duration;
			cin.ignore();

			if (new_duration <= 0)
			{
				cout << "Длительность должна быть положительным числом. Изменения отменены.\n";
				return;
			}


			std::visit([&new_script, new_duration](auto& script) {
				script.setScript(new_script);
				script.setDuration(new_duration);
				}, selected_script);

			std::queue<DeviceScriptVariant> updated_queue;
			for (const auto& script : scripts_vector)
			{
				updated_queue.push(script);
			}

			script_subsequence = updated_queue;


			cout << "Сценарий успешно отредактирован!\n";
		}

		DeviceVariant SmartHomeInteraction::chooseDevice()
		{
			bool is_running = true;
			while (is_running)
			{
				showDeviceCatalogHeaderMenu();
				std::cout << "Выберите тип устройства:" << std::endl;
				short choice;
				while (true) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				switch (choice)
				{
				case 0:
				{
					SmartLight light;
					return light;
				}
				case 1:
				{
					Thermostat thermo;
					return thermo;
				}
				case 2:
				{

					SecurityCamera camera;
					return camera;
				}
				default:
				{
					cout << "Вы ввели неверный выбор, попробуйте еще раз." << std::endl;
					break;
				}
				}

			}
		}

		DeviceVariant SmartHomeInteraction::chooseDevice(SmartType type)
		{

			switch (type)
			{
			case Light:
			{
				SmartLight light;
				return light;
			}
			case Thermo:
			{
				Thermostat thermo;
				return thermo;
			}
			case SecCamera:
			{
				SecurityCamera camera;
				return camera;
			}
			default:
				throw runtime_error("Неверный тип устройства");
			}
		}

		void SmartHomeInteraction::showSHOHeaderMenu()
		{
			cout << "Меню SmartHome:" << endl
				<< "-->  1. Добавление устройства на аккаунт." << endl
				<< "-->  2. Редактирование устройства." << endl
				<< "-->  3. Удаление устройства." << endl
				<< "-->  4. Посмотреть информацию о смарт-устройствах." << endl
				<< "-->  5. Сортировка устройств (по названию)." << endl
				<< "-->  6. Поиск устройств (по названию)." << endl
				<< "-->  7. Создание отчета." << endl
				<< "-->  8. Фильтрация устройств (online)." << endl
				<< "-->  9. Фильтрация устройств (по дате)." << endl
				<< "--> 10. Создание сценария." << endl
				<< "--> 11. Удаление сценария." << endl
				<< "--> 12. Редактирование сценария." << endl
				<< "--> 13. Посмотреть информацию о всех сценариях." << endl
				<< "-->  0. Выход..." << endl << ">> ";
		}

		/*void SmartHomeInteraction::generateUserReport()
		{
			string report_filename = current_user->getUserName() + "_smart_home_report.txt";
			ofstream report_file(report_filename, ios::out | ios::trunc);

			if (!report_file.is_open())
				throw runtime_error("Не удалось создать файл отчета: " + report_filename);

			report_file << "=============================================\n";
			report_file << "          ОТЧЕТ УМНОГО ДОМА\n";
			report_file << "=============================================\n\n";


			report_file << "ИНФОРМАЦИЯ О ПОЛЬЗОВАТЕЛЕ:\n";
			report_file << "============================\n";
			report_file << current_user << "\n";

			report_file << "УСТРОЙСТВА (" << devices.size() << "):\n";
			report_file << "============================\n";

			if (device.empty())
			{
				report_file << "Нет подключенных устройств\n";
			}
			else
			{
				int device_number = 1;
				for (const auto& device : devices)
				{
					report_file << "\nУстройство #" << device_number++ << ":\n";
					report_file << device;
					report_file << "----------------------------------------\n";
				}
			}

			report_file << "\nАКТИВНЫЕ СЦЕНАРИИ:\n";
			report_file << "============================\n";

			auto scripts_copy = script_subsequence;
			if (scripts_copy.empty())
			{
				report_file << "Нет активных сценариев\n";
			}
			else
			{
				int script_number = 1;
				while (!scripts_copy.empty())
				{
					report_file << "\nСценарий #" << script_number++ << ":\n";
					report_file << scripts_copy.front();
					report_file << "----------------------------------------\n";
					scripts_copy.pop();
				}
			}

			report_file << "\nСТАТИСТИКА:\n";
			report_file << "============================\n";
			report_file << "Всего устройств: " << devices.size() << "\n";


			int lights_count = 0, thermos_count = 0, cameras_count = 0;
			int online_count = 0;

			for (const auto& device : devices)
			{

				SmartType type = visit([](auto&& arg) {
					return arg.getType();
					}, device);


				switch (type)
				{
				case SmartType::Light:
					lights_count++;
					break;
				case SmartType::Thermo:
					thermos_count++;
					break;
				case SmartType::SecCamera:
					cameras_count++;
					break;
				}


				if (std::visit([](auto&& arg) { return arg.getOnline(); }, device))
				{
					online_count++;
				}
			}

			report_file << "  • Умных ламп: " << lights_count << "\n";
			report_file << "  • Термостатов: " << thermos_count << "\n";
			report_file << "  • Камер безопасности: " << cameras_count << "\n";
			report_file << "Активных сценариев: " << script_subsequence.size() << "\n";

			if (!devices.empty())
			{
				double online_percent = (online_count * 100.0) / devices.size();
				report_file << "Устройств онлайн: " << online_count << " из " << devices.size()
					<< " (" << fixed << setprecision(1) << online_percent << "%)\n";
			}
			else
			{
				report_file << "Устройств онлайн: 0 из 0 (0.0%)\n";
			}

			time_t now = time(0);
			tm* local_time = localtime(&now);
			report_file << "\nОтчет сгенерирован: "
				<< (local_time->tm_year + 1900) << "-"
				<< setw(2) << setfill('0') << (local_time->tm_mon + 1) << "-"
				<< setw(2) << setfill('0') << local_time->tm_mday << " "
				<< setw(2) << setfill('0') << local_time->tm_hour << ":"
				<< setw(2) << setfill('0') << local_time->tm_min << ":"
				<< setw(2) << setfill('0') << local_time->tm_sec << "\n";

			report_file << "=============================================\n";

			report_file.close();
			cout << "Отчет успешно сохранен в файл: " << report_filename << "\n";
		}*/

		void SmartHomeInteraction::generateFullReport()
		{
			string report_filename;
			if(!admin_action)
				report_filename = current_user->getUserName() + "_smart_home_report.txt";
			else
				report_filename = current_user->getUserName() + "_smart_home_admin_report.txt";

			stringstream ss;
			ss << *current_user; 


			ofstream report_file(report_filename, ios::out | ios::trunc);

			report_file << string(40, '=') << "\n";
			report_file << "ОТЧЕТ УМНОГО ДОМА\n";
			report_file << string(40, '=') << "\n";

			report_file << "ИНФОРМАЦИЯ О ПОЛЬЗОВАТЕЛЕ:\n";
			report_file << string(40, '=') << "\n";

			report_file << ss.str();

			report_file.close();

			device_file.generateDeviceReport(report_filename);

			report_file.open(report_filename, ios::out | ios::app);

			report_file << string(40, '=') << "\n";
			report_file << "АКТИВНЫЕ СЦЕНАРИИ:\n";
			report_file << string(40, '=') << "\n";

			auto scripts_copy = script_subsequence;
			if (scripts_copy.empty())
			{
				report_file << "Нет активных сценариев\n";
			}
			else
			{
				int script_number = 1;
				while (!scripts_copy.empty())
				{
					report_file << "\nСценарий #" << script_number++ << ":\n";

					stringstream script_ss;
					script_ss << scripts_copy.front();
					report_file << script_ss.str();

					report_file << string(40, '-') << "\n";
					scripts_copy.pop();
				}

				report_file << "Всего сценариев: " << script_subsequence.size() << "\n";
			}

			time_t now = time(0);
			tm* local_time = localtime(&now);
			report_file << "\nОтчет сгенерирован: "
				<< (local_time->tm_year + 1900) << "-"
				<< setw(2) << setfill('0') << (local_time->tm_mon + 1) << "-"
				<< setw(2) << setfill('0') << local_time->tm_mday << " "
				<< setw(2) << setfill('0') << local_time->tm_hour << ":"
				<< setw(2) << setfill('0') << local_time->tm_min << ":"
				<< setw(2) << setfill('0') << local_time->tm_sec << "\n";

			report_file << string(40, '=') << "\n";

			report_file.close();
			cout << "Отчет создан: " << report_filename << "\n";
		}

		void SmartHomeInteraction::showSmartHomeMenu()
		{
			bool is_running = true;
			short choice;
			while (is_running)
			{
				showSHOHeaderMenu();
				while (is_running) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				switch (choice)
				{
				case 0:
				{
					is_running = false;
					cout << "\n\n";
					break;
				}
				case 1:
				{
					if (admin_action) {
						cout << "На аккаунте другого пользователя, вы можете только редактировать, удалять и создать отчет." << endl;
						break;
					}
					auto device = chooseDevice();
					cin >> device;
					string user_password = current_user->getPassword();

					std::visit([user_password](auto&& dev) {
						dev.setPassword(user_password);
						}, device);
					if (device_file.checkDevice(device))
						cout << "Данное устройство уже существует на вашем аккаунте." << endl;
					else
						device_file.writeF(device);
					break;
				}
				case 2:
				{

					int count = device_file.readF();
					if (!count) {
						cout << "У вас нет устройств для редактирования\n";
						break;
					}
					cout << "Выбор устройства для редактирования (1-" << count << ")." << endl;
					auto old_device = device_file.chooseCertainDevice(count);

					SmartType device_type = std::visit([](auto&& dev) -> SmartType {
						return dev.getType();
						}, old_device);

					auto new_device = chooseDevice(device_type);
					cout << "Введите новые данные для устройства:\n";
					cin >> new_device;

					string user_password = current_user->getPassword();
					std::visit([user_password](auto&& dev) {
						dev.setPassword(user_password);
						}, new_device);
					if (device_file.checkDevice(new_device))
					{
						cout << "Данное устройство уже существует на вашем аккаунте." << endl;
						break;
					}
					device_file.editF(old_device, new_device);

					if (!script_subsequence.empty())
					{
						std::queue<DeviceScriptVariant> updated_queue;
						bool scripts_updated = false;

						while (!script_subsequence.empty())
						{
							DeviceScriptVariant script = script_subsequence.front();
							script_subsequence.pop();


							bool updated = std::visit([&old_device, &new_device](auto& script_obj) -> bool {

								auto device_in_script = script_obj.getDevice();


								if (device_in_script == old_device)
								{
									using ScriptDeviceType = std::decay_t<decltype(device_in_script)>;
									ScriptDeviceType new_device_concrete = std::get<ScriptDeviceType>(new_device);
									script_obj.setDevice(new_device_concrete);
									return true;
								}
								return false;
								}, script);

							if (updated) scripts_updated = true;
							updated_queue.push(script);
						}

						script_subsequence = updated_queue;

						if (scripts_updated)
							cout << "Сценарии обновлены с новым устройством.\n";
						
					}

					break;
				}
				case 3:
				{
					int count = device_file.readF();
					int choice;
					if (!count)
						break;
					while (true) {
						cin >> choice;
						if (cin.fail() || choice < 1 || choice > count) {
							cin.clear();
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							cout << "Ошибка ввода! Введите число от 1 до " << count << ": ";
						}
						else {
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							break;
						}
					}
					auto device = device_file.removeF(choice);
					if (!script_subsequence.empty()) removeScript(device);
					break;
				}
				case 4:
				{

					device_file.readF();
					break;
				}
				case 5:
				{
					if (admin_action) {
						cout << "На аккаунте другого пользователя, вы можете только редактировать, удалять и создать отчет." << endl;
						break;
					}
					device_file.sortF([](const DeviceVariant& a, const DeviceVariant& b) -> bool {
						return std::visit([](const auto& dev_a, const auto& dev_b) -> bool {
							string title_a = dev_a.getTitle();
							string title_b = dev_b.getTitle();
							std::transform(title_a.begin(), title_a.end(), title_a.begin(), ::tolower);
							std::transform(title_b.begin(), title_b.end(), title_b.begin(), ::tolower);
							return title_a < title_b;
							}, a, b);
						});
					break;
				}
				case 6:
				{
					if (admin_action) {
						cout << "На аккаунте другого пользователя, вы можете только редактировать, удалять и создать отчет." << endl;
						break;
					}
					string target_dev_name;
					cout << "Введите название устройства: ";
					getline(cin, target_dev_name);
					device_file.searchF(target_dev_name);
					break;
				}
				case 7:
				{
					generateFullReport();
					break;
				}
				case 8:
				{
					if (admin_action) {
						cout << "На аккаунте другого пользователя, вы можете только редактировать, удалять и создать отчет." << endl;
						break;
					}
					bool isOnline;
					cout << "Устройство онлайн? (0-Нет, 1-Да): ";
					
					while (true) {
						cin >> isOnline;
						if (cin.fail()) {
							cin.clear();
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							cout << "Ошибка ввода! Введите 0 или 1: ";
						}
						else {
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							break;
						}
					}
					device_file.filter(
						[isOnline](const DeviceVariant& device) -> bool {
							bool device_online = std::visit([](const auto& dev) -> bool {
								return dev.getOnline();
								}, device);
							return isOnline == device_online;
						},
						"Статус: " + std::string(isOnline ? "онлайн" : "офлайн")
					);
					break;
				}
				case 9:
				{
					if (admin_action) {
						cout << "На аккаунте другого пользователя, вы можете только редактировать, удалять и создать отчет." << endl;
						break;
					}
					Date first_date, second_date;
					cout << "Введите первую дату:" << endl;
					cin >> first_date;
					cout << "Введите вторую дату:" << endl;
					cin >> second_date;
					if (second_date < first_date || second_date == first_date)
					{
						cout << "Вторая дата не может быть раньше или равна первой." << endl;
						break;
					}
					device_file.filter(
						[first_date, second_date](const DeviceVariant& device) -> bool {
							Date purchase_date = std::visit([](const auto& dev) -> Date {
								return dev.getPurchaseDate();
								}, device);
							return !(purchase_date < first_date) && !(second_date < purchase_date);
						},
						"Период с " +
						std::to_string(first_date.getYear()) + "-" +
						std::to_string(first_date.getMonth()) + "-" +
						std::to_string(first_date.getDay()) + " по " +
						std::to_string(second_date.getYear()) + "-" +
						std::to_string(second_date.getMonth()) + "-" +
						std::to_string(second_date.getDay())
					);
					break;
				}
				case 10:
				{
					if (admin_action) {
						cout << "На аккаунте другого пользователя, вы можете только редактировать, удалять и создать отчет." << endl;
						break;
					}
					int count = device_file.readF();
					if (!count)
						cout << "К сожалению вы не можете добавить сценарий." << std::endl;
					else
					{
						auto device_variant = device_file.chooseCertainDevice(count);

						cout << "Введите описание сценария: ";
						string script_desc;
						getline(cin, script_desc);


						cout << "Введите длительность сценария (в минутах): ";
						short duration;
						while (true) {
							cin >> duration;
							if (cin.fail() || duration < 0) {
								cin.clear();
								cin.ignore((numeric_limits<streamsize>::max)(), '\n');
								cout << "Ошибка ввода! Введите число: ";
							}
							else {
								cin.ignore((numeric_limits<streamsize>::max)(), '\n');
								break;
							}
						}

						DeviceScriptVariant dev_script = std::visit([&script_desc, &duration](const auto& concrete_device) -> DeviceScriptVariant {
							return DeviceScript<std::decay_t<decltype(concrete_device)>>(concrete_device,script_desc,duration);
							}, device_variant);


						SmartHomeInteraction::script_subsequence.push(dev_script);
						cout << "Сценарий успешно добавлен!\n";
					}

					break;
				}
				case 11:
				{
					if (script_subsequence.empty()) cout << "На вашем аккаунте нет сценариев." << endl;
					else 
					{
						int count = device_file.readF();
						auto device_variant = device_file.chooseCertainDevice(count);
						removeScript(device_variant);
					}
					break;
				}
				case 12:
				{
					editScript();
					break;
				}
				case 13:
				{
					printScripts();
					break;
				}
				default:
				{
					cout << "Вы ввели неверный выбор. Попробуйте еще раз." << endl;
					break;
				}
				}
			}
		}

		bool SmartHomeInteraction::showAccountMenu()
		{
			bool is_running = true;
			int choice;
			while (is_running)
			{
				cout << "Меню учетной записи:" << endl
					<< "--> 1.Информация об аккаунте." << endl
					<< "--> 2.Удалить учетную запись." << endl
					<< "--> 0.Выход..." << endl << ">> ";

				while (is_running) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				switch (choice) {
				case 1:
				{
					cout << *current_user << endl;
					string temp_password;
					cout << "Для просмотра пароля, введите пароль: ";
					getline(cin, temp_password);
					string hashed_temp_password = hashPassword(current_user->getUserName(),temp_password);
					if (hashed_temp_password == current_user->getPassword())
						cout << "Пароль: " << temp_password << endl;
					else
						cout << "Пароль не совпадает." << endl;
					break;
				}
				case 2:
				{
					if (!admin_action)
					{
						string temp_password;
						cout << "Для удаления учетной записи, введите пароль: ";
						getline(cin, temp_password);
						string hashed_temp_password = hashPassword(current_user->getUserName(), temp_password);
						if (hashed_temp_password != current_user->getPassword())
						{
							cout << "Пароль учетной записи и введенный пароли не совпадают." << endl;
							break;

						}
					}
						if (std::filesystem::remove(device_file.getFileName()))
						{
							std::cout << "Файл " << device_file.getFileName() << " успешно удален\n";
						}
						else
						{
							std::cout << "Файл " << device_file.getFileName() << " не найден или не может быть удален\n";
						}

						user_file.removeF(*current_user);
						return true;

					break;
				}
				case 0:
				{
					is_running = false;
					break;
				}
				default:
				{
					cout << "Неверное значение выбора, попробуйте ещё раз." << endl;
					break;
				}
				}

			}
			return 0;
		}

		void SmartHomeInteraction::showAdminOperationsMenu()
		{
			bool is_running = true;
			int choice;
			while (is_running)
			{
				cout << "Меню учетной записи:" << endl
					<< "--> 1.Информация всех зарегистрированных аккаунтов." << endl
					<< "--> 2.Войти в качестве User." << endl
					<< "--> 0.Выход..." << endl << ">> ";

				while (is_running) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				switch (choice) {
				case 1:
				{
					user_file.readF_a();
					break;
				}
				case 2:
				{
					string name;
					cout << "Введите имя аккаунта: ";
					getline(cin, name);
					User temp_user = user_file.chooseUser_a(name);
					if (temp_user == User()) break;
					*current_user = temp_user;
					admin_action = true;
					showMainMenu();
					admin_action = false;
					break;
				}
				case 0:
				{
					is_running = false;
					break;
				}
				default:
				{
					cout << "Неверное значение выбора, попробуйте ещё раз." << endl;
					break;
				}
				}

			}
		}

		void SmartHomeInteraction::showRegistrationMenu()
		{

			if (user_file.checkUser(*current_user) == 1)
			{
				cout << "Данная учетная запись уже существует, вход совершается автоматически." << endl;
			}
			else if (user_file.checkUser(*current_user) == 0)
			{
				user_file.writeF(*current_user);
				cout << "Пользователь зарегистрирован. Добро пожаловать в систему Smart Home!" << endl;
			}
			else 
			{
				cout << "Пользователь уже существует с таким же именем." << std::endl;
				return;
			}
			
			if (current_user->getRole() == Admin_)
			{
				User admin = *current_user;
				short choice;
				bool is_running = true;
				while (is_running)
				{
					*current_user = admin;
					cout << "Дальнейшие действия:" << endl
						<< "1.Админские операции." << endl
						<< "2.Главное меню." << endl
						<< "0.Выход." << endl
						<< "Выберите соответсвующее действие: ";
					while (true) {
						cin >> choice;
						if (cin.fail()) {
							cin.clear();
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							cout << "Ошибка ввода! Введите число: ";
						}
						else {
							cin.ignore((numeric_limits<streamsize>::max)(), '\n');
							break;
						}
					}
					switch (choice)
					{
					case 1:
					{
						showAdminOperationsMenu();
						break;
					}
					case 2:
					{
						showMainMenu();
						break;
					}
					case 0:
					{
						is_running = false;
						break;
					}
					}
				}
			}
			else
			showMainMenu();
		}

		void SmartHomeInteraction::showLoginMenu()
		{

				if (user_file.checkUser(*current_user) == 1) {
					cout << "Вход выполнен успешно! Добро пожаловать!" << endl;
				}
				else if(user_file.checkUser(*current_user) == 0)
				{
					cout << "Пользователь не найден. Проверьте логин и пароль." << endl;
					return;
				}

				if (current_user->getRole() == Admin_)
				{
					User admin = *current_user;
					short choice;
					bool is_running = true;
					while (is_running)
					{
						*current_user = admin;
						cout << "Дальнейшие действия:" << endl
							<< "1.Админские операции." << endl
							<< "2.Главное меню." << endl
							<< "0.Выход." << endl
							<< "Выберите соответсвующее действие: ";
						while (true) {
							cin >> choice;
							if (cin.fail()) {
								cin.clear();
								cin.ignore((numeric_limits<streamsize>::max)(), '\n');
								cout << "Ошибка ввода! Введите число: ";
							}
							else {
								cin.ignore((numeric_limits<streamsize>::max)(), '\n');
								break;
							}
						}
						switch (choice)
						{
						case 1:
						{
							showAdminOperationsMenu();
							break;
						}
						case 2:
						{
							showMainMenu();
							break;
						}
						case 0:
						{
							is_running = false;
							break;
						}
						}
					}
				}
				else
					showMainMenu();
		}

		void SmartHomeInteraction::showRoleHeaderMenu()
		{
			cout << "Меню выбора роли:" << endl
				<< "--> 1.User." << endl
				<< "--> 2.Admin." << endl
				<< "--> 3.Guest." << endl
				<< "--> 0.Выход..." << endl
				<< "Выберите соответствующую роль" << std::endl << ">> ";
		}

		void SmartHomeInteraction::showUserMenu()
		{
			showAuthorMenu();
		}

		void SmartHomeInteraction::showAdminMenu()
		{
			 std::ifstream file("for_admins.txt", std::ios::in);
			if (!file.is_open()) {
				std::cout << "Ошибка открытия файла с паролем";
				return;
			}  
			//std::string admin_password;
			//file >> admin_password;
			std::string admin_password;
			std::getline(file, admin_password);
			std::string current_password;
			while (true) {
				cout << "Введите админский пароль: " << std::endl << ">>";
				cin >> current_password;
				if (admin_password == current_password) {
					std::cout << "Пароль подтверждён. Продолжаем.\n";
					break;
				}
				else 
				{
					cout << "Вы ввели неверный пароль..." << std::endl;
					return;
				}
			}

			showAuthorMenu();
		}

		void SmartHomeInteraction::showGuestMenu()
		{
			bool is_running = true;
			short choice;
			while (is_running)
			{
				cout << "Гостевое Меню" << endl
					<< "1. Регистрация" << endl
					<< "2. Просмотр перечня смарт-устройств" << endl
					<< "0. Выход..." << endl << ">>";
				while (is_running) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				switch (choice) {
				case 0:
				{
					is_running = false;
					cout << "\n\n";
					break;
				}
				case 1:
				{
					showAuthorMenu();
					auto user = make_shared<User>();
					user->setRole(Guest_);
					current_user = user;
					break;
				}
				case 2:
				{
					showDeviceCatalogHeaderMenu();
					break;
				}
				
				default:
				{
					cout << "Был введен неверный выбор. Попробуйте снова." << std::endl;
					break;
				}
				}
			}
		}

		void SmartHomeInteraction::showMainMenu()
		{
			bool is_running = true;
			short choice;
			device_file.setFileName(current_user->getUserName());
			script_subsequence = script_file.unloadScripts(*current_user);
			cout << "\n";
			while (is_running)
			{

				cout << "Главное меню:" << endl
					<< "--> 1.Умный дом." << endl
					<< "--> 2.Учётная запись." << endl
					<< "--> 0.Выход..." << endl << ">> ";
				while (is_running) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				switch (choice)
				{
				case 0:
				{
					cout << "Автоматическая запись последовательности девайсов в файл." << endl;
					script_file.writeF(script_subsequence);
					is_running = false;
					cout << "\n\n";
					break;
				}
				case 1:
				{
					showSmartHomeMenu();
					break;
				}
				case 2:
				{
					if (showAccountMenu())
						return;
					break;
				}
				default:
				{
					cout << "Вы ввели неверный выбор." << endl;
					break;
				}
				}
			
			}
		}

		void SmartHomeInteraction::showAuthorMenu()
		{
			bool is_running = true;
			short choice;
			UserRole temp_role = current_user->getRole();
			while (is_running)
			{
				cout << "Меню учетной записи:" << endl
					<< "--> 1. Создать учётную запись." << endl
					<< ((current_user->getRole() != Guest_) ? "--> 2. Войти в учётную запись.\n" : "")
					<< "--> 0.Выход..." << endl << ">> ";

				while (true) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}

				switch (choice)
				{
				case 0:
				{
					is_running = false;
					current_user = nullptr;
					cout << "\n\n";
					break;
				}
				case 1:
				{
					if (current_user->getRole() == Guest_)
						current_user->setRole(User_);

					cin >> *current_user;
					showRegistrationMenu();
					auto user = make_shared<User>();
					user->setRole(temp_role);
					current_user = user;
					break;
				}
				case 2:
				{
					//cout << *current_user << endl;
					if (current_user->getRole() == Guest_){}
					else {
						string attempt_name;
						string attempt_password;
						cout << "Введите имя пользователя: ";
						getline(cin, attempt_name);

						attempt_password = getPasswordWithDots();
						
						current_user->setUserName(attempt_name);
						attempt_password = hashPassword(current_user->getUserName(),attempt_password);
						current_user->setPassword(attempt_password);

						showLoginMenu();
						auto user = make_shared<User>();
						user->setRole(temp_role);
						current_user = user;
						break;
					}
				}
				default:
				{
					cout << "Вы ввели неверный выбор." << endl;
					break;
				}
				}
			}
		}

		void SmartHomeInteraction::showDeviceCatalogHeaderMenu()
		{
			cout << "Каталог смарт устройств" << endl
				<< "0. Smart Light" << endl
				<< "1. Thermostat" << endl
				<< "2. Security Camera" << endl;
		}

		void SmartHomeInteraction::showEntryMenu()
		{
			bool is_running = true;
			short choice;
			while (is_running)
			{
				showRoleHeaderMenu();

				while (is_running) {
					cin >> choice;
					if (cin.fail()) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				switch (choice)
				{
				case 0:
				{
					is_running = false;
					break;
				}
				case 1:
				{
					auto user = make_shared<User>();
					user->setRole(User_);
					current_user = user;
					showUserMenu();
					current_user = nullptr;
					break;
				}
				case 2:
				{
					auto user = make_shared<User>();
					user->setRole(Admin_);
					current_user = user;
					showAdminMenu();
					current_user = nullptr;
					break;
				}
				case 3:
				{
					auto user = make_shared<User>();
					user->setRole(Guest_);
					current_user = user;
					showGuestMenu();
					current_user = nullptr;
					break;
				}
				default:
				{
					cout << "Вы выбрали несуществующий пункт меню." << endl;
					break;
				}
				}
			}
		}
		
}

	
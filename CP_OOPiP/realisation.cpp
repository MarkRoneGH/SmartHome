	#include "prototype.h"
	#pragma warning(disable:4996)
	namespace smart_system
	{
		//SmartSmth
		SmartSmth::SmartSmth() 
		{
			memset(title, '\0', SIZE_BUFF); 
			memset(owner_password, '\0', SIZE_BUFF);
			anyType = Light;
			isOnline = false;
			purchase_date = Date();
		}
		bool SmartSmth::getOnline() const { return isOnline; }
		string SmartSmth::getTitle() const { return string(title); }
		SmartType SmartSmth::getType() const { return anyType; }
		void SmartSmth::setOnline(const bool online) { isOnline = online; }
		void SmartSmth::setPurchaseDate(const Date& purchase_date)
		{
			this->purchase_date = purchase_date;
		}
		Date SmartSmth::getPurchaseDate() const { return purchase_date; }
		void SmartSmth::setTitle(const string& title)
		{
			if (title.empty() || title.length() >= SIZE_BUFF) {
				//throw std::invalid_argument("Неверное значение название smart-девайса.");
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

				out << device.purchase_date;
			}
			else {

				out << "Device Title: " << device.title << "\n";
				out << "Password: " << device.owner_password << "\n";
				out << "Type: ";
				switch (device.anyType) {
				case SmartType::Light: out << "Light"; break;
				case SmartType::Thermo: out << "Thermostat"; break;
				case SmartType::SecCamera: out << "Security Camera"; break;
				}
				out << "\n";
				out << "Online: " << (device.isOnline ? "Yes" : "No") << "\n";
				out << "Purchase Date: " << device.purchase_date << "\n";
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

				in >> device.purchase_date;
			}
			else {

				cout << "Введите название устройства: ";
				in.getline(device.title, SIZE_BUFF);

				//cout << "Введите пароль владельца: ";
				//in.getline(device.owner_password, SIZE_BUFF);

				/*cout << "Введите тип устройства (0-Свет, 1-Термостат, 2-Камера): ";
				int type;
				while (true) {
					in >> type;
					if (cin.fail() || type < 0 || type > 2) {
						cin.clear();
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						cout << "Ошибка ввода! Введите число от 0 до 2: ";
					}
					else {
						cin.ignore((numeric_limits<streamsize>::max)(), '\n');
						break;
					}
				}
				device.anyType = static_cast<SmartType>(type);*/

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
				in >> device.purchase_date;
			}
			return in;
		}

		bool operator==(const SmartSmth& lhs, const SmartSmth& rhs)
		{
			return strcmp(lhs.title, rhs.title) == 0 &&
				strcmp(lhs.owner_password, rhs.owner_password) == 0 &&
				lhs.anyType == rhs.anyType &&
				lhs.isOnline == rhs.isOnline &&
				lhs.purchase_date == rhs.purchase_date;
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

		bool Date::isLeapYear(const unsigned short year) const
		{
			if (year_ % 400 == 0)
				return true;
			else if (year_ % 100 == 0)
				return false;
			else if (year_ % 4 == 0)
				return true;
			else
				return false;
		}

		bool Date::validDate(const unsigned short year, const short month, const short day) const
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

					if (date.validDate(date.year_, date.month_, date.day_)) {
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
				out << "Country: " << location.country
					<< ", City: " << location.city
					<< ", Street: " << location.street;
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
				// Это консольный ввод
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
			setPassword(password);
		}

		UserLocation User::getLocation() const { return location_; }
		string User::getPassword() const { return string(hashed_password); }
		UserRole User::getRole() const { return anyRole; }
		Date User::getUserDate() const { return date_; }

		string User::hashPassword(const string& password)
		{
			return "salted_" + password + "_" + std::to_string(password.size() * 12345);
		}

		void User::setRole(const UserRole& role) { anyRole = role; }
		void User::setUserDate(const Date& date) { date_ = date; }
		void User::setUserLocation(const UserLocation& location) { location_ = location; }

		void User::setPassword(const string& password)
		{
			string hashed = hashPassword(password);
			strncpy_s(this->hashed_password, hashed.c_str(), SIZE_BUFF - 1);
			this->hashed_password[SIZE_BUFF - 1] = '\0';
		}

		void User::setUserName(const string& user_name)
		{
			if (user_name.empty() || user_name.length() >= SIZE_BUFF) {
	/*			throw std::invalid_argument("Неверное значение название smart-девайса.");*/
			}
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

				out << "Username: " << user.user_name << "\n";
				out << "Password(Hashed): " << user.hashed_password << "\n";
				out << "Location: " << user.location_ << "\n";
				out << "Birth Date: " << user.date_ << "\n";
				out << "Role: ";
				switch (user.anyRole) {
				case UserRole::Admin_: out << "Admin"; break;
				case UserRole::User_: out << "User"; break;
				case UserRole::Guest_: out << "Guest"; break;
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

				cout << "Введите пароль: ";
				string temp_password;
				getline(in, temp_password);
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

		int SmartLight::getBright() const { return brightness_; }

		string SmartLight::getColor() const { return string(color); }

		void SmartLight::setBrightness(const int brightness)
		{
			if (brightness_ < 0 || brightness_ > 100)

				//throw std::invalid_argument("Brightness must be between 0 and 100");

			brightness_ = brightness_;
		}

		void SmartLight::setColor(const string& newColor) {
			if (newColor.empty() || newColor.length() >= SIZE_BUFF) {
				//throw std::invalid_argument("Invalid color value");
			}
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


				out << "Brightness: " << light.brightness_ << "%\n";
				out << "Color: " << light.color << "\n";
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

			return lhs.getBright() == rhs.getBright() &&
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
				//throw std::invalid_argument("Temperature must be between -50°C and 50°C");
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
			if (newMode.empty() || newMode.length() >= SIZE_BUFF) {
	/*			throw std::invalid_argument("Неверное значение режима.");*/
			}
			strncpy_s(mode, newMode.c_str(), SIZE_BUFF - 1);
		}

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

				
				out << "Current Temperature: " << thermostat.currentTemperature << "°C\n";
				out << "Target Temperature: " << thermostat.targetTemperature << "°C\n";
				out << "Mode: " << thermostat.mode << "\n";
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
			: SmartSmth(owner_password, title, SecCamera, online, purchase_date), isRecording(false), motionDetectionEnabled(true) {
			setResolution(resolution);
		}

		int SecurityCamera::getResolution() const { return resolution; }
		bool SecurityCamera::getMotion() const { return motionDetectionEnabled; }
		bool SecurityCamera::getRecording() const { return isRecording; }


		void SecurityCamera::setResolution(const int resolution)
		{
			//if (resolution != 720 && resolution != 1080 && resolution != 1440 && resolution != 2160)
				//throw std::invalid_argument("Resolution must be 720, 1080, 1440, or 2160");
			this->resolution = resolution;
		}

		void SecurityCamera::startRecording()
		{
			if (isRecording) {
				//throw std::runtime_error("Camera is already recording");
			}
			isRecording = true;
		}

		void SecurityCamera::stopRecording()
		{
			if (!isRecording) {
				//throw std::runtime_error("Camera is not recording");
			}
			isRecording = false;
		}

		void SecurityCamera::toggleMotionDetection()
		{
			motionDetectionEnabled = !motionDetectionEnabled;
			if (!motionDetectionEnabled && isRecording) {
				stopRecording();
			}
		}

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

				
				out << "Recording: " << (camera.isRecording ? "Yes" : "No") << "\n";
				out << "Resolution: " << camera.resolution << "p\n";
				out << "Motion Detection: " << (camera.motionDetectionEnabled ? "Enabled" : "Disabled") << "\n";
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
			/*else {

				cout << "Select device type:\n";
				cout << "1. Smart Light\n";
				cout << "2. Thermostat\n";
				cout << "3. Security Camera\n";
				cout << "Your choice: ";

				int choice;
				in >> choice;

				switch (choice) {
				case 1: {
					SmartLight light;
					cout << "Enter Smart Light details:\n";
					in >> light;
					variant = light;
					break;
				}
				case 2: {
					Thermostat thermo;
					cout << "Enter Thermostat details:\n";
					in >> thermo;
					variant = thermo;
					break;
				}
				case 3: {
					SecurityCamera camera;
					cout << "Enter Security Camera details:\n";
					in >> camera;
					variant = camera;
					break;
				}
				default: {
					cout << "Invalid choice, creating Smart Light by default\n";
					SmartLight default_device;
					in >> default_device;
					variant = default_device;
					break;
				}
				}
			}
			return in;*/
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
						out << "[SmartLight Script]\n";
					}
					else if constexpr (std::is_same_v<T, DeviceScript<Thermostat>>) {
						out << "[Thermostat Script]\n";
					}
					else if constexpr (std::is_same_v<T, DeviceScript<SecurityCamera>>) {
						out << "[SecurityCamera Script]\n";
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
			else {
				cout << "Select device type for script:\n";
				cout << "1. Smart Light\n";
				cout << "2. Thermostat\n";
				cout << "3. Security Camera\n";
				cout << "Your choice: ";

				int choice;
				in >> choice;

				switch (choice) {
				case 1: {
					DeviceScript<SmartLight> light_script;
					cout << "Enter Smart Light script details:\n";
					in >> light_script;
					variant = light_script;
					break;
				}
				case 2: {
					DeviceScript<Thermostat> thermo_script;
					cout << "Enter Thermostat script details:\n";
					in >> thermo_script;
					variant = thermo_script;
					break;
				}
				case 3: {
					DeviceScript<SecurityCamera> camera_script;
					cout << "Enter Security Camera script details:\n";
					in >> camera_script;
					variant = camera_script;
					break;
				}
				default: {
					cout << "Invalid choice, creating Smart Light script by default\n";
					DeviceScript<SmartLight> default_script;
					in >> default_script;
					variant = default_script;
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

		bool FileSystem<User>::isUserExisting(User& user_to_check)
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
				cout << temp_user;
				if (user_to_check.getUserDate() != Date() && user_to_check.getLocation() != UserLocation())
				{
					if (temp_user == user_to_check)
					{
						smartFile.close();
						return true;
					}
				}
				else if (temp_user.getPassword() == user_to_check.getPassword() && temp_user.getUserName() == user_to_check.getUserName()) 
				{
					user_to_check = temp_user;
					smartFile.close();
					return true;
				}
			}

			if (smartFile.fail() && !smartFile.eof()) {
				smartFile.close();
				throw std::runtime_error("Ошибка чтения файла пользователей");
			}

			smartFile.close();
			return false;
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

		//FileSystem<DeviceVariant>

		void FileSystem<DeviceVariant>::readF()
		{

			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				cout << "Файл устройств не найден или пуст\n";
				return;
			}

			smartFile.seekg(0, ios::end);
			if (smartFile.tellg() == 0) {
				cout << "У вас пока нет устройств\n";
				smartFile.close();
				return;
			}
			smartFile.seekg(0, ios::beg);

			cout << "=== Ваши устройства ===\n";
			int device_count = 0;

			while (!smartFile.eof()) {
				DeviceVariant device;
				smartFile >> device;

				if (smartFile.good()) {
					cout << "Устройство " << ++device_count << ":\n";
					cout << device;
					cout << "------------------------\n";
				}
				else {
					if (!smartFile.eof()) {
						smartFile.clear();
						smartFile.ignore(1024, '\n');
					}
				}

				if (smartFile.eof()) break;
			}

			if (device_count == 0) {
				cout << "У вас нет устройств\n";
			}
			else {
				cout << "Всего устройств: " << device_count << "\n";
			}

			smartFile.close();
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
			}/*

			std::visit([this](const auto& dev) {
				using T = std::decay_t<decltype(dev)>;

				if constexpr (std::is_same_v<T, SmartLight>) {
					smartFile << dev;
				}
				else if constexpr (std::is_same_v<T, Thermostat>) {
					smartFile << dev;
				}
				else if constexpr (std::is_same_v<T, SecurityCamera>) {
					smartFile << dev;
				}
				}, device);*/

			smartFile << device;

			if (smartFile.fail())
			{
				smartFile.close();
				throw std::runtime_error("Ошибка записи устройства в файл");
			}

			smartFile.close();
		}

		void FileSystem<DeviceVariant>::searchF(const User& user, const string& dev_name)
		{
			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				cout << "Файл устройств не найден\n";
				return;
			}

			cout << "=== Результаты поиска: '" << dev_name << "' ===\n";
			int found_count = 0;

			while (!smartFile.eof()) {
				bool device_checked = false;

				SmartLight light;
				streampos pos = smartFile.tellg();
				smartFile >> light;
				if (smartFile.good() && light.getPassword() == user.getPassword()) {
					if (light.getTitle().find(dev_name) != string::npos) {
						cout << "Найденное устройство " << ++found_count << ":\n";
						cout << light;
						cout << "------------------------\n";
					}
					device_checked = true;
				}

				if (!device_checked) {
					smartFile.clear();
					smartFile.seekg(pos);
					Thermostat thermo;
					smartFile >> thermo;
					if (smartFile.good() && thermo.getPassword() == user.getPassword()) {
						if (thermo.getTitle().find(dev_name) != string::npos) {
							cout << "Найденное устройство " << ++found_count << ":\n";
							cout << thermo;
							cout << "------------------------\n";
						}
						device_checked = true;
					}
				}

				if (!device_checked) {
					smartFile.clear();
					smartFile.seekg(pos);
					SecurityCamera camera;
					smartFile >> camera;
					if (smartFile.good() && camera.getPassword() == user.getPassword()) {
						if (camera.getTitle().find(dev_name) != string::npos) {
							cout << "Найденное устройство " << ++found_count << ":\n";
							cout << camera;
							cout << "------------------------\n";
						}
						device_checked = true;
					}
				}

				if (!device_checked) {
					smartFile.clear();
					smartFile.ignore(1024, '\n');
				}

				if (smartFile.eof()) break;
			}

			if (found_count == 0) {
				cout << "Устройства не найдены\n";
			}

			smartFile.close();
		}

		void FileSystem<DeviceVariant>::filterByOnline(const User& user, bool online)
		{
			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				cout << "Файл устройств не найден\n";
				return;
			}

			cout << "=== Устройства в статусе: " << (online ? "онлайн" : "офлайн") << " ===\n";
			int filtered_count = 0;

			while (!smartFile.eof()) {
				bool device_checked = false;

				SmartLight light;
				streampos pos = smartFile.tellg();
				smartFile >> light;
				if (smartFile.good() && light.getPassword() == user.getPassword()) {
					if (light.getOnline() == online) {
						cout << "Устройство " << ++filtered_count << ":\n";
						cout << light;
						cout << "------------------------\n";
					}
					device_checked = true;
				}

				if (!device_checked) {
					smartFile.clear();
					smartFile.seekg(pos);
					Thermostat thermo;
					smartFile >> thermo;
					if (smartFile.good() && thermo.getPassword() == user.getPassword()) {
						if (thermo.getOnline() == online) {
							cout << "Устройство " << ++filtered_count << ":\n";
							cout << thermo;
							cout << "------------------------\n";
						}
						device_checked = true;
					}
				}

				if (!device_checked) {
					smartFile.clear();
					smartFile.seekg(pos);
					SecurityCamera camera;
					smartFile >> camera;
					if (smartFile.good() && camera.getPassword() == user.getPassword()) {
						if (camera.getOnline() == online) {
							cout << "Устройство " << ++filtered_count << ":\n";
							cout << camera;
							cout << "------------------------\n";
						}
						device_checked = true;
					}
				}

				if (!device_checked) {
					smartFile.clear();
					smartFile.ignore(1024, '\n');
				}

				if (smartFile.eof()) break;
			}

			if (filtered_count == 0) {
				cout << "Устройства не найдены\n";
			}

			smartFile.close();
		}

		void FileSystem<DeviceVariant>::filterByDate(const User& user, const Date& first_date, const Date& second_date)
		{
			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				cout << "Файл устройств не найден\n";
				return;
			}

			smartFile.seekg(0, ios::end);
			if (smartFile.tellg() == 0) {
				cout << "Файл устройств пуст\n";
				smartFile.close();
				return;
			}
			smartFile.seekg(0, ios::beg);

			cout << "=== Устройства купленные в период с " << first_date << " по " << second_date << " ===\n";
			int filtered_count = 0;

			auto isDateInRange = [&](const Date& date) -> bool {

				if (date.getYear() < first_date.getYear() || date.getYear() > second_date.getYear()) {
					return false;
				}

				if (date.getYear() == first_date.getYear()) {
					if (date.getMonth() < first_date.getMonth()) return false;
					if (date.getMonth() == first_date.getMonth() && date.getDay() < first_date.getDay()) return false;
				}

				if (date.getYear() == second_date.getYear()) {
					if (date.getMonth() > second_date.getMonth()) return false;
					if (date.getMonth() == second_date.getMonth() && date.getDay() > second_date.getDay()) return false;
				}

				return true;
				};

			while (!smartFile.eof()) {
				bool device_checked = false;


				SmartLight light;
				streampos pos = smartFile.tellg();
				smartFile >> light;
				if (smartFile.good() && light.getPassword() == user.getPassword()) {
					Date purchase_date = light.getPurchaseDate();
					if (isDateInRange(purchase_date)) {
						cout << "Устройство " << ++filtered_count << ":\n";
						cout << light;
						cout << "------------------------\n";
					}
					device_checked = true;
				}

	
				if (!device_checked) {
					smartFile.clear();
					smartFile.seekg(pos);
					Thermostat thermo;
					smartFile >> thermo;
					if (smartFile.good() && thermo.getPassword() == user.getPassword()) {
						Date purchase_date = thermo.getPurchaseDate();
						if (isDateInRange(purchase_date)) {
							cout << "Устройство " << ++filtered_count << ":\n";
							cout << thermo;
							cout << "------------------------\n";
						}
						device_checked = true;
					}
				}


				if (!device_checked) {
					smartFile.clear();
					smartFile.seekg(pos);
					SecurityCamera camera;
					smartFile >> camera;
					if (smartFile.good() && camera.getPassword() == user.getPassword()) {
						Date purchase_date = camera.getPurchaseDate();
						if (isDateInRange(purchase_date)) {
							cout << "Устройство " << ++filtered_count << ":\n";
							cout << camera;
							cout << "------------------------\n";
						}
						device_checked = true;
					}
				}


				if (!device_checked) {
					smartFile.clear();
					smartFile.ignore(1024, '\n');
				}

				if (smartFile.eof()) break;
			}

			if (filtered_count == 0) {
				cout << "Устройства не найдены в указанном диапазоне дат\n";
			}
			else {
				cout << "Найдено устройств: " << filtered_count << "\n";
			}

			smartFile.close();
		}


		//FileSystem<DeviceScriptVariant>

		string FileSystem<DeviceScriptVariant>::getFileName() const { return file_name; }

		FileSystem<DeviceScriptVariant>::FileSystem() { file_name = "device_scripts_storage.dat"; }

		void FileSystem<DeviceScriptVariant>::readF(const User& user)
		{
			smartFile.open(file_name, ios::in | ios::binary);
			if (!smartFile.is_open()) {
				cout << "Файл сценариев не найден или пуст\n";
				return;
			}


			smartFile.seekg(0, ios::end);
			if (smartFile.tellg() == 0) {
				cout << "У вас пока нет сценариев\n";
				smartFile.close();
				return;
			}
			smartFile.seekg(0, ios::beg);

			cout << "=== Ваши сценарии ===\n";
			int script_count = 0;

	
			while (!smartFile.eof()) {

				bool script_found = false;

				DeviceScript<SmartLight> light_script;
				streampos pos = smartFile.tellg();
				smartFile >> light_script;
				if (smartFile.good()) {
					SmartLight device = light_script.getDevice();
					if (device.getPassword() == user.getPassword()) {
						cout << "Сценарий " << ++script_count << ":\n";
						cout << "Описание: " << light_script.getScript() << "\n";
						cout << "Длительность: " << light_script.getDuration() << " минут\n";
						cout << "Устройство: " << device.getTitle() << " (Умный свет)\n";
						cout << "------------------------\n";
						script_found = true;
					}
				}


				if (!script_found) {
					smartFile.clear();
					smartFile.seekg(pos);
					DeviceScript<Thermostat> thermo_script;
					smartFile >> thermo_script;
					if (smartFile.good()) {
						Thermostat device = thermo_script.getDevice();
						if (device.getPassword() == user.getPassword()) {
							cout << "Сценарий " << ++script_count << ":\n";
							cout << "Описание: " << thermo_script.getScript() << "\n";
							cout << "Длительность: " << thermo_script.getDuration() << " минут\n";
							cout << "Устройство: " << device.getTitle() << " (Термостат)\n";
							cout << "------------------------\n";
							script_found = true;
						}
					}
				}

		
				if (!script_found) {
					smartFile.clear();
					smartFile.seekg(pos);
					DeviceScript<SecurityCamera> camera_script;
					smartFile >> camera_script;
					if (smartFile.good()) {
						SecurityCamera device = camera_script.getDevice();
						if (device.getPassword() == user.getPassword()) {
							cout << "Сценарий " << ++script_count << ":\n";
							cout << "Описание: " << camera_script.getScript() << "\n";
							cout << "Длительность: " << camera_script.getDuration() << " минут\n";
							cout << "Устройство: " << device.getTitle() << " (Камера безопасности)\n";
							cout << "------------------------\n";
							script_found = true;
						}
					}
				}

				if (!script_found) {
					smartFile.clear();
					smartFile.ignore(1024, '\n');
				}

				if (smartFile.eof()) break;
			}

			if (script_count == 0) {
				cout << "У вас нет сценариев\n";
			}
			else {
				cout << "Всего сценариев: " << script_count << "\n";
			}

			smartFile.close();
		}

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
				streampos pos = smartFile.tellg();
				smartFile >> script;

				if (smartFile.good()) {
	
					bool belongs_to_user = false;
					std::visit([&user, &belongs_to_user](const auto& scr) {
						auto device = scr.getDevice();
						if (device.getPassword() == user.getPassword()) {
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

			smartFile.close();
			cout << "Выгружено сценариев: " << loaded_count << "\n";
			return user_scripts;
		}

		//SmartHome

		SmartHome::SmartHome():device_file(),script_file(),user_file(),current_user(nullptr){}

		void SmartHome::setCurrentUser(shared_ptr<User> user) { current_user = user; } //&

		shared_ptr<User> SmartHome::getCurrentUser() const { return current_user; }

		bool SmartHome::hasUser() const { return current_user != nullptr; }

		//MenuInterface
	
		SmartHome MenuInteraction::smart_home = SmartHome();

		DeviceVariant MenuInteraction::chooseDevice()
		{
			bool is_running = true;
			while (is_running)
			{
				showDeviceCatalogHeaderMenu();
				std::cout << "" << std::endl;
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

		void MenuInteraction::showSmartHomeMenu()
		{
			bool is_running = true;
			short choice;
			while (is_running)
			{
				cout << "Меню SmartHome:" << endl
					<< "--> 1. Добавление устройства на аккаунт." << endl
					<< "--> 2. Редактирование устройства." << endl
					<< "--> 3. Удаление устройства." << endl
					<< "--> 4. Посмотреть информацию о смарт-устройствах." << endl
					<< "--> 5. Сортировка устройств (по названию)." << endl
					<< "--> 6. Поиск устройств (по названию)." << endl
					<< "--> 7. Создание отчета." << endl
					<< "--> 8. Создание сценария." << endl
					<< "--> 9. Удаление сценария." << endl
					<< "--> 10. Редактирование сценария." << endl
					<< "--> 11. Посмотреть информацию о всех сценариях." << endl
					<< "--> 0. Выход..." << endl << ">>";
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
					auto device = chooseDevice();
					cin >> device;
					string user_password = smart_home.getCurrentUser()->getPassword();

					std::visit([user_password](auto&& dev) {
						using T = std::decay_t<decltype(dev)>;
						if constexpr (std::is_same_v<T, SmartLight> ||
							std::is_same_v<T, Thermostat> ||
							std::is_same_v<T, SecurityCamera>) {
							dev.setPassword(user_password);
						}
						}, device);
					smart_home.device_file.writeF(device);
					break;
				}
				case 4:
				{
					smart_home.device_file.readF();
					break;
				}
				}
			}
		}

		void MenuInteraction::showRegistrationMenu()
		{
			//try {
				if (smart_home.user_file.isUserExisting(*smart_home.getCurrentUser())) {
					cout << "Данная учетная запись уже существует, вход совершается автоматически." << endl;
				}
				else {
					smart_home.user_file.writeF(*smart_home.getCurrentUser());
					cout << "Пользователь зарегистрирован. Добро пожаловать в систему Smart Home!" << endl;
				}
			/*}*/
			/*catch (const std::exception& e) {
				cout << "Ошибка регистрации: " << e.what() << endl;
				cout << "Попробуйте позже." << endl;
				return;
			}*/
			showMainMenu();
		}

		void MenuInteraction::showLoginMenu()
		{
			try {
				if (smart_home.user_file.isUserExisting(*smart_home.getCurrentUser())) {
					cout << "Вход выполнен успешно! Добро пожаловать!" << endl;
				}
				else
				{
					cout << "Пользователь не найден. Проверьте логин и пароль." << endl;
					return;
				}
			}
			catch (const std::exception& e) {
				cout << "Ошибка входа: " << e.what() << endl;
				return;
			}
			showMainMenu();
		}

		void MenuInteraction::showRoleHeaderMenu()
		{
			cout << "Меню выбора роли:" << endl
				<< "--> 1.User." << endl
				<< "--> 2.Admin." << endl
				<< "--> 3.Guest." << endl
				<< "--> 4.Выход..." << endl
				<< "Выберите соответствующую роль" << std::endl << ">>";
		}

		void MenuInteraction::showUserMenu()
		{
			showAuthorMenu();
		}

		void MenuInteraction::showAdminMenu()
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

		void MenuInteraction::showGuestMenu()
		{
			bool is_running = true;
			short choice;
			while (is_running)
			{
				cout << "Гостевое Меню" << endl
					<< "1. Регистрация" << endl
					<< "2. Просмотр перечня смарт-устройств" << endl
					<< "3. Выход..." << endl << ">>";
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
					showAuthorMenu();
					break;
				}
				case 2:
				{
					showDeviceCatalogHeaderMenu();
					break;
				}
				case 3:
				{
					is_running = false;
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

		void MenuInteraction::showMainMenu()
		{
			bool is_running = true;
			short choice;
			smart_home.device_file.setFileName(smart_home.getCurrentUser()->getUserName());
			while (is_running)
			{
				cout << "Главное меню:" << endl
					<< "--> 1.Умный дом." << endl
					<< "--> 2.Учётная запись." << endl
					<< ((smart_home.getCurrentUser()->getRole() == Admin_) ? "--> 3. Админские операции\n" : "")
					<< "--> 0.Выход..." << endl << ">>";
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
					showSmartHomeMenu();
					break;
				}
				case 2:
				{

					break;
				}
				case 3:
				{
					if(smart_home.getCurrentUser()->getRole() == User_)
					{ }
					else {

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

		void MenuInteraction::showAuthorMenu()
		{
			bool is_running = true;
			short choice;
			UserRole temp_role = smart_home.getCurrentUser()->getRole();
			while (is_running)
			{
				cout << "Меню учетной записи:" << endl
					<< "--> 1. Создать учётную запись." << endl
					<< ((smart_home.getCurrentUser()->getRole() == User_ || smart_home.getCurrentUser()->getRole() == Admin_) ? "--> 2. Войти в учётную запись.\n" : "")
					<< "--> 0.Выход..." << endl << ">>";

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
					smart_home.setCurrentUser(nullptr);
					break;
				}
				case 1:
				{
					if (smart_home.getCurrentUser()->getRole() == Guest_)
						smart_home.getCurrentUser()->setRole(User_);

					cin >> *smart_home.getCurrentUser();
					showRegistrationMenu();
					auto user = make_shared<User>();
					user->setRole(temp_role);
					smart_home.setCurrentUser(user);
					break;
				}
				case 2:
				{
					cout << *smart_home.getCurrentUser() << endl;
					if (smart_home.getCurrentUser()->getRole() == Guest_){}
					else {
						string attempt_name;
						string attempt_password;
						cout << "Введите имя пользователя:";
						getline(cin, attempt_name);

						cout << "Введите пароль: ";
						getline(cin, attempt_password);

						smart_home.getCurrentUser()->setUserName(attempt_name);
						smart_home.getCurrentUser()->setPassword(attempt_password);

						showLoginMenu();
						auto user = make_shared<User>();
						user->setRole(temp_role);
						smart_home.setCurrentUser(user);
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

		void MenuInteraction::showDeviceCatalogHeaderMenu()
		{
			cout << "Каталог смарт устройств" << endl
				<< "0. Smart Light" << endl
				<< "1. Thermostat" << endl
				<< "2. Security Camera" << endl;
		}

		void MenuInteraction::showEntryMenu()
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
				case 1:
				{
					auto user = make_shared<User>();
					user->setRole(User_);
					smart_home.setCurrentUser(user);
					showUserMenu();
					smart_home.setCurrentUser(nullptr);
					break;
				}
				case 2:
				{
					auto user = make_shared<User>();
					user->setRole(Admin_);
					smart_home.setCurrentUser(user);
					showAdminMenu();
					smart_home.setCurrentUser(nullptr);
					break;
				}
				case 3:
				{
					auto user = make_shared<User>();
					user->setRole(Guest_);
					smart_home.setCurrentUser(user);
					showGuestMenu();
					smart_home.setCurrentUser(nullptr);
					break;
				}
				case 4:
				{
					is_running = false;
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

	
#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <map>
#include <sstream>

using namespace std;

struct Year {
    Year() {
        year = 0;
    }

    explicit Year (int new_year) {
        year = new_year;
    }
    int year;
};

struct Month {
    Month() {
        month = 1;
    }

    explicit Month (int new_month) {
        if (new_month >= 1 && new_month <= 12) {
            month = new_month;
        } else {
            throw invalid_argument("Month value is invalid: " + to_string(new_month));
        }
    }

    int month;
};

struct Day {
    Day() {
        day = 1;
    }

    explicit Day (int new_day) {
        if (new_day < 1 || new_day > 31) {
            throw invalid_argument("Day value is invalid: " + to_string(new_day));
        } else {
            day = new_day;
        }
    }
    int day;
};

class Date {
public:

    Date() {
        year = Year(0);
        month = Month(1);
        day = Day(1);
    }

    Date(Year new_year, Month new_month, Day new_day) {
        year = new_year;
        month = new_month;
        day = new_day;
    }

  int GetYear() const {
      return year.year;
  }

  int GetMonth() const {
      return month.month;
  }
  int GetDay() const {
      return day.day;
  }

private:
    Year year;
    Month month;
    Day day;
};

ostream& operator << (ostream& stream, const Date& date) {
    if (stream) {
        stream << setw(4) << setfill('0') << date.GetYear() << '-'
        << setw(2) << setfill('0') << date.GetMonth() << '-'
        << setw(2) << setfill('0') << date.GetDay();
    }

    return stream;
}

istream& operator >> (istream& stream, Date& date) {
    int year, month, day;
    string current_date = "";

    if (!(stream >> current_date)) {
        return stream;
    }

    stringstream current_stream(current_date);

    if (!(current_stream >> year)) {
        throw invalid_argument("Wrong date format: " + current_date);
    }

    if (current_stream.peek() == '-') {
        current_stream.ignore(1);
    } else {
        throw invalid_argument("Wrong date format: " + current_date);
    }

    if (!(current_stream >> month)) {
        throw invalid_argument("Wrong date format: " + current_date);
    }

    if (current_stream.peek() == '-') {
        current_stream.ignore(1);
    } else {
        throw invalid_argument("Wrong date format: " + current_date);
    }

    if (!(current_stream >> day)) {
        throw invalid_argument("Wrong date format: " + current_date);
    }

    if (!(current_stream.peek() == EOF)) {
        throw invalid_argument("Wrong date format: " + current_date);
    }

    Year new_year(year);
    Month new_month(month);
    Day new_day(day);
    Date new_date(new_year, new_month, new_day);
    date = new_date;

    return stream;
}

bool operator < (const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() < rhs.GetYear()) {
        return true;
    }
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() < rhs.GetMonth()) {
            return true;
        }

        if (lhs.GetMonth() == rhs.GetMonth()) {
            if (lhs.GetDay() < rhs.GetDay()) {
                return true;
            }
        }
    }

    return false;
}

bool operator == (const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() == rhs.GetYear() &&
        lhs.GetMonth() == rhs.GetMonth() &&
        lhs.GetDay() == rhs.GetDay()) {
        return true;
    }

    return false;
}



class Database {
public:
  void AddEvent(const Date& date, const string& event) {
      date_event[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const string& event) {
      if (date_event.count(date) && date_event[date].count(event)) {
          date_event[date].erase(event);
          if (date_event[date].size() == 0) {
              date_event.erase(date);
          }
          cout << "Deleted successfully" << endl;
      } else {
          cout << "Event not found" << endl;
          return false;
      }

      return true;
  }

  int  DeleteDate(const Date& date) {
      int count_of_events = 0;

      if (date_event.count(date)) {
          count_of_events = date_event[date].size();
          date_event.erase(date);
      }

      cout << "Deleted " + to_string(count_of_events) + " events" << endl;
      return count_of_events;
  }

void Find(const Date& date) const {
      if (date_event.count(date)) {
          for (const auto& event : date_event.at(date)) {
              cout << event << endl;
          }
      }
  }
  
  void Print() const {
      for (const auto& [key, value] : date_event) {
          for (const auto& item : value) {
              cout << key << " " << item << endl;
          }
      }
  }

private:
    map <Date, set <string>> date_event;
};

int main() {
  Database db;
    
  string command;
  while (getline(cin, command)) {
      if (!command.empty()) {
          stringstream stream(command);
          stream >> command;

          string event = "";
          Date date;
          if (command == "Add") {
              try {
                  stream >> date;
                  stream >> event;
                  db.AddEvent(date, event);
              } catch (invalid_argument& ee) {
                  cout << ee.what() << endl;
                  return 1;
              }
          } else if (command == "Del") {
              try {
                  stream >> date;
                  if (stream) {
                      stream >> event;
                  }

                  if (!event.empty()) {
                      db.DeleteEvent(date, event);
                  } else {
                      db.DeleteDate(date);
                  }
              } catch (invalid_argument& ee) {
                  cout << ee.what() << endl;
                  return 2;
              }
          } else if (command == "Find") {
              try {
                  stream >> date;
                  db.Find(date);
              } catch (invalid_argument& ee) {
                  cout << ee.what() << endl;
                  return 3;
              }
          } else if (command == "Print") {
              db.Print();
          } else {
              cout << "Unknown command: " << command << endl;
              return 4;
          }
      }
  }

  return 0;
}

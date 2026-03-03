#include <iostream>
#include <cmath>
using namespace std;
class Time {
private:
    int hours;
    int minutes;
    int seconds;
    Time From_seconds(int seconds_) {
        int total = seconds_ % (24 * 3600);
        if (total < 0) total += 24 * 3600;
        Time result;
        result.hours = total / 3600;
        total %= 3600;
        result.minutes = total / 60;
        result.seconds = total % 60;
        return result;
    }
public:
    Time() : hours(0), minutes(0), seconds(0) {}
    void set_time(int hours_, int minutes_, int seconds_) {
        Time temp = From_seconds(hours_ * 3600 + minutes_ * 60 + seconds_);
        hours = temp.hours; minutes = temp.minutes; seconds = temp.seconds;
    }
    void get_time() const {
        cout << hours << ':' << minutes << ':' << seconds << endl;
    }
    int To_seconds(Time Set_Time) {
        return Set_Time.hours * 3600 + Set_Time.minutes * 60 + Set_Time.seconds;
    }
    void difference(const Time& comparable) {
        int a = To_seconds(*this); int b = To_seconds(comparable);
        Time result = From_seconds(abs(a - b));
        result.get_time();
    }
    void shift_time_up(const Time& up) {
        int current = To_seconds(*this);
        int add = To_seconds(up);
        Time result = From_seconds(current + add);
        result.get_time();
    }
    void shift_time_down(const Time& down) {
        int curr = To_seconds(*this); int sub = To_seconds(down);
        Time result = From_seconds(curr - sub);
        result.get_time();
    }
};
int main() {
    cout << "examples of working with the class" << endl;
    Time t1, t2;
    t1.get_time();
    t1.set_time(13, 8, 21);
    t1.get_time();
    t2.set_time(11, 0, 0);
    t1.difference(t2);
    t1.shift_time_up(t2);
    t1.shift_time_down(t2);
    return 0;
}
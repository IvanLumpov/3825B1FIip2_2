#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    static const int MAX = 40;
    char str[MAX + 1];
    int len;

public:
    String() {
        len = 0;
        str[0] = '\0';
    }
    String(const char* s) {
        if (s == nullptr) {
            len = 0;
            str[0] = '\0';
            return;
        }
        len = strlen(s);
        if (len > MAX) len = MAX;
        for (int i = 0; i < len; i++) {
            str[i] = s[i];
        }
        str[len] = '\0';
    }

    String(const String& other) {
        len = other.len;
        for (int i = 0; i < len; i++) {
            str[i] = other.str[i];
        }
        str[len] = '\0';
    }

    ~String() {}

    String& operator=(const String& other) {
        if (this != &other) {
            len = other.len;
            for (int i = 0; i < len; i++) {
                str[i] = other.str[i];
            }
            str[len] = '\0';
        }
        return *this;
    }

    void set() {
        char temp[100];
        cout << "Enter string: ";
        cin >> temp;
        len = strlen(temp);
        if (len > MAX) len = MAX;
        for (int i = 0; i < len; i++) {
            str[i] = temp[i];
        }
        str[len] = '\0';
        cout << "OK\n";
    }

    void showLen() {
        cout << "Length: " << len << endl;
    }

    void getChar() {
        if (len == 0) {
            cout << "String is empty\n";
            return;
        }
        int idx;
        cout << "Index (0-" << len - 1 << "): ";
        cin >> idx;
        if (idx >= 0 && idx < len) {
            cout << "Char: " << str[idx] << endl;
        }
        else {
            cout << "Bad index\n";
        }
    }

    void setChar() {
        if (len == 0) {
            cout << "String is empty\n";
            return;
        }
        int idx;
        char ch;
        cout << "Index (0-" << len - 1 << "): ";
        cin >> idx;
        if (idx >= 0 && idx < len) {
            cout << "New char: ";
            cin >> ch;
            str[idx] = ch;
            cout << "OK\n";
        }
        else {
            cout << "Bad index\n";
        }
    }

    void sub() {
        if (len == 0) {
            cout << "String is empty\n";
            return;
        }
        int start, end;
        cout << "Start: ";
        cin >> start;
        cout << "End: ";
        cin >> end;
        if (start < 0) start = 0;
        if (end >= len) end = len - 1;
        if (start > end) {
            cout << "Bad indices\n";
            return;
        }
        cout << "Substring: ";
        for (int i = start; i <= end; i++) {
            cout << str[i];
        }
        cout << endl;
    }

    void pal() {
        if (len == 0) {
            cout << "String is empty\n";
            return;
        }
        bool ok = true;
        for (int i = 0; i < len / 2; i++) {
            if (str[i] != str[len - 1 - i]) {
                ok = false;
                break;
            }
        }
        if (ok) cout << "Palindrome\n";
        else cout << "Not palindrome\n";
    }

    void latin() {
        if (len == 0) {
            cout << "String is empty\n";
            return;
        }
        bool letters[26] = { false };
        int count = 0;
        for (int i = 0; i < len; i++) {
            char c = str[i];
            if (c >= 'a' && c <= 'z') {
                if (!letters[c - 'a']) {
                    letters[c - 'a'] = true;
                    count++;
                }
            }
            if (c >= 'A' && c <= 'Z') {
                if (!letters[c - 'A']) {
                    letters[c - 'A'] = true;
                    count++;
                }
            }
        }
        cout << "Different Latin letters: " << count << endl;
    }

    void print() {
        cout << str << endl;
    }
};
int main() {
    String s;
    int choice;

    cout << "=== STRING ===\n";
    cout << "Start with empty string. Use 1 to set.\n\n";

    do {
        cout << "1.Set 2.Length 3.Get char 4.Set char 5.Substring\n";
        cout << "6.Palindrome 7.Latin letters 8.Print 0.Exit\n";
        cout << "Choice: ";
        cin >> choice;
        cout << endl;

        switch (choice) {
        case 1: s.set(); break;
        case 2: s.showLen(); break;
        case 3: s.getChar(); break;
        case 4: s.setChar(); break;
        case 5: s.sub(); break;
        case 6: s.pal(); break;
        case 7: s.latin(); break;
        case 8: s.print(); break;
        case 0: cout << "Bye\n"; break;
        default: cout << "Wrong choice\n";
        }
        cout << endl;
    } while (choice != 0);

    return 0;
}
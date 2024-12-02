/*
 * use ctime for the Date
 * 2024.12.02 by dralee
 *
 * Datetime structures are structures that represent different components of the
date and time as members. This makes it easier for us to specify dates. Datetime
structures have the following members:

tm_sec - The seconds within a minute
tm_min - The minutes within an hour
tm_hour - The hour within a day (from 0 to 23)
tm_mday - The day of the month
tm_mon - The month (from 0 to 11 starting with January)
tm_year - The number of years since 1900
tm_wday - The weekday (from 0 to 6 starting with Sunday)
tm_yday - The day of the year (from 0 to 365 with 0 being January 1)
tm_isdst - Positive when daylight saving time is in effect, zero when not in
effect and negative when unknown Always keep in mind the way that date
components are represented:

Hours are represented in 24-hour format. 11pm would be represented as 23.
The months go from 0 to 11. For example, December would be represented as 11
rather than 12. Years are represented relative to the year 1900. The year 2024
would be represented as 124 because 124 years have passed since 1900.

The ctime() and asctime() functions allow us to display the date but they do not allow us to choose how it is displayed.

To choose how a date is displayed we can use the strftime() function.

Format Specifier	Description	Example
%a	Short representation of the weekday	Fri
%b	Short representation of the month name	Dec
%B	Full representation of the month name	December
%d	Day of the month with leading zero	09
%e	Day of the month with leading spaces	 9
%H	24-hour format of an hour	14
%I	12-hour format of an hour	02
%M	Minutes within an hour	30
%p	AM or PM	PM
%S	Seconds within a minute	01
%y	2-digit year representation	23
%Y	4-digit year representation	2023

The clock() function is useful for measuring short intervals of time while the program is running. It is more precise than the difftime() function.
 */
#include <ctime>
#include <iostream>
using namespace std;

void now() {
    // get the timestamp for current date and time
    time_t timestamp;
    time(&timestamp);

    // display date and time by timestamp
    cout << ctime(&timestamp);

    time_t timestamp2 = time(NULL);
    cout << ctime(&timestamp2);
}

void create_timestamp() {
    struct tm datetime;
    time_t timestamp;

    datetime.tm_year = 2024 - 1900; // number of year since 1900
    datetime.tm_mon = 12 - 1;       // number of month since January
    datetime.tm_mday = 2;
    datetime.tm_hour = 17;
    datetime.tm_min = 20;
    datetime.tm_sec = 12;

    // daylight savings must be specified
    // -1 uses the computer's timezone setting
    datetime.tm_isdst = -1; //-1;

    timestamp = mktime(&datetime);

    cout << "create timestamp" << endl;
    cout << ctime(&timestamp);
}

void create_datetime() {
    // create the datetime struct and use mktime to fill in the missing members
    struct tm datetime;
    datetime.tm_year = 2024 - 1900; // number of years since 1900
    datetime.tm_mon = 12 - 1;       // number of month since January
    datetime.tm_mday = 2;
    datetime.tm_hour = 17;
    datetime.tm_min = 25;
    datetime.tm_sec = 52;
    datetime.tm_isdst = -1;
    mktime(&datetime);

    string weekdays[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                         "Thursday", "Friday", "Saturday"};

    cout << "the date is on a " << weekdays[datetime.tm_wday];
}

void local_time(){
	time_t timestamp = time(&timestamp); // time(NULL);
	struct tm datetime = *localtime(&timestamp);
	
	cout<<"the local time"<<endl;
	cout<<datetime.tm_hour<<endl;
	cout<<asctime(&datetime);	
}

void format_time(){
	time_t timestamp = time(NULL);
	struct tm datetime = *localtime(&timestamp);

	char output[50];

	cout<<"format the date"<<endl;

	strftime(output, 50, "%B %e, %Y", &datetime);
	cout<<output<<endl;
	
	strftime(output, 50, "%I:%M:%S %p", &datetime);
	cout<<output<<endl;
	
	strftime(output, 50, "%m/%d/%y", &datetime);
	cout<<output<<endl;

	strftime(output, 50, "%a %b %e %H:%M:%S %Y", &datetime);
	cout<<output<<endl;

}

void different_time(){
	time_t now;
 	time_t nexyear;
	struct tm datetime;

	now = time(NULL);
	datetime = *localtime(&now);
	datetime.tm_year = datetime.tm_year + 1;
	datetime.tm_mon = 0;
	datetime.tm_mday = 1;
	datetime.tm_hour = 0;
	datetime.tm_min = 0;
	datetime.tm_sec = 0;
	datetime.tm_isdst = -1;
	nexyear = mktime(&datetime);

	int diff = difftime(nexyear, now);
	cout<<diff<<" seconds until next year"<<endl;
}

void clock_time(){
	clock_t before = clock();
	int k = 0;
	for(int i = 0; i < 100000; ++i){
		k += i;
	}
	clock_t duration = clock() - before;
	cout<<"Duration: "<<(float)duration/CLOCKS_PER_SEC<<" seconds";
}

int main(int argc, char **argv) {
    now();

    create_timestamp();

	create_datetime();

	local_time();
	
	format_time();

	different_time();

	clock_time();

    return 0;
}
